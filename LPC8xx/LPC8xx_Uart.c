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
** File name:           LPC8xx_Uart.c
** Last modified Date:  2016-1-14
** Last Version:        V1.00
** Descriptions:        ����LPC824��UART�������������
**
**--------------------------------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdarg.h>

#include "LPC8xx.h"
#include "LPC8xx_Uart.h"
#include "stdint.h"

/*********************************************************************************************************
  �궨��
*********************************************************************************************************/
#define UART_BPS        115200                                          /* ����ͨ�Ų�����               */
#define BUFFSIZE        64                                              /* BUFF��С                     */

/*********************************************************************************************************
  ȫ�ֱ���
*********************************************************************************************************/
char     GucRcvBuf[BUFFSIZE];                                           /* ���ڽ������ݻ�����           */
uint32_t GuiIn;                                                         /* ���ڽ��ռ�������             */
uint32_t GuiOut;                                                        /* ���ڷ��ͼ�������             */
uint32_t GuiNum;                                                        /* BUFF����Ч�����ݸ���         */


/*********************************************************************************************************
** UART ���ݶ���
*********************************************************************************************************/
volatile    uint8_t          GucRcvNew;                                 /* ���ڽ��յ����ݱ�־           */
uint8_t     Send_Buff[Maxlength];                                       /* ���ݷ��ͻ���                 */
uint8_t     Recv_Buff[Maxlength];                                       /* ���ݽ��ջ�����               */
uint8_t     GulNum;                                                     /* ���յ����ݸ���               */

/*********************************************************************************************************
** Function name:       UARTInit
** Descriptions:        UART��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UARTInit (void)
{
    LPC_SWM->PINASSIGN[0] &= ~( 0xFFFF << 0 );
    LPC_SWM->PINASSIGN[0] |=  ( 0 << 0 );                               /* P0.4 ~ UART0_TXD             */
    LPC_SWM->PINASSIGN[0] |=  ( 6 << 8 );                               /* P0.0 ~ UART0_RXD             */
    
    LPC_SYSCON->UARTCLKDIV     = 1;                                     /* UARTʱ�ӷ�ƵֵΪ 1           */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 14);                             /* ��ʼ��UART AHBʱ��           */
    
    LPC_USART0->BRG = SystemCoreClock * LPC_SYSCON->SYSAHBCLKDIV /
                      (LPC_SYSCON->UARTCLKDIV * 16 * UART_BPS) - 1;     /* ����ͨ�Ų�����               */
    LPC_USART0->CFG = (1 << 0) |                                        /* ʹ��UART                     */
                      (1 << 2) |                                        /* 8λ����λ                    */
                      (0 << 4) |                                        /* ��У��                       */
                      (0 << 6);                                         /* 1λֹͣλ                    */
    
    LPC_USART0->INTENSET = (1 << 0);                                    /* ʹ�ܽ����ж�                 */
    NVIC_EnableIRQ(UART0_IRQn);                                         /* ��UART�ж�                 */
}

/*********************************************************************************************************
** Function name:       UART0_IRQHandler
** Descriptions:        UART0�жϷ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

#if 0//wdq debug
void UART0_IRQHandler (void)
{
    uint32_t delay = 0;
    if (LPC_USART0->STAT & 0x01) {                                      /* �����ж�                     */
        GulNum = 0;
        GucRcvNew = 1;                                                  /* ���յ����ݱ�־��1            */
        Recv_Buff[GulNum++] = LPC_USART0->RXDATA;                       /* ��������                     */
        while(delay < 3000) {
            delay = 0;
            while( (LPC_USART0->STAT & 0x01) == 0x00 ) {         
                delay++;
                if(delay>5000) {
                    break;
                }
            }
            if(delay>5000) {
                break;
            }
            GulNum =GulNum > Maxlength?0:GulNum; 
            Recv_Buff[GulNum++] = LPC_USART0->RXDATA;    
        }
    }
    if(LPC_USART0->STAT & 0x04)   {                                     /* �����ж�                     */
        LPC_USART0->INTENCLR |=  (1 << 2);                              /* ���������ݣ��رշ����ж�     */
    }
}
#endif
/*********************************************************************************************************
** Function name:       uartSendByte
** Descriptions:        �����ֽ�����
** input parameters:    ucDat: ��Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartSendByte (uint8_t ucData)
{
    while ((LPC_USART0->STAT & 0x04) == 0);                             /* �ȴ���������׼����           */
    LPC_USART0->TXDATA = ucData;                                        /* ��������                     */
    LPC_USART0->INTENSET |= (1 << 2);                                   /* �򿪷����жϣ���������       */
}

/*********************************************************************************************************
** Function name:       uartSendHex
** Descriptions:        ��ʮ�������������ַ���ʽ���
** input parameters:    pucStr:ʮ������������
**                      ulNum: ����ĸ���
** output parameters:   
** Returned value:      
*********************************************************************************************************/
void uartSendHex (uint8_t *pucStr,uint8_t num)
{
    uint8_t hChar,lChar;
    uint8_t *pChar;
    pChar = pucStr;
    while(num--)                 
    {
        hChar = *(pChar)>>4;
        lChar = *(pChar++) & 0x0f;
        if( hChar < 0x0a )  {
            hChar += '0';
        }
        else {
            hChar = hChar-10+'A';
        }
        if( lChar < 0x0a )  {
            lChar += '0';
        }
        else {
            lChar = lChar-10+'A';
        }
        uartSendByte(hChar);
        uartSendByte(lChar);
    }
}

/*********************************************************************************************************
** Function name:       uartSendStr
** Descriptions:        
** input parameters:    pucStr:  ����ָ��
**                      ulNum:   ���ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    while (ulNum--) {
        uartSendByte(*pucStr++);
    }
}

/*********************************************************************************************************
** Function name:       uartPrintf()
** Descriptions:        uartPrintf
** input parameters:    ��Ҫ��ӡ���ַ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartPrintf(char *str,...)
{
    char buf[128];
    uint8_t num = 0;
    va_list ptr;
    va_start(ptr,str);
    vsprintf(buf,str,ptr);
    while (buf[num]) {
        uartSendByte(buf[num]);
        num++;
    }
}




































