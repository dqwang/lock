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
** Descriptions:        ����LPC824����ͷ�ļ�
**
**--------------------------------------------------------------------------------------------------------
** Created By:           linshan wen 
** Created date:         2016-01-08
** Version:              1.0
** Descriptions:         First version
**
**--------------------------------------------------------------------------------------------------------
*/

#ifndef __LPC8xx_UART_H
#define __LPC8xx_UART_H
#include "Stdint.h"

//���ڷ��ͺͽ�������֡��ʽ����
#define    Frame_End      0x03
#define    Maxlength      100

//֡��ʽ����
#define    DataLength     0                                           //����֡�ĳ���
#define    Data            1                                           //����֡������ʼ
#define    Cammand         0                                           //������
  

extern volatile   uint8_t          GucRcvNew;                                  /* ���ڽ��յ����ݱ�־        */
extern uint8_t     Send_Buff[Maxlength];                                       //���ݷ��ͻ���
extern uint8_t     Recv_Buff[Maxlength];                                       //���ݽ��ջ�����         */
extern uint8_t     GulNum;                                                      /* ���յ����ݸ���           */
extern uint32_t    TotalNum;                                                    /* ���յ������ܸ���           */

void uartInit (void);
void uartSendByte (uint8_t ucDat);
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum);

#endif

/*********************************************************************************************************
** Function name:       UARTInit
** Descriptions:        UART��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void UARTInit (void);
/*********************************************************************************************************

*********************************************************************************************************/
extern void uartSendByte (uint8_t ucDat);

extern void uartSendByteS (uint8_t *ucData,uint8_t num);
/*********************************************************************************************************

*********************************************************************************************************/
extern void uartSendStr(uint8_t const *pucStr, uint32_t ulNum);
extern void uartSendHex (uint8_t *pucStr,uint8_t num);
extern void uartPrintf(char *str,...);









