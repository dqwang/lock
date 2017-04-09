/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      广州周立功单片机科技有限公司所提供的所有服务内容旨在协助客户加速产品的研发进度，在服务过程中所提供
**  的任何程序、文档、测试结果、方案、支持等资料和信息，都仅供参考，客户有权不使用或自行参考修改，本公司不
**  提供任何的完整性、可靠性等保证，若在客户使用过程中因任何原因造成的特别的、偶然的或间接的损失，本公司不
**  承担任何责任。
**                                                                        ——广州周立功单片机科技有限公司
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           LPC8xx_Gpio.c
** Last modified Date:  2016-1-4
** Last Version:        V1.00
** Descriptions:        操作LPC824的GPIO，主要用于控制指示灯LED和控制FM175xx读卡芯片复位。
**
**--------------------------------------------------------------------------------------------------------
*/

#include "LPC8xx.h"
#include "LPC8xx_IO.h"
#include "LPC8xx_Function.h"
#include "Utility.h"


#define GPIO_HIGH 1
#define GPIO_LOW 0
#define GPIO_OUTPUT 1
#define GPIO_INPUT 0

#define GPIO_INA (1<<17)
#define GPIO_INB (1<<18)




/*********************************************************************************************************
** Function name:       GPIOInit
** Descriptions:        GPIO初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void GPIOInit (void)  {
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* 初始化GPIO AHB时钟           */
    LPC_IOCON->PIO0[IOCON_PIO10] &=  ~(0x03UL<<8);                      /* 设置P0_10 P0_11 IO功能       */
    LPC_IOCON->PIO0[IOCON_PIO10] |=   (0x01UL<<8); 
    LED_GreenCfg();                                                     /* 配置绿灯IO为输出             */
    LED_GreenOn();                                                      /* 点亮绿灯                     */
    LED_RedCfg();
    LED_RedOn();                   
    CON_Input();                                                        /* 配置CON为输入引脚            */
	gpio_lg9110_init();
}



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


 










