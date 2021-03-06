#include "main.h"
#include "door.h"

rfac_u gw_addr_channel;
rfac_u lock_addr_channel;


extern uart_t uart0;


extern Queue gq;//gateway queue


/*----------------------------------------------------------------------------*/


void lock_addr_channel_init(void)
{
	//todo 从服务器下发，存在flash中。
	//todo read from the flash

	SH_PUT_4_BYTE(lock_addr_channel.rfac2.addr, 0x00000002);
	lock_addr_channel.rfac1.channel = 0x28;

	//todo
}

void gateway_addr_channel_init(void)
{
	//todo 从服务器下发，存在flash中。
	//todo read from the flash

	SH_PUT_4_BYTE(gw_addr_channel.rfac2.addr, 0x00000001);
	gw_addr_channel.rfac1.channel = 0x11;

	//todo
}


errno_t is_valid_packet_from_gateway(u8 *in_buf)
{
	//header length
	if (0 == memcmp(in_buf, gw_addr_channel.rfac0, sizeof(rfac_u)))//todo, the tail != 0
		//&& !(in_buf[LOCK_PACKET_SIZE -2] ==0 && in_buf[LOCK_PACKET_SIZE -1] ==0))//For new 433m module
	{
		return EOK;
	}else{
		return E_INVALID_PACKET;
	}	
}

errno_t hwapi10_decode_packet_from_gateway(u8 *in_buf, lpkt_u *out_lp)
{
	if (NULL == in_buf || NULL == out_lp){
		return E_NULL_POINTER;
	}

	//decode
	if (EOK == is_valid_packet_from_gateway(in_buf)){
		memcpy(out_lp->lpkt0, in_buf, sizeof(lpkt_u));
		return EOK;
	}else{
		return E_INVALID_PACKET;
	}
}

errno_t hwapi10_echo_packet_to_gateway(lpkt_u *in_lp)
{
	lpkt_u lpkt;

	if (NULL == in_lp){
		return E_NULL_POINTER;
	}

	//encode
	memcpy(lpkt.lpkt0, gw_addr_channel.rfac0, sizeof(rfac_u));
	memcpy(lpkt.lpkt2.lpro.lpro0, in_lp->lpkt2.lpro.lpro0, sizeof(lpro_u));		
	
	//send
	send2gateway(lpkt.lpkt0, sizeof(lpkt_u));
	
	return EOK;
}

void hwapi10_ack_error_to_gateway(errno_t error)
{
	lpkt_u lp;

	CLEAR_TYPE(&lp, lpkt_u);

	memcpy(lp.lpkt2.lock_addr, gw_addr_channel.rfac2.addr, sizeof(RF_ADDR_SIZE));
	lp.lpkt1.lock_channel = gw_addr_channel.rfac1.channel;
	memset(lp.lpkt2.lpro.lpro0, (u8)error, LOCK_PROTOCOL_SIZE);

	send2gateway(lp.lpkt0, LOCK_PACKET_SIZE);
}


void test_hwapi10_ack_error_to_gateway(void)
{
	hwapi10_ack_error_to_gateway(E_INVALID_CMD);
	delay_ms(1000);
}
void handle_echo_packet_from_gateway(u8 *in_buf)
{
	lpkt_u lp;
	errno_t ret = EOK;

	if (NULL == in_buf){
		return;
	}

	ret = hwapi10_decode_packet_from_gateway(in_buf, &lp);
	if(EOK != ret){
		goto ACK_ERROR_TO_GATEWAY;
	}

	//todo handle packet
	

	ret = hwapi10_echo_packet_to_gateway(&lp);
	if (EOK != ret){
		goto ACK_ERROR_TO_GATEWAY;
	}

	return;

ACK_ERROR_TO_GATEWAY:
	hwapi10_ack_error_to_gateway(ret);
}


errno_t is_valid_header(lpro_u * in_lpro)
{
	if (in_lpro == NULL){
		return E_NULL_POINTER;
	}
	
	if (LOCK_HEADER_5FE6 == SH_GET_2_BYTE(in_lpro->lpro2.header)){
		return EOK;
	}else{
		return E_INVALID_HEADER;
	}
}


