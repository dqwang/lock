#include "main.h"

extern rfac_u gw_addr_channel;
extern timer_t open_door_timer;
iccard_t g_ic;
errno_t ic_spi_is_ok(void)
{
	if (MCU_TO_PCD_TEST() == TRUE){
		return EOK;
	}else{
		return E_ICCARD_SPI_ERROR;
	}
}

void test_ic_spi_is_ok(void)
{
	if (ic_spi_is_ok() == EOK){
		hwapi01_beep_cnt(1, 100);
		delay_ms(1000);
	}else{
		hwapi01_beep_cnt(3, 100);
		delay_ms(1000);
	}
}

void ic_check_spi(void)
{
	if (ic_spi_is_ok() != EOK){
		hwapi01_beep_cnt(3, 100);
	}
}


void iccard_init(void)
{
    FM175X_SoftReset( );                                                /* FM175xx软件复位              */
    Set_Rf( 3 );                                                        /* 打开双天线                   */
    Pcd_ConfigISOType( 0 );                                             /* ISO14443寄存器初始化         */  
	//FM175X_SoftPowerdown();
}

void test_iccard_read_id(void)
{
	uint8_t statues = TRUE;
    uint8_t picc_atqa[2];                                               /* 储存寻卡返回卡片类型信息     */
    static uint8_t picc_uid[15];                                        /* 储存卡片UID信息              */
    uint8_t picc_sak[3];                                                /* 储存卡片应答信息             */
	u8 ret = FALSE;
	u8 buf[16]={0};
	u8 wbuf[16]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	u8 mifare_keya[KEYA_SIZE]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

	
    statues = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );      /* 激活卡片                     */
    if ( statues == TRUE ) {
        ret = Mifare_Auth(AUTH_MODE_KEYA, 1, mifare_keya, picc_uid);
		if (ret == TRUE){			

			ret = Mifare_Blockwrite(4, wbuf);
			
			ret = Mifare_Blockread(4,buf);
			if (ret == TRUE){
				hwapi01_beep_cnt(2,100);
				debug_log(buf, sizeof(buf));	
			}
			
		}
		memset(picc_uid,0x00,15);
		TypeA_Halt(0);
    }

	delay_ms(1000);
}


errno_t get_iccard_data(iccard_t *ic)
{
    uint8_t picc_atqa[2];                                               /* 储存寻卡返回卡片类型信息     */
    static uint8_t picc_uid[15];                                        /* 储存卡片UID信息              */
    uint8_t picc_sak[3];                                                /* 储存卡片应答信息             */
	u8 ret = FALSE;
	u8 mifare_keya[KEYA_SIZE]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
	
	ret = TypeA_CardActive( picc_atqa,picc_uid,picc_sak );
	if (ret == FALSE){
		goto ERROR;
	}

	ret = Mifare_Auth(AUTH_MODE_KEYA, SECTOR0, mifare_keya, picc_uid);
	if (ret == FALSE){
		goto ERROR;
	}

	ret = Mifare_Blockread(BLOCK0, g_ic.b0.b0_buf);
	if (ret == FALSE){
		goto ERROR;
	}
	ret = Mifare_Blockread(BLOCK1, g_ic.block1);
	if (ret == FALSE){
		goto ERROR;
	}
	ret = Mifare_Blockread(BLOCK2, g_ic.block2);
	if (ret == FALSE){
		goto ERROR;
	}
	
	ret = Mifare_Auth(AUTH_MODE_KEYA, SECTOR1, mifare_keya, picc_uid);
	if (ret == FALSE){
		goto ERROR;
	}
	
	ret = Mifare_Blockread(BLOCK4, g_ic.block4);
	if (ret == FALSE){
		goto ERROR;
	}
	ret = Mifare_Blockread(BLOCK5, g_ic.block5);
	if (ret == FALSE){
		goto ERROR;
	}
	ret = Mifare_Blockread(BLOCK6, g_ic.block6);
	if (ret == FALSE){
		goto ERROR;
	}
	
	TypeA_Halt(0);
	return EOK;
ERROR:	
	//hwapi01_beep_cnt(3,100);
	return E_ICCARD_READ_ERROR;
}

void test_get_iccard_data(void)
{
	get_iccard_data(&g_ic);
	debug_log(g_ic.block4, 16);
	memset(&g_ic, 0, sizeof(iccard_t));
	delay_ms(1000);
}

void iccard_thread(void)
{
	
	if (open_door_timer.flag == 0 && get_iccard_data(&g_ic) == EOK){

		if (SH_GET_4_BYTE(g_ic.b0.b0_buf) == 0xdc212956 ||SH_GET_4_BYTE(g_ic.b0.b0_buf) == 0x7ce42056){
			open_door();
			cmd14_CMD_REPORT_OPENDOOR_ICCARD(&g_ic);
			delay_ms(10);//sendto server time
		}		
	}
}

