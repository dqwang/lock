/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           IAP.h
** Last modified Date:  2012-08-22
** Last Version:        V1.00
** Descriptions:        The IAP Head File
**
**--------------------------------------------------------------------------------------------------------
** Created by:          CaoHua
** Created date:        2012-08-22
** Version:             V1.00
** Descriptions:        Creat The File
**
**--------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
**
** Rechecked by:        
*********************************************************************************************************/
#ifndef __IAP_H__                                                       /*  __IAP_H__                   */
#define __IAP_H__                                                       /*  __IAP_H__                   */

#ifdef  __cplusplus
    extern "C" {
#endif

#include "stdint.h"
/* 
 *  ����CCLKֵ��С����λΪKHz 
 */
#define     IAP_FCCLK                                   (24000)
#define     IAP_ENTER_ADR                               0x1FFF1FF1      /* IAP��ڵ�ַ����              */
/*
 *  ����IAP����״̬��
 */
#define     CMD_SUCCESS                                 0               /* �ɹ�ִ������                 */
#define     INVALID_COMMAND                             1               /* ��Ч����                     */
#define     SRC_ADDR_ERROR                              2               /* Դ��ַ��������Ϊ�߽�         */
#define     DST_ADDR_ERROR                              3               /* Ŀ���ַ����                 */
#define     SRC_ADDR_NOT_MAPPED                         4               /* Դ��ַ���ڴ洢��ӳ����       */
#define     DST_ADDR_NOT_MAPPED                         5               /* Ŀ���ַ���ڴ洢��ӳ����     */
#define     COUNT_ERROR                                 6               /* �ֽڼ���������4�ı������߷Ƿ�*/
#define     INVALID_SECTOR                              7               /* ��������Ч                   */
#define     SECTOR_NOT_BLANK                            8               /* �����ǿ�                     */
#define     SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION     9               /* Ϊд����׼������δִ��       */
#define     COMPARE_ERROR                               10              /* Դ��Ŀ�����ݲ���ͬ           */
#define     BUSY                                        11              /* Flash���Ӳ���ӿ�æ          */

/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP��������ѡ���������50
** input parameters:    IAP_Prepare:    ѡ������������
**                      sec1:           ��ʼ����
**                      sec2:           ��ֹ����
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t sectorPrepare (uint8_t  sec1, uint8_t  sec2);

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        ����RAM�����ݵ�FLASH���������51
** input parameters:    IAP_RAMTOFLASH: ����RAM���ݵ�Flash������
**                      dst:            Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**                      src:            Դ��ַ����RAM��ַ����ַ�����ֶ���
**                      no:             �����ֽڸ�����Ϊ512/1024/4096/8192
**                      IAP_FCCLK��     ϵͳʱ��Ƶ��
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t ramCopy (uint32_t  dst, uint32_t  src, uint32_t  no);

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        �����������������52
** input parameters:    IAP_ERASESECTOR����������������
**                      sec1��          ��ʼ����
**                      sec2��          ��ֹ����
**                      IAP_FCCLK��     ϵͳʱ��Ƶ��
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t sectorErase (uint8_t  sec1, uint8_t  sec2);

/*********************************************************************************************************
** Function name:       blankChk
** Descriptions:        ������գ��������53
** input parameters:    IAP_BLANKCHK��  �������������
                        sec1:           ��ʼ����
**                      sec2:           ��ֹ����92
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t blankChk (uint8_t  sec1, uint8_t  sec2);

/*********************************************************************************************************
** Function name:       parIdRead
** Descriptions:        ������ID���������54
** input parameters:    IAP_READPARTID��������ID������
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t partIdRead (void);

/*********************************************************************************************************
** Function name:       codeIdBoot
** Descriptions:        ������ID���������55
** input parameters:    IAP_BOOTCODEID��������ID������
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t codeIdBoot (void);

/*********************************************************************************************************
** Function name:       dataCompare
** Descriptions:        У�����ݣ��������56
** input parameters:    IAP_COMPARE��   ���ݱȽ�������
                        dst:            Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���
**                      src:            Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
**                      no:             �Ƚ��ֽڸ����������ܱ�4����
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t dataCompare (uint32_t  dst, uint32_t  src, uint32_t  no);

/*********************************************************************************************************
** Function name:       ReinvokeISP
** Descriptions:        ���µ���ISP���������57
** input parameters:    IAP_REINVOKEISP�����µ���ISP������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void ReinvokeISP (void);

/*********************************************************************************************************
** Function name:       uIdRead
** Descriptions:        ��UID���������58
** input parameters:    IAP_READUID:    ��UID������
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t uIdRead (void);

/*********************************************************************************************************
** Function name:       pageErase
** Descriptions:        ҳ�������������59
** input parameters:    IAP_READUID:    ��UID������
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
extern uint32_t pageErase (uint8_t  page1, uint8_t  page2);

#ifdef  __cplusplus
    }
#endif

#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
