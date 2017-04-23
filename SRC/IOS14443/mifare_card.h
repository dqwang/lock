#ifndef __MIFARE_CARD_H__
#define __MIFARE_CARD_H__

#define AUTH_MODE_KEYA 0x00
#define AUTH_MODE_KEYB 0x01

extern unsigned char Mifare_Transfer(unsigned char block);
extern unsigned char Mifare_Restore(unsigned char block);
extern unsigned char Mifare_Blockset(unsigned char block,unsigned char *buff);
extern unsigned char Mifare_Blockinc(unsigned char block,unsigned char *buff);
extern unsigned char Mifare_Blockdec(unsigned char block,unsigned char *buff);
extern unsigned char Mifare_Blockwrite(unsigned char block,unsigned char *buff);
extern unsigned char Mifare_Blockread(unsigned char block,unsigned char *buff);
extern unsigned char Mifare_Auth(unsigned char mode,unsigned char sector,unsigned char *mifare_key,unsigned char *card_uid);

#endif
