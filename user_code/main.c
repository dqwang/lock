#include "main.h"


/*
** ���Ժ���
*/
extern void TyteA_Test(void);
extern void TyteB_Test(void);
extern void MifareCard_Test (uint8_t command);

/*********************************************************************************************************
** Function name:       UartCommand()
** Descriptions:        ���ڽ��յ����ݣ�ִ�в�������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UartCommand(void)
{
    
    if(GucRcvNew ==1)  {                                                /* ���ڽ��յ�����               */
        MifareCard_Test( Recv_Buff[0] );                                /* ��һ����������λ��������     */
        GucRcvNew = 0;                                                  /* ����������                 */
        memset(Recv_Buff,0x00,Maxlength);                               /* ������0                      */
    }
}

/*********************************************************************************************************
** �����ܣ�Demo��ͨ�����ڽ���������ݲ�ͬ��������ִ�в�ͬ�Ĳ���----�����ʹ̶�115200
** ���̹��ܣ�ͨ��CON���ŵĵ�ƽ�������ֲ�ͬģʽ��  CON=1-----ģʽ1����Ƭ�Զ����ģʽ��---�̵Ƴ���
**     ��ģʽ�£���Ƭ��һֱ��⿨Ƭ�����п�Ƭ��������ʱ�����ȡ��Ƭ��ID�Ų�ͨ�����ڴ�ӡID��
**     �����ʽ������115200���ַ����
**                                                CON=0-----ģʽ2�������������ģʽ���������£�
** ע�⣺����ѭ��Ѱ��ģʽ��Demo��Ͳ��ܽ��������ˣ�Ҫ�뷵��ǰ�������ģʽ������Ҫ�����ϵ�
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
    SystemInit();                                                       /* MCUϵͳ��ʼ��                */
    Delay100us(1000);

	gpio_init_beep();

	
    GPIOInit();                                                         /* GPIO��ʼ��                   */
    pcd_Init();                                                         /* ����оƬ��ʼ��               */
    UARTInit();                                                         /* MCU���ڳ�ʼ��                */
	
	//uartPrintf("wdq debug\r\n");
	//Delay100us(100);


	
	statues = 0;
    while(statues == 0)  {                                              /* �ж�SPI�ӿ��Ƿ�����ͨ��      */
         statues = MCU_TO_PCD_TEST();            
    }
    if(statues == TRUE){                                                /* ͨ�������������һ��         */
        LED_RedOn();
        uartPrintf("FM175xx Demo Test Start!\r\n",statues);
        Delay100us(10000);
        LED_RedOff();
						
        while (1) {
            //if( (LPC_GPIO_PORT->PIN[0]& CON) == 0 )   {                 /* CON�ӵؽ��봮�ڽ�������      */
              //  UartCommand();                                          /* ��������                     */
            //}
            //else  {                                                     /* ͨ���Զ�Ѱ��                 */
                TyteA_Test();                                           /* ISO14443A�Զ�Ѱ������        */
                //TyteB_Test();                                           /* ISO14443B�Զ�Ѱ������        */
            //}
        }
                
    }
}






