/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Technology Co., LTD
**
**                                 http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           LPC8xx_Gpio.c
** Last modified Date:  2016-1-4
** Last Version:        V1.00
** Descriptions:        ����LPC824��GPIO����Ҫ���ڿ���ָʾ��LED�Ϳ���FM175xx����оƬ��λ��
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
** Descriptions:        GPIO��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void GPIOInit (void)  {
    
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��           */
    LPC_IOCON->PIO0[IOCON_PIO10] &=  ~(0x03UL<<8);                      /* ����P0_10 P0_11 IO����       */
    LPC_IOCON->PIO0[IOCON_PIO10] |=   (0x01UL<<8); 
    LED_GreenCfg();                                                     /* �����̵�IOΪ���             */
    LED_GreenOn();                                                      /* �����̵�                     */
    LED_RedCfg();
    LED_RedOn();                   
    CON_Input();                                                        /* ����CONΪ��������            */
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


 










