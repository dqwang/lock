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
** File name:           test.c
** Last modified Date:  2016-3-4
** Last Version:        V1.00
** Descriptions:        ������ʾ��������
**
**--------------------------------------------------------------------------------------------------------
*/
#include "fm175xx.h"
#include "type_a.h"
#include "type_b.h"
#include "Utility.h"
#include "mifare_card.h"
#include "LPC8xx_Uart.h"
#include "string.h"
#include "LPC8xx_IO.h"
#include "LPC8xx_Uart.h"



/*********************************************************************************************************
** Function name:       UartSendUid
** Descriptions:        ʹ�ô��ڽ�UID���ͳ���
** input parameters:    picc_atqa  ��Ƭ������Ϣ
**                      picc_uid   ��ƬUID��Ϣ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void UartSendUid( uint8_t *picc_atqa,uint8_t *picc_uid )
{
    if( ( picc_atqa[0] & 0xc0 ) == 0x00 )   {                           /* 4���ֽ�UID                   */
        uartSendHex(picc_uid ,4);
    }
    if( ( picc_atqa[0] & 0xc0 ) == 0x40 )   {                           /* 7�ֽ�UID                     */
        uartSendHex(picc_uid+1 ,3);
        uartSendHex(picc_uid+5 ,4);
    }
    if( ( picc_atqa[0] & 0xc0 ) == 0x80 )   {
        uartSendHex(picc_uid+1 ,3);
        uartSendHex(picc_uid+5 ,3);
        uartSendHex(picc_uid+10 ,4);
    }
}

/*********************************************************************************************************
** Function name:       TyteA_Test()
** Descriptions:        ISO14443AЭ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void TyteA_Test ()
{
    uint8_t statues = TRUE;
    uint8_t num=0;
    uint8_t picc_atqa[2];                                               /* ����Ѱ�����ؿ�Ƭ������Ϣ     */
    static uint8_t picc_uid[15];                                        /* ���濨ƬUID��Ϣ              */
    uint8_t picc_sak[3];                                                /* ���濨ƬӦ����Ϣ             */
    FM175X_SoftReset( );                                                /* FM175xx������λ              */
    Set_Rf( 3 );                                                        /* ��˫����                   */
    Pcd_ConfigISOType( 0 );                                             /* ISO14443�Ĵ�����ʼ��         */
    while(num <2 ) {
        statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );      /* ���Ƭ                     */
        if ( statues == TRUE ) {
            num = 0;
            TypeA_Halt(0);                                              /* ˯�߿�Ƭ                     */
            LED_RedOn();
            uartPrintf("CardUID:0x");
            UartSendUid(picc_atqa,picc_uid);    
            uartPrintf("\r\n");     
            memset(picc_uid,0x00,15); 
        }
        else {
            num++;
        }                    
    }
    Delay100us(1000);
    LED_RedOff();
}


/*********************************************************************************************************
** Function name:       TyteB_Test()
** Descriptions:        ISO14443BЭ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void TyteB_Test ()
{
    uint8_t statues;
    uint32_t rec_len;
    uint8_t pupi[4];
    uint8_t buff[12];
    FM175X_SoftReset( );                                                /* FM175xx������λ              */
    Pcd_ConfigISOType( 1 );                                             /* ISO14443�Ĵ�����ʼ��         */
    Set_Rf( 3 );                                                        /* ��˫����                   */ 
    statues = TypeB_WUP(&rec_len,buff,pupi);                            /* Ѱ��                         */
    if ( statues == TRUE ) {
        statues = TypeB_Select(pupi,&rec_len,buff); 
    }
    if ( statues == TRUE ) {
        LED_RedOn();
        statues = TypeB_GetUID(&rec_len,&buff[0]);
        if(statues == TRUE) {
            uartPrintf("CardUID:0x");                                   /* ���UID����                  */
            uartSendHex( buff, 8 );
            uartPrintf("\r\n");
        }
        Delay100us(1000);
        LED_RedOff();    
    }
    Set_Rf( 0 );                                                        /* �ر�����                     */    
}

