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


int main(void)
{
    volatile unsigned char statues;
    SystemInit();                                                       /* MCUϵͳ��ʼ��                */
    Delay100us(1000);

	gpio_init_beep();

	
    GPIOInit();                                                         /* GPIO��ʼ��                   */
    pcd_Init();                                                         /* ����оƬ��ʼ��               */
    UARTInit();                                                         /* MCU���ڳ�ʼ��                */

#if 0// TDD
	while(1){
		test_hwapi01_beep_crtl();

	}
#else
	
	
	statues = 0;
    while(statues == 0)  {                                              /* �ж�SPI�ӿ��Ƿ�����ͨ��      */
         statues = MCU_TO_PCD_TEST();            
    }
    if(statues == TRUE){                                                /* ͨ�������������һ��         */
        hwapi01_beep_crtl(ON);
        uartPrintf("FM175xx Demo Test Start!\r\n",statues);
        Delay100us(10000);
        hwapi01_beep_crtl(OFF);
						
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

#endif
}

