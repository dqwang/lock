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

    regVal0 = LPC_ADC->CTRL;                                            /* ����ADC CTRL�Ĵ���ֵ	        */
    
    NVIC_DisableIRQ(ADC_SEQA_IRQn);
    NVIC_DisableIRQ(ADC_SEQB_IRQn);
    
    LPC_ADC->CTRL = ((0x1 << 30) | (SystemCoreClock / 500000 - 1));     /* У׼ADC������ADCʱ��Ϊ500kHZ */
    while ( LPC_ADC->CTRL & (0x1 << 30) );
    LPC_ADC->CTRL = regVal0;                                            /* У׼��ϣ����¸�ֵADC CTRL   */
    return;	
}



void power_adc_init(void)
{
    ADCValue = 0x0; 
    
    LPC_SYSCON->PDRUNCFG &= ~(0x1 << 4 );                               /* ��ADCģ���ϵ�                */                    
    LPC_SYSCON->SYSAHBCLKCTRL |= ( 0x1 << 24 );                         /* ʹ��ADCʱ��                  */
    
    LPC_SWM->PINENABLE0 &= ~(0x1 << 23);                                /* ADC_10ͨ��ʹ����PIO0_13        */
    LPC_IOCON->PIO0[IOCON_PIO13] = 0;                                   /* ģ������PIO0_13 ����Ϊ����̬  */
    
    LPC_ADC->CTRL = (0x0 << 0)|                                         /* 0��Ƶ                        */
                    (0x0 << 10);                                        /* �رյ͹���ģʽ               */

    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] = (1<<0) |                          /* ��ͨ��0ʹ����SQEA��          */
                                      (1<<18);                          /* �����ش���                   */
                                                                         
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= ((u32)1<<31);                         /* ʹ��SEQA                     */       
    LPC_ADC->INTEN = (0x1 << 0);                                        /* ʹ��SEQA�ж�                 */ 
    NVIC_EnableIRQ(ADC_SEQA_IRQn);
       
    return;	
}

void ADC_SEQA_IRQHandler( void )
{                                                              
    if((LPC_ADC->SEQ_GDAT[ADC_SEQA_IDX] & ((u32)0x1 << 31))){                /* �жϷ�ʽ��ȡ����             */
        ADCValue = (LPC_ADC->SEQ_GDAT[ADC_SEQA_IDX] >> 4 ) & 0xFFF;
    }
}

void power_adc_start(uint8_t ucChannel)
{
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] = (0x1 << ucChannel) |              /* ��ͨ��xʹ����SQEA��          */
                                      (0x1 << 18);     
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= ((u32)0x1 << 31);                     /* ʹ��SQEA                     */     
    LPC_ADC->SEQ_CTRL[ADC_SEQA_IDX] |= (0x1 << 26) ;                    /* �����ʽ����ADCת��          */
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
