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
** File name:           main.c
** Last modified Date:  2016-01-04
** Last Version:        V1.00
** Descriptions:        The LPC824 IO function example template
**
**--------------------------------------------------------------------------------------------------------
** Created by:          LinShanwen
** Created date:        2016-01-04
** Version:             V1.00
** Descriptions:        定义LPC824GPIO相关操作，用于控制FM175xx读卡芯片。
*/
#ifndef __LPC8XX_IO_H
#define __LPC8XX_IO_H

#include "LPC8xx.h"

#ifdef __cplusplus
    extern "C" {
#endif

/*
** IO引脚定义
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
** CON引脚配置，配置为UART命令模式
*/
//#define CON_Input()         ( LPC_GPIO_PORT->DIR[0] &= ~CON )           /* 配置CON为输入                */


//#define CON_IO            ( LPC_GPIO_PORT->PIN[0] &= CON  )


/*
** 读卡芯片中断引脚NINT操作
*/

#define NINT_Cfg()            ( LPC_GPIO_PORT->DIR[0] &= ~LED_RED )         /* 配置NINT为输入               */

/*
** 读卡芯片TPD复位操作
*/
#define CD_CfgTPD()            ( LPC_GPIO_PORT->DIR[0] |=  FM175xx_RST )       /* 设置TPD为输出    */
#define CD_SetTPD()            ( LPC_GPIO_PORT->PIN[0] |=  FM175xx_RST )       /* 拉高TPD                      */
#define CD_ClrTPD()            ( LPC_GPIO_PORT->PIN[0] &= ~FM175xx_RST )       /* 拉低TPD                      */


#define CD_CfgISP()            ( LPC_GPIO_PORT->DIR[0] &= ~ISP )       /* 设置TPD为输出    */
#define CD_GetISP()            ( LPC_GPIO_PORT->CLR[0] & ISP)                      /* 拉高TPD                      */

#define CD_CfgNSS()     ( LPC_GPIO_PORT->DIR[0] |=  SPI_CS ) 
#define CD_EnNSS()      ( LPC_GPIO_PORT->PIN[0] &= ~SPI_CS ) 
#define CD_DisNSS()     ( LPC_GPIO_PORT->PIN[0] |=  SPI_CS ) 


// ======================================================================================================
#ifdef __cplusplus
    }
#endif

#endif                // __LPC8XX_IO_H