errno_t encode_lock_packet_to_gateway(u8 cmd, u8 payload_len, lpp_u * in_lpp)
{
	lpkt_u lpkt;
	u16 crc = 0;
	
	if (in_lpp == NULL){
		return E_NULL_POINTER;
	}

	memcpy(lpkt.lpkt0, gw_addr_channel.rfac2.addr, RF_ADDR_SIZE);
	lpkt.lpkt1.lock_channel = gw_addr_channel.rfac1.channel;

	
	SH_PUT_4_BYTE(lpkt.lpkt2.lpro.lpro2.rnd,0x12345678);
	SH_PUT_2_BYTE(lpkt.lpkt2.lpro.lpro2.header, LOCK_HEADER_5FE6);
	lpkt.lpkt2.lpro.lpro2.len[0]=payload_len;	
	lpkt.lpkt2.lpro.lpro2.cmd[0]=cmd;

	memcpy(lpkt.lpkt2.lpro.lpro2.lpp.lpp0, in_lpp->lpp0, LOCK_DATA_PAYLOAD_SIZE_MAX);

	crc = crc16(lpkt.lpkt0, LOCK_PACKET_SIZE - LOCK_CRC_SIZE);
	SH_PUT_2_BYTE(lpkt.lpkt2.lpro.lpro2.crc,crc);


	send2gateway(lpkt.lpkt0, LOCK_PACKET_SIZE);
	return EOK;
	
}


void debug_log(u8 *debug_buf, u8 size)
{
	lpkt_u lpkt;

	memcpy(lpkt.lpkt0, gw_addr_channel.rfac2.addr, RF_ADDR_SIZE);
    lpkt.lpkt1.lock_channel = gw_addr_channel.rfac1.channel;

	memcpy(lpkt.lpkt2.lpro.lpro0, debug_buf, size);
	send2gateway(lpkt.lpkt0, sizeof(rfac_u)+size);
}

errno_t cmd13_CMD_REPORT_HEARTBEAT_TIME(void)
{
	lpp_u lpp;
	u8 time[TIME_SIZE]={0x20,0x17,0x04,0x13,0x19,0x35};	
	u8 room_addr[ROOM_ADDR_SIZE] = {0X00,0X00,0X01,0X01,0X01,0X01,0X02,0X01};
	
	// lock do something

	//hwapi01_beep_crtl(ON);
	//delay_ms(100);
	//hwapi01_beep_crtl(OFF);	
	// todo:get time
	
	memcpy(lpp.lpp_heartbeat_time.time1, time, TIME_SIZE);
	lpp.lpp_heartbeat_time.heartlen2 = 60;//minute
	lpp.lpp_heartbeat_time.cardver3 = 1;
	lpp.lpp_heartbeat_time.cardnum4 = 3;
	lpp.lpp_heartbeat_time.keyver5 = 5;
	lpp.lpp_heartbeat_time.livestate6 = 1;
	memcpy(lpp.lpp_heartbeat_time.room_addr7, room_addr,ROOM_ADDR_SIZE);
	lpp.lpp_heartbeat_time.wait_time8 = 3;
	memset(lpp.lpp_heartbeat_time.reserved9,0x00, 2);
	
	encode_lock_packet_to_gateway(CMD_REPORT_HEARTBEAT_TIME, PAYLOAD_LEN_HEARTBEAT_TIME,&lpp);

	return EOK;
}


void test_cmd13_CMD_REPORT_HEARTBEAT_TIME(void)
{
	
	cmd13_CMD_REPORT_HEARTBEAT_TIME();
	delay_ms(10000);
}

 
errno_t handle_cmd(lpro_u * p_lpro)
{
	if (p_lpro == NULL){
		return E_NULL_POINTER;
	}
	switch(p_lpro->lpro1.cmd){
		case CMD_UPDATE_HEARTBEAT_TIME:{

			//hwapi01_beep_cnt(1,100);
		}break;

		case CMD_UPDATE_ROOM_ADDR:{

		}break;

		

		case CMD_UPDATE_ROOM_STATUS:{

		}break;

		case ACK_REPORT_OPENDOOR:
		{
			//
		}break;

		
		case CMD_ROMOTE_OPEN_DOOR:
		{
			open_door();

			cmd14_CMD_REPORT_OPENDOOR();
		}break;


		default:{
			return E_INVALID_CMD;
		}//break;			
	}
	return EOK;
}

