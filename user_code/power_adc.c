#include "main.h"

volatile uint32_t ADCValue = 0;
volatile u8 g_adc_flag = POWER_INVALID;
extern rfac_u gw_addr_channel;

extern timer_t adc_timer;

void power_adc_alibration( void )
{
    uint32_t regVal0;


    LPC_SYSCON->PDRUNCFG &= ~(0x1 << 4 );                               
    LPC_SYSCON->SYSAHBCLKCTRL |= ( 0x1 << 24 );

    regVal0 = LPC_ADC->CTRL;                                            /* 保存ADC CTRL寄存器值	        */
    
    NVIC_DisableIRQ(ADC_SEQA_IRQn);
    NVIC_DisableIRQ(ADC_SEQB_IRQn);
    
    LPC_ADC->CTRL = ((0x1 << 30) | (SystemCoreClock / 500000 - 1));     /* 校准ADC，设置ADC时钟为500kHZ */
    while ( LPC_ADC->CTRL & (0x1 << 30) );
    LPC_ADC->CTRL = regVal0;                                            /* 校准完毕，重新赋值ADC CTRL   */
    return;	
}



void power_adc_init(void)
{
    ADCValue = 0x0; 
    
    LPC_SYSCON->PDRUNCFG &= ~(0x1 << 4 );                               /* 给ADC模块上电                */                    
    LPC_SYSCON->SYSAHBCLKCTRL |= ( 0x1 << 24 );                         /* 使能ADC时钟                  */
    
    LPC_SWM->PINENABLE0 &= ~(0x1 << 23);                                /* ADC_10通道使能在PIO0_13        */
    LPC_IOCON->PIO0[IOCON_PIO13] = 0;                                   /* 模拟引脚PIO0_13 配置为高阻态  */
    
    LPC_ADC->CTRL = (0x0 << 0)|                                         /* 0分频                        */
                    (0x0 << 10);                                        /* 关闭低功耗模式               */

    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] = (1<<0) |                          /* 将通道0使能在SQEA中          */
                                      (1<<18);                          /* 上升沿触发                   */
                                                                         
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= ((u32)1<<31);                         /* 使能SEQA                     */       
    LPC_ADC->INTEN = (0x1 << 0);                                        /* 使能SEQA中断                 */ 
    NVIC_EnableIRQ(ADC_SEQA_IRQn);
       
    return;	
}

void ADC_SEQA_IRQHandler( void )
{                                                              
    if((LPC_ADC->SEQ_GDAT[ADC_SEQA_IDX] & ((u32)0x1 << 31))){                /* 中断方式读取数据             */
        ADCValue = (LPC_ADC->SEQ_GDAT[ADC_SEQA_IDX] >> 4 ) & 0xFFF;
    }
}

void power_adc_start(uint8_t ucChannel)
{
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] = (0x1 << ucChannel) |              /* 将通道x使能在SQEA中          */
                                      (0x1 << 18);     
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= ((u32)0x1 << 31);                     /* 使能SQEA                     */     
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= (0x1 << 26) ;                    /* 软件方式启动ADC转换          */
}



void power_adc_thread(void)
{
	if (adc_timer.flag){
		adc_timer_reset();
		g_adc_flag = power_adc_check(ADCValue);
		switch (g_adc_flag){
			case POWER_OK:{
				
			}break;

			case POWER_LOW_ALARM:
			case POWER_LOW_WARNING:{
				cmd16_CMD_REPORT_LOW_VOLTAGE((power_adc_t)g_adc_flag);
			}break;
			default:{//POWER_INVALID
			
			}break;			
		}		
	}
	
}

void test_power_adc(void)
{
	u8 buf[20]={0};

	memcpy(buf, gw_addr_channel.rfac0, sizeof(rfac_u));
	
	
	power_adc_start(POWER_ADC_CHANNEL);

	SH_PUT_4_BYTE(buf+sizeof(rfac_u), ADCValue);
	
	SH_PUT_4_BYTE(buf+sizeof(rfac_u)+sizeof(u32), ADCValue * 3300 / 4095);
	

	send2gateway(buf,sizeof(buf));
	delay_ms(1000);
}

power_adc_t power_adc_check(uint32_t adc_value)
{
	power_adc_start(POWER_ADC_CHANNEL);
	
	if (adc_value <= LOW_VOLTAGE_WARNING && adc_value > LOW_VOLTAGE_ALARM){
		return POWER_LOW_WARNING;
	}

	if (adc_value <= LOW_VOLTAGE_ALARM){
		return POWER_LOW_ALARM;
	}
	return POWER_OK;
}

void test_power_adc_check(void)
{
	g_adc_flag = power_adc_check(ADCValue);
	switch (g_adc_flag){
		case POWER_OK:
		{
			hwapi01_beep_cnt(1,1000);
		}break;

		case POWER_LOW_WARNING:
		{
			hwapi01_beep_cnt(3, 100);
			delay_ms(700);
		}break;

		case POWER_LOW_ALARM:
		{
			hwapi01_beep_cnt(5, 100);
			delay_ms(500);
			
		}break;
		default:{

		}break;
	}
}