/*********************************************************************************************************
** Function name:       MifareCard_Test()
** Descriptions:        mifareCard����
** input parameters:    
**                    0x01�� Demo��᷵�ظ����̵İ汾�������Ϣ
**                    0x02�� Demo��ִ�м���TypeA����������ʱ�п�Ƭ�����߸������ͨ�����ڴ�ӡ��Ƭ��ID��
**                    0x03�� TypeA��Ƭ��Կ��֤����Ƭ����������ͨ����֤����򿪿�Ƭ�Ķ�дȨ��
**                    0x04�� ��TypeA��Ƭ01�����ݣ���ȡ�ɹ����ͨ�����ڴ�ӡ������16�ֽ�����(16���Ƹ�ʽ)
**                    0x05�� ��TypeA��Ƭ01��д��16�ֽ����ݣ�д�����BlockData�����е����ݣ����ҷ���ִ�н��
**                    0x06�� ��TypeB�����Demo���յ���������󣬻�Ѱ��TypeB���͵Ŀ����ȡ��
**                           ��ID�Ų�ͨ�����ڷ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void MifareCard_Test (uint8_t command)
{
    uint8_t statues;
    uint8_t picc_atqa[2];                                               /* ����Ѱ�����ؿ�Ƭ������Ϣ     */
    static uint8_t picc_uid[15];                                        /* ���濨ƬUID��Ϣ              */
    uint8_t picc_sak[3];                                                /* ���濨ƬӦ����Ϣ             */
    uint8_t key[6]= {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};                    /* Ĭ������                     */
    uint8_t blockAddr;
    uint8_t blockData[16];
    uint8_t i;
    uint32_t rec_len;
    uint8_t pupi[4];
    uint8_t buff[12];
    switch (command)
    {
        case 1:                                                         /* Demo��ϵͳ��Ϣ               */
            uartPrintf("FM17550Demo V1.0 2016/03/16.\r\nSupport ISO14443A/Mifare/B.\r\nSupport ISO18092/NFC\r\n");
            break;    
        case 2:                                                         /* ���Ƭ                     */
            Set_Rf( 3 );                                                /* ��˫����                   */
            Pcd_ConfigISOType( 0 );                                     /* ISO14443�Ĵ�����ʼ��         */
            statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );  /* ���Ƭ                     */
            if ( statues == TRUE ) {
                uartPrintf("CardUID:0x");
                UartSendUid(picc_atqa,picc_uid);    
                uartPrintf("\r\n");
            }
            else {
                uartPrintf("No Card!\r\n");
            }
            break;
        case 3:                                                         /* У����Կ                     */
            if(GulNum == 1) {
                blockAddr = 0x00;                                       /* ���ַ  Ĭ��Ϊ0����          */
            }
            else if(GulNum == 2)  {                                     /* �ڶ��ֽ�Ϊ���ַ             */
                blockAddr = Recv_Buff[1];      
                memset(key,0xff,6);                                     /* Ĭ����Կ6��0xff              */
            }
            else if(GulNum >= 8) {                                      /* ��ԿΪ��3-8�ֽ�              */
                blockAddr = Recv_Buff[1];
                memcpy(key,&Recv_Buff[2],6);
            }
            else
            {
            }
            statues = Mifare_Auth(0x00,blockAddr>>2,key,picc_uid);      /* У������                     */
            if ( statues == TRUE ) {
                uartPrintf("Auth Ok!\r\n");
            }
            else {
                uartPrintf("Auth error!\r\n");
            }
            break;
        case 4:                                                         /* ��������                     */
            if(GulNum == 1) {
                blockAddr = 0x01;                                       /* ��1������                    */
            }
            else if(GulNum == 2)   {                                    /* ���ݿ��ַ                   */ 
                blockAddr = Recv_Buff[1];
            }
            else
            {
            }
            statues = Mifare_Blockread(blockAddr,blockData);            /* �������ݺ���                 */
            if ( statues == TRUE )  {                                   /* ���سɹ����������           */                   
                uartPrintf("Read  %d block:0x",blockAddr);
                uartSendHex(blockData,16);
                uartPrintf("\r\n");
            }
            else  {                                                     /* ����ʧ��                     */
                uartPrintf("Read  error!\r\n");
            }
            break;
        case 5:                                                         /* д���ݿ���Ϣ                 */
            if(GulNum == 1) {
                blockAddr = 0x01;                                       /* Ĭ����1���ַ                */
            }
            else if(GulNum == 2) {                                      /* ���ڵڶ��ֽ�Ϊ���ַ         */
            blockAddr = Recv_Buff[1];
            for(i=0;i<16;i++)                                           /* Ĭ��д������Ϊ00-0x0f        */
                blockData[i] = i;
            }
            else if(GulNum >= 8)  {                                  
                blockAddr = Recv_Buff[1];
                memcpy(blockData,&Recv_Buff[2],16);                     /* ������������ֽڵ�ʮ���ֽ�   */
            }
            else
            {
            }
            statues = Mifare_Blockwrite(blockAddr,blockData);
             if ( statues == TRUE )  {                                  /* �����Ϣ                    */
                 uartPrintf("Write %d block:0x",blockAddr);
                 uartSendHex(blockData,16);
                 uartPrintf("\r\n");
            }
            else {
                  uartPrintf("Write error!\r\n");
            }
            break;
        case 6:
            statues = TypeA_Halt(Recv_Buff[1]);
            uartPrintf("Halt command over.\r\n");
            break;
        case 7:
            FM175X_SoftReset( );                                        /* FM175xx������λ              */
            Pcd_ConfigISOType( 1 );                                     /* ISO14443�Ĵ�����ʼ��         */
            Set_Rf( 3 );                                                /* ��˫����                   */ 
            statues = TypeB_WUP(&rec_len,buff,pupi);                    /* Ѱ��                         */
            if ( statues == TRUE ) {
                statues = TypeB_Select(pupi,&rec_len,buff); 
            }
            if ( statues == TRUE ) {
                statues = TypeB_GetUID(&rec_len,&buff[0]);              /* ��ȡ����֤����               */
            }
            if(statues == TRUE) {
                uartPrintf("CardUID:0x");
                uartSendHex( buff, 8 );
                 uartPrintf("\r\n");
             }
             else {
                uartPrintf("No card!\r\n");
            }
            break;
        default:
            uartPrintf("Input error!\r\n");
            break;
        }
}
