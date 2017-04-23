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
** File name:           main.c
** Last modified Date:  2016-01-04
** Last Version:        V1.00
** Descriptions:        The LPC824 IO function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          LinShanwen
** Created date:        2016-01-04
** Version:             V1.00
** Descriptions:        ����LPC824GPIO��ز��������ڿ���FM175xx����оƬ��
*/
#ifndef __LPC8XX_IO_H
#define __LPC8XX_IO_H

#include "LPC8xx.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*
** IO���Ŷ���
*/        

#define FM175xx_RST        (1<<15)
#define FM175xx_INT        (1<<24)
//#define UART_TXD           (1<<4)
//#define UART_RXD           (1<<0)
#define ISP                (1<<12)
//#define CON                (1<<13)
#define SPI_MISO           (1<<16)
#define SPI_MOSI           (1<<27)
#define SPI_SCK            (1<<26)
#define SPI_CS             (1<<25)



/*
** CON�������ã�����ΪUART����ģʽ
*/
//#define CON_Input()         ( LPC_GPIO_PORT->DIR[0] &= ~CON )           /* ����CONΪ����                */


//#define CON_IO            ( LPC_GPIO_PORT->PIN[0] &= CON  )


/*
** ����оƬ�ж�����NINT����
*/

#define NINT_Cfg()            ( LPC_GPIO_PORT->DIR[0] &= ~LED_RED )         /* ����NINTΪ����               */

/*
** ����оƬTPD��λ����
*/
#define CD_CfgTPD()            ( LPC_GPIO_PORT->DIR[0] |=  FM175xx_RST )       /* ����TPDΪ���    */
#define CD_SetTPD()            ( LPC_GPIO_PORT->PIN[0] |=  FM175xx_RST )       /* ����TPD                      */
#define CD_ClrTPD()            ( LPC_GPIO_PORT->PIN[0] &= ~FM175xx_RST )       /* ����TPD                      */


#define CD_CfgISP()            ( LPC_GPIO_PORT->DIR[0] &= ~ISP )       /* ����TPDΪ���    */
#define CD_GetISP()            ( LPC_GPIO_PORT->CLR[0] & ISP)                      /* ����TPD                      */

#define CD_CfgNSS()     ( LPC_GPIO_PORT->DIR[0] |=  SPI_CS ) 
#define CD_EnNSS()      ( LPC_GPIO_PORT->PIN[0] &= ~SPI_CS ) 
#define CD_DisNSS()     ( LPC_GPIO_PORT->PIN[0] |=  SPI_CS ) 


// ======================================================================================================
#ifdef __cplusplus
    }
#endif

#endif                // __LPC8XX_IO_H