void protocol_cmd_process(lpkt_u *in_lp)
{
	errno_t ret = EOK;
	lpro_u *p_lpro = NULL;
	
	if (in_lp == NULL){
		ret = E_NULL_POINTER;
		goto ACK_ERROR_TO_GATEWAY;
	}

	p_lpro = &in_lp->lpkt2.lpro;//get protocol from rnd to crc.

	
	//todo: rnd, encryption/decryption
	// 00 00 00 02 28 12 34 56 78 5F E6 12 31 4B FD B9 B6 78 01 03 05 01 00 00 01 01 01 01 02 01 03 00 00 00 00 85 4A
	// 00 00 00 01 11 12 34 56 78 5F E6 12 31 4B FD B9 B6 78 05 01 00 00 01 01 01 01 02 01 03 00 00 00 00 00 00 3D 8B 


	//debug_log(p_lpro->lpro0, sizeof(lpro_u));
	//return;
	
	ret  = is_valid_header(p_lpro);
	if (ret != EOK){
		goto ACK_ERROR_TO_GATEWAY;
	}
	
	
	ret = is_valid_crc(p_lpro);
	if (ret != EOK){
		goto ACK_ERROR_TO_GATEWAY;
	}

	ret = handle_cmd(p_lpro);
	if (ret != EOK){
		goto ACK_ERROR_TO_GATEWAY;
	}
	return;

ACK_ERROR_TO_GATEWAY:
	hwapi10_ack_error_to_gateway(ret);
}


void hwapi10_handle_packet_from_gateway(u8 *in_buf)
{
	lpkt_u lp;
	errno_t ret = EOK;

	if (NULL == in_buf){
		return;
	}

	memset(&lp, 0x00, sizeof(lpkt_u));

	ret = hwapi10_decode_packet_from_gateway(in_buf, &lp);
	if(EOK != ret){
		goto ACK_ERROR_TO_LOCK;
	}

	//todo handle packet
	
	protocol_cmd_process(&lp);


	return;

ACK_ERROR_TO_LOCK:
	hwapi10_ack_error_to_gateway(ret);
}


void test_echo_gateway_packet_thread(void)
{
	u16 i=0;

	if (uart0.rflag == 1){
	
		delay_ms(20);/*TBD*/

		for (i=0; i<UART_RBUF_SIZE-LOCK_PACKET_SIZE+1; i++){
			if (EOK == is_valid_packet_from_gateway(uart0.rbuf+i)){
				Enqueue(&gq, i);
			}
		}
		while (!is_queue_empty(&gq)){

			hwapi01_beep_crtl(ON);
			delay_ms(100);
			hwapi01_beep_crtl(OFF);
			
			handle_echo_packet_from_gateway(uart0.rbuf + front(&gq));
			Dequeue(&gq);
			delay_ms(10);//sendto server time
		}
		CLEAR_UART(&uart0);
	}
}


//lock send packet to server needed : more than 120ms

void handle_gateway_packet_thread(void)
{
	u16 i=0;

	if (uart0.rflag == 1){
	
		delay_ms(20);/*TBD*/

		for (i=0; i<UART_RBUF_SIZE-LOCK_PACKET_SIZE+1; i++){
			if (EOK == is_valid_packet_from_gateway(uart0.rbuf+i)){
				Enqueue(&gq, i);
			}
		}
		while (!is_queue_empty(&gq)){
			hwapi10_handle_packet_from_gateway(uart0.rbuf + front(&gq));
			Dequeue(&gq);
			delay_ms(10);//sendto server time
		}
		CLEAR_UART(&uart0);
	}
}

