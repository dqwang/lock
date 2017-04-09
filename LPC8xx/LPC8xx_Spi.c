/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       LPC8xx_Spi.c
** Descriptions:    SPI驱逗
**
**------------------------------------------------------------------------------------------------------
** Created by:      林善文
** Created date:    2016-01-04
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:   
** Version:         
** Descriptions:    
********************************************************************************************************/
#include   "LPC8xx.h"
#include   "LPC8xx_IO.h"
#include   <stdint.h>

/*
 * 移植本软件包时需要修改以下的函数或宏 
 */
#define     SSEL_EN         (0 << 16)
#define     SSEL_DIS        (1 << 16)
#define     EOT_EN          (1 << 20)
#define     EOT_DIS         (0 << 20)
#define     EOF_EN          (1 << 21)
#define     EOF_DIS         (0 << 21)
#define     RXIGNORE_EN     (1 << 22)
#define     RXIGNORE_DIS    (0 << 22)
#define     FLEN(n)         (((n) - 1) << 24)

#define TRUE  1
#define FALSE 0


/*********************************************************************************************************
** 函数名称：SPIInit
** 功能说明：初始化SPI
** 输入参数：无
** 输出参数：无
*********************************************************************************************************/
void  SPI_Init (void)
{
    LPC_SWM->PINASSIGN[3] &= ~( 0xFFUL << 24 );
    LPC_SWM->PINASSIGN[3] |=  ( 26  << 24 );                            /* SPI0_SCK  ~ P0.26            */ 
    LPC_SWM->PINASSIGN[4] &= ~( 0xFFFFUL <<  0 );
    LPC_SWM->PINASSIGN[4] |=  ( 27 <<  0 );                             /* SPI0_MOSI ~ P0.27            */
    LPC_SWM->PINASSIGN[4] |=  ( 16 <<  8 );                             /* SPI0_MISO ~ P0.16            */
    //LPC_SWM->PINASSIGN[4] |=  ( 24 << 16 );                           /* SPI0_SSEL ~ P0.17            */
    CD_CfgNSS();
    CD_DisNSS();
    LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 11);                            /* 初始化SPI0 AHB时钟           */
    LPC_SYSCON->PRESETCTRL    &= ~(1 << 0);                             /* 复位SPI0                     */
    LPC_SYSCON->PRESETCTRL    |=  (1 << 0);
    
    LPC_SPI0->DIV = SystemCoreClock / 1000000 - 1;                      /* 主机模式下SCK频率分频设置    */
    LPC_SPI0->DLY = (0 << 0) |                                          /* 起始插入延迟：0 * T_SCK      */
                    (0 << 4) |                                          /* 结束插入延迟：0 * T_SCK      */
                    (0 << 8) |                                          /* 帧间插入延迟：0 * T_SCK      */
                    (0 << 12);                                          /* 传输插入延迟：0 * T_SCK      */
    
    /*
     * TXCTRL 发送控制器设置：
     * TXCTRL
     *      16：传输从机选择    0：选择从机     1：取消选择从机     控制写数据前SSEL信号的有效性
     *      20：传输结束控制    0：传输不结束   1：传输结束         控制每次写数据后SSEL信号的有效性
     *      21：帧结束控制      0：帧不结束     1：帧结束           控制下次写数据前是否插入帧间延迟
     *      22：接收忽略控制    0：不忽略接收   1：忽略接收
     *   27:24：帧长度          0x0 ~ 0xF：对应 1 ~ 16 位帧长度
     */
    LPC_SPI0->TXCTRL = (1 << 16) |                                      /* 从机选择                     */
                       (0 << 20) |                                      /* 传输不结束                   */
                       (1 << 21) |                                      /* 帧结束                       */
                       (0 << 22) |                                      /* 不忽略接收                   */
                       (7 << 24);                                       /* 帧长度：8位                  */
    
    /*
     * CFG 配置寄存器设置：
     * CFG
     *      0：SPI使能位        0：SPI禁能      1：SPI使能
     *      2：模式选择位       0：SPI从机      1：SPI主机
     *      3：传输位序选择     0：标准(MSBF)   1：反转(LSBF)
     *      4：时钟相位选择     0：第1个沿捕获  1：第2个沿捕获
     *      5：时钟极性选择     0：帧间SCK为0   1：帧间SCK为1
     *      7：回写模式使能     0：禁止回写     1：使能回写
     *      8：SSEL有效极性选择 0：低电平有效   1：高电平有效
     */
    LPC_SPI0->CFG = (1 << 0) |                                          /* SPI使能                      */
                    (1 << 2) |                                          /* 主机模式                     */
                    (0 << 3) |                                          /* MSBF                         */
                    (0 << 4) |                                          /* CPHA = 0                     */
                    (0 << 5) |                                          /* CPOL = 0                     */
                    (0 << 7) |                                          /* 禁止回写                     */
                    (0 << 8);                                           /* SSEL低电平有效               */
}

