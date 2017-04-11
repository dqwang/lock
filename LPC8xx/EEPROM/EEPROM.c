#include "main.h"

#define DisableIRQ()    __disable_irq()
#define EnableIRQ()     __enable_irq()

__align(4) uint8_t ucBuff[1024];


/*********************************************************************************************************
** Function name:       eepromRead
** Descriptions:        ���øú������Զ���EEPROM_STARTSEC(EEPROM��ʼ������ʼ������)
** input parameters:    eeprom_addr:    ����EEPROM�洢�ռ��ƫ������ַ
**                      pucData:        ���ն�ȡ���ݵ�ָ��
**                      uiLen:          ��ȡ���ֽ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint32_t eepromRead (uint32_t uieepromAddr, uint8_t *pucData, uint32_t uiLen)
{
    if ((uieepromAddr + uiLen) >= EEPROM_SIZE) {
        return PARA_SRCADDR_ERROR;
    }
    
    while (uiLen--) {
        *(pucData++) = *((uint8_t *)(EEPROM_STARTADDR + uieepromAddr++));
    }
    
    return EE_SUCCESS;
}

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
uint32_t eepromWriteNByte (uint32_t uieepromAddr, uint8_t *pucData, uint32_t uiLen)
{
    uint8_t  uceepromSec, ucFlashSec;
    uint32_t uiOff = 0, uiChangeMax, uiChangeLen;
    uint32_t uiErr = 0;
    uint32_t uiBuffAddr = (uint32_t)ucBuff;
    
    if ((uieepromAddr + uiLen) >= EEPROM_SIZE) {
        return PARA_SRCADDR_ERROR;
    }
    
    DisableIRQ();                                                       /* �ر��ж�                     */
    uceepromSec = uieepromAddr / 1024;                                  /* EEPROM ��д����ʼ������      */
    uiOff       = uieepromAddr - 1024 * uceepromSec;                    /* ��ʼ�޸ĵ�ƫ����             */
    ucFlashSec  = uceepromSec + EEPROM_STARTSEC;
    uiChangeMax = 1024 * (uceepromSec + 1) - uieepromAddr;              /* �޸ĵ���󳤶�               */
    uiChangeLen = (uiChangeMax > uiLen) ? uiLen : uiChangeMax;          /* �޸ĵĳ���                   */
    
    eepromRead((uint32_t)(uceepromSec << 10), ucBuff, 1024);            /* ��ȡEEPROM����               */
    
    for (; uiOff < uiChangeLen; uiOff++) {
        ucBuff[uiOff] = *pucData++;                                     /* �޸�EEPROM����               */
    }
    /*
     * ��������
     */
    uiErr = sectorPrepare(ucFlashSec, ucFlashSec);
    uiErr = sectorErase(ucFlashSec, ucFlashSec);
    uiErr = blankChk(ucFlashSec, ucFlashSec);
    /*
     * д���޸ĺ������
     */
    uiErr = sectorPrepare(ucFlashSec, ucFlashSec);
    uiErr = ramCopy(ucFlashSec << 10, uiBuffAddr, 1024);
    uiErr = dataCompare(ucFlashSec << 10, uiBuffAddr, 1024);
    
    if (uiErr != CMD_SUCCESS) {
        return IAP_ERROR;
    }
    
    if (uiChangeLen == uiChangeMax) {
        uiLen = uiLen - uiChangeMax;
        while (uiLen >= 1024) {
            ucFlashSec++;
            for (uiOff = 0; uiOff < 1024; uiOff++) {
                ucBuff[uiOff] = *pucData++;                             /* �޸�EEPROM����               */
            }
            /*
             * ��������
             */
            uiErr = sectorPrepare(ucFlashSec, ucFlashSec);
            uiErr = sectorErase(ucFlashSec, ucFlashSec);
            uiErr = blankChk(ucFlashSec, ucFlashSec);
            /*
             * д���޸ĺ������
             */
            uiErr = sectorPrepare(ucFlashSec, ucFlashSec);
            uiErr = ramCopy(ucFlashSec << 10, uiBuffAddr, 1024);
            uiErr = dataCompare(ucFlashSec << 10, uiBuffAddr, 1024);
            
            if (uiErr != CMD_SUCCESS) {
                return IAP_ERROR;
            }
            uiLen -= 1024;
        }
        ucFlashSec++;
        uceepromSec = ucFlashSec - EEPROM_STARTSEC;
        
        eepromRead(uceepromSec << 10, ucBuff, 1024);                    /* ��ȡEEPROM����               */
        for (uiOff = 0; uiOff < uiLen; uiOff++) {
            ucBuff[uiOff] = *pucData++;                                 /* �޸�EEPROM����               */
        }
        /*
         * ��������
         */
        uiErr = sectorPrepare(ucFlashSec, ucFlashSec);
        uiErr = sectorErase(ucFlashSec, ucFlashSec);
        uiErr = blankChk(ucFlashSec, ucFlashSec);
        /*
         * д���޸ĺ������
         */
        uiErr = sectorPrepare(ucFlashSec, ucFlashSec);
        uiErr = ramCopy(ucFlashSec << 10, uiBuffAddr, 1024);
        uiErr = dataCompare(ucFlashSec << 10, uiBuffAddr, 1024);
        
        if (uiErr != CMD_SUCCESS) {
            return IAP_ERROR;
        }
    }
    
    EnableIRQ();                                                        /* ���ж�                     */
    
    return EE_SUCCESS;
}


#define EEPROM_MAX_BYTES_ONCE 256//write and read once , about 110ms
void test_eeprom(void)
{
	u32 uiErr = 0;
    u8  ucSrcData[EEPROM_MAX_BYTES_ONCE];
	u16 i=0;
	
    for (i = 0; i < EEPROM_MAX_BYTES_ONCE; i++) {
        ucSrcData[i] = i;
    }
    
    uiErr = eepromWriteNByte(0, ucSrcData, EEPROM_MAX_BYTES_ONCE);
	//memset(ucSrcData, 0x0, EEPROM_MAX_BYTES_ONCE);
	uiErr = eepromRead(0, ucSrcData, EEPROM_MAX_BYTES_ONCE);

    if (uiErr == EE_SUCCESS) {
        for (i = 0; i < EEPROM_MAX_BYTES_ONCE; i++) {
            if (ucSrcData[i] != i) {
                hwapi01_beep_crtl(ON);
                break;
            }
        }
        if (i == EEPROM_MAX_BYTES_ONCE) {                                                  /* ����У��ɹ���LED��˸һ��    */
			hwapi01_beep_crtl(OFF);//no news is good news
		}
    } else {
        hwapi01_beep_crtl(ON);
    }   

}


/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
