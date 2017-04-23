#include "main.h"


u8 touch_flag = 0;
extern timer_t open_door_timer;

void touch_init(void)
{
	gpio_dir(GPIO_TOUCH, GPIO_INPUT);

	LPC_SYSCON->PINTSEL[0] = 0x0a;
	LPC_PININT->ISEL &= ~0x1;
	LPC_PININT->IENR |= 0x1;

	NVIC_EnableIRQ(PIN_INT0_IRQn);
}

void PIN_INT0_IRQHandler (void)
{
	NVIC_DisableIRQ(PIN_INT0_IRQn);
	LPC_PININT->IST = 0x1;//clear interrupt status

	touch_flag = 1;

	if(open_door_timer.flag != 1){
		cmd30_CMD_REPORT_TOUCH();
		hwapi01_beep_cnt(1,100);	
	}
	//else{
		
	//}
	
	NVIC_EnableIRQ(PIN_INT0_IRQn);
}

void test_touch(void)
{
	if (touch_flag == 1){
		touch_flag =0;

		cmd30_CMD_REPORT_TOUCH();
		hwapi01_beep_cnt(1,100);
	}
}


