/*********************************************Copyright (c)***********************************************
**                                Guangzhou ZLG MCU Technology Co., Ltd.
**
**                                        http://www.zlgmcu.com
**
**      ������������Ƭ���Ƽ����޹�˾���ṩ�����з�������ּ��Э���ͻ����ٲ�Ʒ���з����ȣ��ڷ�����������ṩ
**  ���κγ����ĵ������Խ����������֧�ֵ����Ϻ���Ϣ���������ο����ͻ���Ȩ��ʹ�û����вο��޸ģ�����˾��
**  �ṩ�κε������ԡ��ɿ��Եȱ�֤�����ڿͻ�ʹ�ù��������κ�ԭ����ɵ��ر�ġ�żȻ�Ļ��ӵ���ʧ������˾��
**  �е��κ����Ρ�
**                                                                        ����������������Ƭ���Ƽ����޹�˾
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               zlg_stdint.h
** Last modified date:      2014-03-12
** Last version:            V1.0
** Description:             ���Ͷ���
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Li Xiang
** Created date:            2014-03-12
** Version:                 V1.0
** Descriptions:            The original version ��ʼ�汾
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#ifndef  __ZLG_STDINT_H
#define  __ZLG_STDINT_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************************************************************************************************
  ͨ�����������ض���
*********************************************************************************************************/
typedef unsigned char       BOOLEAN;                                    /* ��������                     */
typedef unsigned char       INT8U;                                      /* �޷���8λ���ͱ���            */
typedef signed   char       INT8S;                                      /* �з���8λ���ͱ���            */
typedef unsigned short      INT16U;                                     /* �޷���16λ���ͱ���           */
typedef signed   short      INT16S;                                     /* �з���16λ���ͱ���           */
typedef unsigned long       INT32U;                                     /* �޷���32λ���ͱ���           */
typedef signed   long       INT32S;                                     /* �з���32λ���ͱ���           */
typedef unsigned long long  INT64U;                                     /* �޷���64λ���ͱ���           */
typedef signed long long    INT64S;                                     /* �з���64λ���ͱ���           */
typedef float               FP32;                                       /* �����ȸ�������32λ���ȣ�     */
typedef double              FP64;                                       /* ˫���ȸ�������64λ���ȣ�     */

typedef unsigned char       uint8;                                      /* �޷���8λ���ͱ���            */
typedef unsigned short int  uint16;                                     /* �޷���16λ���ͱ���           */
typedef unsigned long int   uint32;                                     /* �޷���32λ���ͱ���           */

typedef char                int8;                                       /* �з���8λ���ͱ���            */
typedef short int           int16;                                      /* �з���16λ���ͱ���           */
typedef int                 int32;                                      /* �з���32λ���ͱ���           */

typedef volatile int8       vint8;                                      /*  8 bits                      */
typedef volatile int16      vint16;                                     /* 16 bits                      */
typedef volatile int32      vint32;                                     /* 32 bits                      */

typedef volatile uint8      vuint8;                                     /*  8 bits                      */
typedef volatile uint16     vuint16;                                    /* 16 bits                      */
typedef volatile uint32     vuint32;                                    /* 32 bits                      */


#ifdef __cplusplus
}
#endif

#endif


/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/