#if 1
void test_lock_packet_union(void)
{
	lpkt_u lp_u;

	/*

	lock right order
	00 00 00 01 11 12 34 56 78 56 59 01 01 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 ee ff
	
	server right order
	00 00 00 01 11 00 00 00 02 28 12 34 56 78 56 59 01 01 00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f 10 11 12 13 14 15 ee ff 0d 0a 
	000000011100000002281234567856590101000102030405060708090a0b0c0d0e0f101112131415eeff0d0a
	*/

	/*step 1, encode plaintext*/

	u8 x[LOCK_DATA_PAYLOAD_SIZE_MAX]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21};


	memcpy(lp_u.lpkt2.lock_addr, gw_addr_channel.rfac2.addr, RF_ADDR_SIZE);
	lp_u.lpkt1.lock_channel = gw_addr_channel.rfac1.channel;

	//memcpy(lp_u.lpkt0, gw_addr_channel.rfac0, sizeof(rfac_u));

	SH_PUT_4_BYTE(lp_u.lpkt2.lpro.lpro2.rnd,0x12345678);
	SH_PUT_2_BYTE(lp_u.lpkt2.lpro.lpro2.header,0x5659);
	lp_u.lpkt2.lpro.lpro2.len[0]=0x01;	
	lp_u.lpkt2.lpro.lpro2.cmd[0]=0x01;

	memcpy(lp_u.lpkt2.lpro.lpro2.lpp.lpp0, x, sizeof(x));

	SH_PUT_2_BYTE(lp_u.lpkt2.lpro.lpro2.crc, 0xeeff);	

	/*step. send*/
	send2gateway(lp_u.lpkt0, sizeof(lpkt_u));
	delay_ms(1000);
}
#endif


errno_t cmd30_CMD_REPORT_TOUCH(void)
{
	lpp_u lpp;
	
	u8 time[TIME_SIZE]={0x20,0x17,0x04,0x13,0x19,0x35};
	u8 room_addr[ROOM_ADDR_SIZE] = {0X00,0X00,0X01,0X01,0X01,0X01,0X02,0X01};
	
	memcpy(lpp.lpp_touch_cmd.time1, time, TIME_SIZE);

	lpp.lpp_touch_cmd.heartlen2 = 60;//minute
	lpp.lpp_touch_cmd.cardver3 = 1;
	lpp.lpp_touch_cmd.cardnum4 = 3;
	lpp.lpp_touch_cmd.keyver5 = 5;
	lpp.lpp_touch_cmd.livestate6 = 1;
	memcpy(lpp.lpp_touch_cmd.room_addr7, room_addr,ROOM_ADDR_SIZE);
	lpp.lpp_touch_cmd.wait_time8 = 3;
	memset(lpp.lpp_touch_cmd.reserved9,0x00, sizeof(lpp.lpp_touch_cmd.reserved9));
	
	encode_lock_packet_to_gateway(CMD_REPORT_TOUCH, PAYLOAD_LEN_CMD_REPORT_TOUCH,&lpp);

	return EOK;
}


errno_t cmd14_CMD_REPORT_OPENDOOR(void)
{
	lpp_u lpp;
	
	//debug
	u8 card[4]={1,2,3,4};
	u8 room_addr[ROOM_ADDR_SIZE] = {0X00,0X00,0X01,0X01,0X01,0X01,0X02,0X01};
	
	lpp.lpp_report_opendoor.door1=0x01;
	lpp.lpp_report_opendoor.dtype2=0x07;
	lpp.lpp_report_opendoor.drole3=0x01;
	lpp.lpp_report_opendoor.dopenret4 = 0x01;

	memcpy(lpp.lpp_report_opendoor.dcard5,card,4);
	lpp.lpp_report_opendoor.dalarm6 = 0x01;
	lpp.lpp_report_opendoor.cardver7 = 1;
	lpp.lpp_report_opendoor.keyver8= 5;
	lpp.lpp_report_opendoor.livestate9= 1;
	
	memcpy(lpp.lpp_report_opendoor.room_addr10, room_addr,ROOM_ADDR_SIZE);
	
	memset(lpp.lpp_report_opendoor.reserved11,0x00, sizeof(lpp.lpp_report_opendoor.reserved11));
	
	encode_lock_packet_to_gateway(CMD_REPORT_OPENDOOR, PAYLOAD_LEN_CMD_REPORT_OPENDOOR,&lpp);

	return EOK;
}


