#include "main.h"

volatile u16 test_timer = 0;
u8 test_flag = 0;

timer_t open_door_timer;
timer_t adc_timer;


void delay_ms (uint32_t ulTime)
{
    uint32_t i;
    
    while (ulTime--) {
        for (i = 0; i < 2401; i++);
    }
}

void WKTInit (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 9);                             /* ��ʼ��WKT AHBʱ��            */
    LPC_SYSCON->PRESETCTRL    &= ~(1 << 9);
    LPC_SYSCON->PRESETCTRL    |=  (1 << 9);                             /* ��λWKT                      */
    
    /*
     * WKTʱ��Դѡ��
     * 0��IRC�����ڲ�RCʱ��Դ��750 kHz��    1��LPO�����͹���������10 kHz������ +/-45%��
     * WKT��ʱʱ�����ã�
     * IRC��N(ms) ���� COUNT = 750 * N      LPO��N(ms) ���� COUNT = 10 * N
     */
    LPC_WKT->CTRL  = 0;                                                 /* IRCΪʱ��Դ                  */

	LPC_WKT->COUNT = 750 * 1;// 1ms
	NVIC_EnableIRQ(WKT_IRQn);

	open_door_timer_reset();
	adc_timer_reset();
}


void WKT_IRQHandler (void)
{
    if (LPC_WKT->CTRL & (1 << 1)) {
        LPC_WKT->CTRL |= (1 << 1);                                      /* ����жϱ�־                 */
		
		LPC_WKT->COUNT = 750 * 1;// 1ms
		test_timer++;
		open_door_timer.cnt++;

		adc_timer.cnt++;
		if(adc_timer.cnt >= ADC_TIME){
			adc_timer_set();
		}
    }
}


void WKTdelayMs (uint32_t delayInMs)
{
    LPC_WKT->COUNT = 750 * delayInMs;    
    while( !(LPC_WKT->CTRL & (1 << 1))) {;}    
    LPC_WKT->CTRL |= (1 << 1);
}


void test_timer_reset(void)
{
	test_timer = 0;
	test_flag = 0;
}

void test_timer_thread(void)
{
	if (test_timer<500 && test_flag == 0){
		hwapi01_beep_crtl(ON);
		test_flag = 1;
	}

	if (test_timer >=500 && test_flag == 1){
		hwapi01_beep_crtl(OFF);
		test_flag = 2;
	}

	if (test_timer>1000 && test_flag == 2){
		test_timer_reset();
	}
}

void open_door_timer_set(void)
{
	open_door_timer.cnt = 0;
	open_door_timer.flag = 1;
}



void open_door_timer_reset(void)
{
	CLEAR_TYPE(&open_door_timer, timer_t);
}

void adc_timer_reset(void)
{
	adc_timer.cnt = 0;
	adc_timer.flag = 0;
}

void adc_timer_set(void)
{
	adc_timer.cnt = 0;
	adc_timer.flag = 1;
}

