/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           IAP.c
** Last modified Date:  2012-08-22
** Last Version:        V1.00
** Descriptions:        The IAP.c File
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
#include "IAP.h"


/* 
 *  ����IAP������
 */
#define     IAP_Prepare                                 50              /* ѡ������                     */
                                                                        /* ����ʼ�����š����������š�   */
#define     IAP_RAMTOFLASH                              51              /* �������� FLASHĿ���ַ       */
                                                                        /* ��Ŀ��Flash��ַ��RAMԴ��ַ�� */
                                                                        /* д���ֽ�����ϵͳʱ��Ƶ�ʡ�   */
#define     IAP_ERASESECTOR                             52              /* ��������                     */
                                                                        /* ����ʼ�����š����������š�   */
                                                                        /* ϵͳʱ��Ƶ�ʡ�               */
#define     IAP_BLANKCHK                                53              /* �������                     */
                                                                        /* ����ʼ�����š����������š�   */
#define     IAP_READPARTID                              54              /* ������ID    ���ޡ�           */
#define     IAP_BOOTCODEID                              55              /* ��Boot�汾�š��ޡ�           */
#define     IAP_COMPARE                                 56              /* �Ƚ�����                     */
                                                                        /* ��Flash��ʼ��ַ��RAM��ʼ��ַ */
                                                                        /* ��Ҫ�Ƚϵ��ֽ���             */
#define     IAP_REINVOKEISP                             57              /* ���µ���ISP ���ޡ�           */
#define     IAP_READUID                                 58              /* ��UID       ���ޡ�           */
#define     IAP_ERASEPAGE                               59              /* ����ҳ                       */
                                                                        /* ����ʼҳ�š�����ҳ�š�       */


uint32_t  __Guiparamin[5];                                              /* IAP��ڲ���������            */
uint32_t  __Guiparamout[5];                                             /* IAP���ڲ���������            */

typedef void (*IAP)(uint32_t [], uint32_t []);
IAP __GIAP_Entry = (IAP)IAP_ENTER_ADR;

/*********************************************************************************************************
** Function name:       sectorPrepare
** Descriptions:        IAP��������ѡ���������50
** input parameters:    IAP_Prepare:    ѡ������������
**                      sec1:           ��ʼ����
**                      sec2:           ��ֹ����
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t sectorPrepare (uint8_t  sec1, uint8_t  sec2)
{
    __Guiparamin[0] = IAP_Prepare;                                      /* ����������                   */
    __Guiparamin[1] = sec1;                                             /* ���ò���                     */
    __Guiparamin[2] = sec2;                            
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       ramCopy
** Descriptions:        ����RAM�����ݵ�FLASH���������51
** input parameters:    IAP_RAMTOFLASH: ����RAM���ݵ�Flash������
**                      dst:            Ŀ���ַ����FLASH��ʼ��ַ����512�ֽ�Ϊ�ֽ�
**                      src:            Դ��ַ����RAM��ַ����ַ�����ֶ���
**                      no:             �����ֽڸ�����Ϊ64/128/256/512/1024
**                      IAP_FCCLK��     ϵͳʱ��Ƶ��
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t ramCopy (uint32_t  dst, uint32_t  src, uint32_t  no)
{
    __Guiparamin[0] = IAP_RAMTOFLASH;                                   /* ����������                   */
    __Guiparamin[1] = dst;                                              /* ���ò���                     */
    __Guiparamin[2] = src;
    __Guiparamin[3] = no;
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       sectorErase
** Descriptions:        �����������������52
** input parameters:    IAP_ERASESECTOR����������������
**                      sec1��          ��ʼ����
**                      sec2��          ��ֹ����
**                      IAP_FCCLK��     ϵͳʱ��Ƶ��
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t sectorErase (uint8_t  sec1, uint8_t  sec2)
{
    __Guiparamin[0] = IAP_ERASESECTOR;                                  /* ����������                   */
    __Guiparamin[1] = sec1;                                             /* ���ò���                     */
    __Guiparamin[2] = sec2;
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       blankChk
** Descriptions:        ������գ��������53
** input parameters:    IAP_BLANKCHK��  �������������
                        sec1:           ��ʼ����
**                      sec2:           ��ֹ����92
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t blankChk (uint8_t  sec1, uint8_t  sec2)
{
    __Guiparamin[0] = IAP_BLANKCHK;                                     /* ����������                   */
    __Guiparamin[1] = sec1;                                             /* ���ò���                     */
    __Guiparamin[2] = sec2;
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       parIdRead
** Descriptions:        ������ID���������54
** input parameters:    IAP_READPARTID��������ID������
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t partIdRead (void)
{
    __Guiparamin[0] = IAP_READPARTID;                                   /* ����������                   */
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       codeIdBoot
** Descriptions:        ������ID���������55
** input parameters:    IAP_BOOTCODEID��������ID������
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t codeIdBoot (void)
{
    __Guiparamin[0] = IAP_BOOTCODEID;                                   /* ����������                   */
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       dataCompare
** Descriptions:        У�����ݣ��������56
** input parameters:    IAP_COMPARE��   ���ݱȽ�������
                        dst:            Ŀ���ַ����RAM/FLASH��ʼ��ַ����ַ�����ֶ���
**                      src:            Դ��ַ����FLASH/RAM��ַ����ַ�����ֶ���
**                      no:             �Ƚ��ֽڸ����������ܱ�4����
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t dataCompare (uint32_t  dst, uint32_t  src, uint32_t  no)
{
    __Guiparamin[0] = IAP_COMPARE;                                      /* ����������                   */
    __Guiparamin[1] = dst;                                              /* ���ò���                     */
    __Guiparamin[2] = src;
    __Guiparamin[3] = no;
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
** Function name:       ReinvokeISP
** Descriptions:        ���µ���ISP���������57
** input parameters:    IAP_REINVOKEISP�����µ���ISP������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ReinvokeISP (void)
{
    __Guiparamin[0] = IAP_REINVOKEISP;                                  /* ����������                   */
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
}

/*********************************************************************************************************
** Function name:       uIdRead
** Descriptions:        ��UID���������58
** input parameters:    IAP_READUID:    ��UID������
** output parameters:   __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      __Guiparamout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t uIdRead (void)
{
    __Guiparamin[0] = IAP_READUID;                                      /* ����������                   */
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */

    return __Guiparamout[0];
}

/*********************************************************************************************************
** Function name:       pageErase
** Descriptions:        ҳ�������������59
** input parameters:    IAP_READUID:    ��UID������
** output parameters:   paramout[0]:    IAP����״̬��,IAP����ֵ
** Returned value:      paramout[0]:    IAP����״̬��,IAP����ֵ
*********************************************************************************************************/
uint32_t pageErase (uint8_t  page1, uint8_t  page2)
{
    __Guiparamin[0] = IAP_ERASEPAGE;                                    /* ����������                   */
    __Guiparamin[1] = page1;                                            /* ���ò���                     */
    __Guiparamin[2] = page2;
    
    (*__GIAP_Entry)(__Guiparamin, __Guiparamout);                       /* ����IAP�������              */
    
    return (__Guiparamout[0]);                                          /* ����״̬��                   */
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