errno_t cmd14_CMD_REPORT_OPENDOOR_ICCARD(iccard_t *p_card)
{
	lpp_u lpp;
	
	//debug
	
	u8 room_addr[ROOM_ADDR_SIZE] = {0X00,0X00,0X01,0X01,0X01,0X01,0X02,0X01};
	
	lpp.lpp_report_opendoor.door1=0x01;
	lpp.lpp_report_opendoor.dtype2=0x06;
	lpp.lpp_report_opendoor.drole3=0x01;
	lpp.lpp_report_opendoor.dopenret4 = 0x01;

	memcpy(lpp.lpp_report_opendoor.dcard5, p_card->b0.b0_buf,4);
	lpp.lpp_report_opendoor.dalarm6 = 0x01;
	lpp.lpp_report_opendoor.cardver7 = 1;
	lpp.lpp_report_opendoor.keyver8= 5;
	lpp.lpp_report_opendoor.livestate9= 1;
	
	memcpy(lpp.lpp_report_opendoor.room_addr10, room_addr,ROOM_ADDR_SIZE);
	
	memset(lpp.lpp_report_opendoor.reserved11,0x00, sizeof(lpp.lpp_report_opendoor.reserved11));
	
	encode_lock_packet_to_gateway(CMD_REPORT_OPENDOOR, PAYLOAD_LEN_CMD_REPORT_OPENDOOR,&lpp);

	return EOK;
}




void test_cmd14_CMD_REPORT_OPENDOOR(void)
{
	cmd14_CMD_REPORT_OPENDOOR();

	delay_ms(10000);
}



errno_t cmd32_CMD_REPORT_DOOR_STATE(door_state_t state)
{
	lpp_u lpp;
	
	//debug
	u8 card[4]={0};
	u8 room_addr[ROOM_ADDR_SIZE] = {0X00,0X00,0X01,0X01,0X01,0X01,0X02,0X01};
	
	lpp.lpp_report_door_state.doorstate1=state;
	lpp.lpp_report_door_state.dtype2=0;
	lpp.lpp_report_door_state.drole3=0;
	lpp.lpp_report_door_state.dopenret4 = 0;

	memcpy(lpp.lpp_report_door_state.dcard5,card,4);
	lpp.lpp_report_door_state.dalarm6 = 0;
	lpp.lpp_report_door_state.cardver7 = 1;
	lpp.lpp_report_door_state.keyver8= 5;
	lpp.lpp_report_door_state.livestate9= 1;
	
	memcpy(lpp.lpp_report_door_state.room_addr10, room_addr,ROOM_ADDR_SIZE);
	
	memset(lpp.lpp_report_door_state.reserved11,0x00, sizeof(lpp.lpp_report_door_state.reserved11));
	
	encode_lock_packet_to_gateway(CMD_REPORT_DOOR_STATE, PAYLOAD_LEN_CMD_REPORT_DOOR_STATE,&lpp);

	return EOK;
}



const unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
};

const char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
} ;

//Modicon CRC Generation Function
u16 crc16(u8 *puchMsg,  u8 usDataLen)
{
	unsigned char uchCRCHi = 0xff;		//high byte of CRC initialized
	unsigned char uchCRCLo = 0xff;		//low byte of CRC initialized
	unsigned char uchIndex;

	while(usDataLen--)
	{
		uchIndex = uchCRCHi ^ *puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uchIndex];
		uchCRCLo = auchCRCLo[uchIndex];
	}
	return (uchCRCHi<<8 | uchCRCLo);
}


errno_t is_valid_crc(lpro_u * in_lpro)
{
	u16 crc_recv = 0;
	u16 crc_calc = 0;
	
	if (in_lpro == NULL){
		return E_NULL_POINTER;
	}

	crc_recv = SH_GET_2_BYTE(in_lpro->lpro2.crc);
	crc_calc = crc16(in_lpro->lpro0, LOCK_PROTOCOL_SIZE - LOCK_CRC_SIZE);//include the last zeros...
	if (crc_recv  == crc_calc){
		return EOK;
	}else{
		return E_INVALID_CRC;
	}	
}


