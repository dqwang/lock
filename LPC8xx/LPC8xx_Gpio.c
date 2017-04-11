#include "main.h"

void gpio_ctrl(uint32_t gpio, uint32_t value)
{
	if (value == GPIO_LOW)
		LPC_GPIO_PORT->PIN[0] &= ~gpio;
	if (value == GPIO_HIGH)
		LPC_GPIO_PORT->PIN[0] |=  gpio;
}

void gpio_dir(uint32_t gpio, uint32_t dir)
{
	if (dir == GPIO_INPUT)
		LPC_GPIO_PORT->DIR[0] &= ~gpio;
	if (dir == GPIO_OUTPUT)
		LPC_GPIO_PORT->DIR[0] |=  gpio;
}

void GPIOInit (void)  {
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
    LPC_IOCON->PIO0[IOCON_PIO10] &=  ~(0x03UL<<8);                      /* 设置P0_10 P0_11 IO功能       */
    LPC_IOCON->PIO0[IOCON_PIO10] |=   (0x01UL<<8);                   

	gpio_init_rf433m_power();
	gpio_init_rf433m_mode();

	CON_Input();                                                        /* 配置CON为输入引脚            */
	gpio_init_beep();
	//gpio_lg9110_init();
}


void gpio_init_beep(void)
{
	gpio_dir(GPIO_BEEP, GPIO_OUTPUT);
    gpio_ctrl(GPIO_BEEP, GPIO_LOW);
}

void hwapi01_beep_crtl(u8 on_off)
{
	if (on_off > 0){
		gpio_ctrl(GPIO_BEEP, GPIO_HIGH);
	}else{
		gpio_ctrl(GPIO_BEEP, GPIO_LOW);
	}
}

void test_hwapi01_beep_crtl(void)
{
	hwapi01_beep_crtl(1);//beep on
	//delay_ms(1000);
	WKTdelayMs(500);
	hwapi01_beep_crtl(0);//beep off
	WKTdelayMs(500);
	//delay_ms(1000);
}


/*
#define GPIO_INA (1<<17)
#define GPIO_INB (1<<18)


void gpio_lg9110_init(void)
{
	gpio_dir(GPIO_INA, GPIO_OUTPUT);
    gpio_ctrl(GPIO_INA, GPIO_LOW);

	gpio_dir(GPIO_INB, GPIO_OUTPUT);
    gpio_ctrl(GPIO_INB, GPIO_LOW);
	
}
void open_door(void)
{
	gpio_ctrl(GPIO_INA, GPIO_LOW);
	gpio_ctrl(GPIO_INB, GPIO_HIGH);

	Delay100us(1000);

	gpio_ctrl(GPIO_INA, GPIO_LOW);
	gpio_ctrl(GPIO_INB, GPIO_LOW);

	Delay100us(90000);
	gpio_ctrl(GPIO_INA, GPIO_HIGH);
	gpio_ctrl(GPIO_INB, GPIO_LOW);

	Delay100us(1000);

	gpio_ctrl(GPIO_INA, GPIO_LOW);
	gpio_ctrl(GPIO_INB, GPIO_LOW);
}

*/
