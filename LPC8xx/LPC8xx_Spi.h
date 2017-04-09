/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:       LPC8xx_Spi.h
** Descriptions:    SPI������
**
**------------------------------------------------------------------------------------------------------
** Created by:      ������
** Created date:    2016-01-06
** Version:         1.0
** Descriptions:    The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:     
** Modified date:   
** Version:         
** Descriptions:    
********************************************************************************************************/

#ifndef __LPC8XX_SPI_H
#define __LPC8XX_SPI_H



#include <stdint.h>


/*********************************************************************************************************
** �������ƣ�SPIInit
** ������������ʼ��MCU��FM175xx����оƬ�Ŀ��ƺ�ͨѶ�ӿ�
** �����������
** �����������
** �� �� ֵ����
*********************************************************************************************************/
extern  void  SPI_Init(void);

/*********************************************************************************************************
** Function name:       spi_SetReg
** Descriptions:        SPIд����оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
**                      ucRegVal��Ҫд���ֵ
** output parameters:   ��
** Returned value:      TRUE
*********************************************************************************************************/
extern uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);


/*********************************************************************************************************
** Function name:       spi_GetReg
** Descriptions:        SPI������оƬ�Ĵ�������
** input parameters:    ucRegAddr���Ĵ�����ַ
** output parameters:   ��
** Returned value:      Ŀ��Ĵ�����ֵ
*********************************************************************************************************/
extern uint8_t spi_GetReg(uint8_t ucRegAddr);
/*********************************************************************************************************
** Function name:       SPIRead_Sequence
** Descriptions:        SPI��FIFO�Ĵ�����ֵ
** input parameters:    sequence_length ���ݳ��� ucRegAddr���Ĵ�����ַ  *reg_value ����ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void SPIRead_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);


/*********************************************************************************************************
** Function name:       SPIWrite_Sequence
** Descriptions:        SPIдFIFO��ֵ
** input parameters:    sequence_length ���ݳ��� ucRegAddr���Ĵ�����ַ  *reg_value ����ָ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void SPIWrite_Sequence(unsigned char sequence_length,unsigned char ucRegAddr,unsigned char *reg_value);


#endif             // __LPC8XX_IO_H





































