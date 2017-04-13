#ifndef __EEPROM_H__                                                    /*  __EEPROM_H__                */
#define __EEPROM_H__                                                    /*  __EEPROM_H__                */

#ifdef  __cplusplus
    extern "C" {
#endif

#include "stdint.h"

/*
 * ��ΪEEPROM��Flash���ã�4 kB����
 * Sector��     8   ~ 15
 * Page��       128 ~ 255
 * Addr��    0x2000 ~ 0x3FFF
 */
#define EEPROM_STARTSEC             28
#define EEPROM_ENDSEC               31
#define EEPROM_STARTPAGE            436
#define EEPROM_ENDPAGE              499
#define EEPROM_STARTADDR            0x7000
#define EEPROM_ENDADDR              0x7FFF
#define EEPROM_SIZE                 0x1000// 4 KB

/*
 *  ����EEPROM����״̬��
 */
#define EE_SUCCESS                  0                                   /* EEPROMд�����ɹ����         */
#define IAP_ERROR                   1                                   /* �ڲ�IAP�������ó���          */
#define PARA_BANK_ERROR             2                                   /* �����İ�����               */
#define PARA_SRCADDR_ERROR          3                                   /* д����ʱ��Դ��ַ����(z�ֶ���)*/
#define PARA_OVERFLOW_ERROR         4                                   /* д�����                     */


#define EEPROM_ADDR_GATEWAY_RF433M_ADDR_CHANNEL 0// 0-4:5B 

#define EEPROM_ADDR_GATEWAY_RF433M_READ_CONFIG_FLAG 5// 5:1B


#define EEPROM_ADDR_LOCK_RF433M_ADDR_CHANNEL 6// 6-10:5B 

#define EEPROM_ADDR_LOCK_RF433M_READ_CONFIG_FLAG 11// 11:1B



/*********************************************************************************************************
** Function name:       eepromWriteNByte
** Descriptions:        ���øú�������ģ��дEEPROM���������д����256�ֽ�
** input parameters:    uieepromAddr:   EEPROM�洢�ռ��ƫ������ַ
**                      pucData��       ����д�����ݵ�ָ��
**                      uiLen��         д����ֽ���
** output parameters:   ״̬��Ϣ
** Returned value:      ��
ע�⣺���øú�������ʹ����ΪEEPROM��Flashȫ������������ʹǰ��д���������ݶ�ʧ
*********************************************************************************************************/
extern uint32_t eepromWriteNByte(uint32_t uieepromAddr, uint8_t *pucData, uint32_t uiLen);

/*********************************************************************************************************
** Function name:       eepromRead
** Descriptions:        ���øú������Զ���EEPROM_STARTSEC(EEPROM��ʼ������ʼ������)
** input parameters:    eeprom_addr:    ����EEPROM�洢�ռ��ƫ������ַ
**                      pucData:        ���ն�ȡ���ݵ�ָ��
**                      uiLen:          ��ȡ���ֽ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern uint32_t eepromRead(uint32_t uieepromAddr, uint8_t *pucData, uint32_t uiLen);



void test_eeprom(void);


#ifdef  __cplusplus
    }
#endif

#endif

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