/*********************************************************************************************************
** 函数名称: SendRecv_Byte
** 函数功能：一次传输内部的单字节帧发送与接收
** 输入参数: 数据线上发送出的数据
** 输出参数: 数据线上接收到的数据
** 返 回 值：无
*********************************************************************************************************/
uint8_t SPI_SendByte (uint32_t ucData)
{
    while (!(LPC_SPI0->STAT & (1 << 1)));                               /* 等待发送准备就绪             */
    LPC_SPI0->TXDATCTL =  ucData;                                       /* 8 位，帧结束                 */    
    
    while (!(LPC_SPI0->STAT & (1 << 0)));                               /* 等待接收数据完成             */
    ucData = LPC_SPI0->RXDAT;                                           /* 接收数据                     */
    
    return ucData;
}


/*********************************************************************************************************
** Function name:       spi_SetReg
** Descriptions:        SPI写读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
**                      ucRegVal：要写入的值
** output parameters:   无
** Returned value:      TRUE
*********************************************************************************************************/
uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
    CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | (ucRegAddr<<1) );                         /* 8 位，帧结束                 */ 
    SPI_SendByte ( FLEN(8) | EOF_EN | ucRegVal );
    CD_DisNSS ();
    return TRUE;
}

/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI读读卡芯片寄存器函数
** input parameters:    ucRegAddr：寄存器地址
** output parameters:   无
** Returned value:      目标寄存器的值
*********************************************************************************************************/
uint8_t spi_GetReg(uint8_t ucRegAddr)
{
    unsigned char ucRegVal;
    CD_EnNSS ();
    SPI_SendByte ( FLEN(8)  | 0x80|(ucRegAddr<<1) );
    ucRegVal = SPI_SendByte ( FLEN(8) | EOF_EN  | 0x00 );
    CD_DisNSS ();
    return ucRegVal;
}


/*********************************************************************************************************
** Function name:       SPIRead_Sequence
** Descriptions:        SPI读FIFO寄存器的值
** input parameters:    sequence_length 数据长度 ucRegAddr：寄存器地址  *reg_value 数据指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)    
{
    uint8_t i;
    if (sequence_length==0)
    return;
    CD_EnNSS ();
    for(i=0;i<sequence_length;i++) {
        *(reg_value+i) = spi_GetReg(ucRegAddr);
    }
    CD_DisNSS ();
    return;

}

/*********************************************************************************************************
** Function name:       SPIWrite_Sequence
** Descriptions:        SPI写FIFO的值
** input parameters:    sequence_length 数据长度 
**                      ucRegAddr：寄存器地址  
**                      *reg_value 数据指针
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value)
{
    
    uint8_t i;
    if(sequence_length==0)
        return;
    for(i=0;i<sequence_length;i++) {
        spi_SetReg(ucRegAddr, *(reg_value+i));
    }
    CD_DisNSS ();
    return ;    
}




