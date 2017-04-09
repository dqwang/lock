#include "main.h"


/*
** 测试函数
*/
extern void TyteA_Test(void);
extern void TyteB_Test(void);
extern void MifareCard_Test (uint8_t command);

/*********************************************************************************************************
** Function name:       UartCommand()
** Descriptions:        串口接收到数据，执行测试命令
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void UartCommand(void)
{
    
    if(GucRcvNew ==1)  {                                                /* 串口接收到数据               */
        MifareCard_Test( Recv_Buff[0] );                                /* 第一个串口数据位命令类型     */
        GucRcvNew = 0;                                                  /* 清空命令参数                 */
        memset(Recv_Buff,0x00,Maxlength);                               /* 数据请0                      */
    }
}

/*********************************************************************************************************
** 程序功能：Demo板通过串口接收命令，根据不同的命令编号执行不同的操作----波特率固定115200
** 例程功能：通过CON引脚的电平进入两种不同模式：  CON=1-----模式1（卡片自动检测模式）---绿灯常亮
**     此模式下，卡片会一直检测卡片，当有卡片靠近天线时，会读取卡片的ID号并通过串口打印ID号
**     输出格式波特率115200，字符输出
**                                                CON=0-----模式2（串口命令控制模式，介绍如下）
** 注意：进入循环寻卡模式后，Demo板就不能接受命令了，要想返回前面的命令模式，则需要重新上电
**
*********************************************************************************************************/

#define GPIO_HIGH 1
#define GPIO_LOW 0
#define GPIO_OUTPUT 1
#define GPIO_INPUT 0

#define GPIO_BEEP	(1 << 14)//beep P0.14


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

void gpio_init_beep(void)
{
	gpio_dir(GPIO_BEEP, GPIO_OUTPUT);
    gpio_ctrl(GPIO_BEEP, GPIO_LOW);
}


void delay_ms (uint32_t ulTime)
{
    uint32_t i;
    
    while (ulTime--) {
        for (i = 0; i < 2401; i++);
    }
}

typedef unsigned char u8;

#define GPIO_LED1	(1 << 17)//LED1 P0.17
#define GPIO_LED2	(1 << 18)//LED2 P0.18
#define GPIO_LED3	(1 << 19)//LED3 P0.19

#define GPIO_BEEP1	(1 << 12)//beep P0.12

void gpio_init_led(void)
{
	gpio_dir(GPIO_LED1, GPIO_OUTPUT);
	gpio_dir(GPIO_LED2, GPIO_OUTPUT);
	gpio_dir(GPIO_LED3, GPIO_OUTPUT);
    gpio_ctrl(GPIO_LED1, GPIO_LOW);
	gpio_ctrl(GPIO_LED2, GPIO_HIGH);
	gpio_ctrl(GPIO_LED3, GPIO_HIGH);

	gpio_dir(GPIO_BEEP1, GPIO_OUTPUT);
    gpio_ctrl(GPIO_BEEP1, GPIO_LOW);
}




void hwapi02_led1_ctrl(u8 on_off)
{
	if (on_off > 0){
		gpio_ctrl(GPIO_LED1, GPIO_LOW);
	}else{
		gpio_ctrl(GPIO_LED1, GPIO_HIGH);
	}
}

void hwapi02_led2_ctrl(u8 on_off)
{
	if (on_off > 0){
		gpio_ctrl(GPIO_LED2, GPIO_LOW);
	}else{
		gpio_ctrl(GPIO_LED2, GPIO_HIGH);
	}
}

void hwapi02_led3_ctrl(u8 on_off)
{
	if (on_off > 0){
		gpio_ctrl(GPIO_LED3, GPIO_LOW);
	}else{
		gpio_ctrl(GPIO_LED3, GPIO_HIGH);
	}
}


void test_hwapi02_led_ctrl(void)
{
	hwapi02_led1_ctrl(1);//led on
	hwapi02_led2_ctrl(1);//led on
	hwapi02_led3_ctrl(1);//led on
	delay_ms(1000);
	hwapi02_led1_ctrl(0);//led off
	hwapi02_led2_ctrl(0);//led off
	hwapi02_led3_ctrl(0);//led off	
	delay_ms(1000);
}



int main(void)
{
    volatile unsigned char statues;
    SystemInit();                                                       /* MCU系统初始化                */
    Delay100us(1000);

	gpio_init_beep();

	
    GPIOInit();                                                         /* GPIO初始化                   */
    pcd_Init();                                                         /* 读卡芯片初始化               */
    UARTInit();                                                         /* MCU串口初始化                */
	
	//uartPrintf("wdq debug\r\n");
	//Delay100us(100);


	
	statues = 0;
    while(statues == 0)  {                                              /* 判断SPI接口是否正常通信      */
         statues = MCU_TO_PCD_TEST();            
    }
    if(statues == TRUE){                                                /* 通信正常，红灯亮一下         */
        LED_RedOn();
        uartPrintf("FM175xx Demo Test Start!\r\n",statues);
        Delay100us(10000);
        LED_RedOff();
						
        while (1) {
            //if( (LPC_GPIO_PORT->PIN[0]& CON) == 0 )   {                 /* CON接地进入串口解析命令      */
              //  UartCommand();                                          /* 串口命令                     */
            //}
            //else  {                                                     /* 通常自动寻卡                 */
                TyteA_Test();                                           /* ISO14443A自动寻卡测试        */
                //TyteB_Test();                                           /* ISO14443B自动寻卡测试        */
            //}
        }
                
    }
}






