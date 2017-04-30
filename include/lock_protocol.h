#ifndef __LOCK_PROTOCOL_H__
#define __LOCK_PROTOCOL_H__
#include "main.h"
#include "errno.h"

#include "type.h"
#include "iccard.h"
#include "door.h"
#include "power_adc.h"


#define LOCK_MAX_NUM_PER_GATEWAY 20

#define FOR_LPC824 1

/*************************************
 define the server packet offset
*************************************/

#define TAIL_ENTER 0X0D0A

#define SERVER_PACKET_NUM_MAX 24
#define LOCK_PACKET_NUM_MAX 28
#define GW_PACKET_NUM_MAX 28

/*************************************
 define the packet size
*************************************/
#define RF_ADDR_SIZE 4
#define RF_CHANNEL_SIZE 1

#define SERVER_PACKET_SIZE 44
#define SERVER_PAYLOAD_SIZE 37
#define SERVER_TAIL_SIZE 2

#define LOCK_PACKET_SIZE 37
#define LOCK_PROTOCOL_SIZE 32// except the addr and channel

#define LOCK_RND_SIZE 4
#define LOCK_HEADER_SIZE 2
#define LOCK_LEN_SIZE 1
#define LOCK_CMD_SIZE 1
#define LOCK_DATA_PAYLOAD_SIZE_MAX 22// data size
#define LOCK_CRC_SIZE 2


#define LOCK_HEADER_5FE6 0x5FE6

#define ROOM_ADDR_SIZE 8
#define TIME_SIZE 6

enum cmd{
	//cmd lock-->server

	CMD_REPORT_HEARTBEAT_TIME = 0X12,
	CMD_REPORT_OPENDOOR = 0X14,
	CMD_REPORT_LOW_VOLTAGE = 0X16,
	CMD_REQUEST_ROOM_ADDR =0X18,
	CMD_REQUEST_ROOM_STATUS= 0X21,
	CMD_REQUEST_ICCARD_WHITELIST = 0X24,
	CMD_REQUEST_ICCARD_KEY = 0X27,
	CMD_REPORT_TOUCH = 0X30,
	CMD_REPORT_DOOR_STATE = 0X32,
	
	//cmd server --> lock
	CMD_UPDATE_HEARTBEAT_TIME  = 0x13,
	CMD_UPDATE_ROOM_ADDR = 0x19,
	CMD_UPDATE_ROOM_STATUS = 0x22,
	CMD_UPDATE_ICCARD_WHITELIST = 0X25,
	CMD_UPDATE_ICCARD_KEY = 0x28,
	CMD_ROMOTE_OPEN_DOOR = 0X31,
	
	//ack from server to lock
	ACK_REPORT_OPENDOOR = 0x15,
	ACK_REPORT_LOW_VOLTAGE = 0x17,

	
	//ack lock -->server
	ACK_UPDATE_ROOM_ADDR = 0X20,
	ACK_UPDATE_ROOM_STATUS = 0X23,
	ACK_UPDATE_ICCARD_WHITELIST = 0X26,
	ACK_UPDATE_ICCARD_KEY=0X29,	
};

/*----------------------------------------------------------------------------*/

#pragma pack (1)/*byte alignment*/


//CMD_REPORT_HEARTBEAT_TIME = 0X12,
#define PAYLOAD_LEN_CMD_REPORT_HEARTBEAT_TIME 21
typedef struct lpp1{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;//0 1
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REPORT_HEARTBEAT_TIME];	
}lpp_CMD_REPORT_HEARTBEAT_TIME_t;


//CMD_UPDATE_HEARTBEAT_TIME  = 0x13,
#define PAYLOAD_LEN_CMD_UPDATE_HEARTBEAT_TIME 21
typedef struct lpp1_{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_UPDATE_HEARTBEAT_TIME];	
}lpp_CMD_UPDATE_HEARTBEAT_TIME_t;



#define RANDSNS_LEN 4
#define PAYLOAD_LEN_CMD_REPORT_TOUCH 21

//CMD_REPORT_TOUCH = 0X30,
typedef struct lpp2{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;//0 1
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REPORT_TOUCH];	
}lpp_CMD_REPORT_TOUCH_t;


//CMD_ROMOTE_OPEN_DOOR = 0X31,

#define PAYLOAD_LEN_CMD_ROMOTE_OPEN_DOOR 19

typedef struct lpp2_{
	u8 randsns1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;//0 1
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_ROMOTE_OPEN_DOOR];	
}lpp_CMD_ROMOTE_OPEN_DOOR_t;


//CMD_REPORT_OPENDOOR = 0X14,
#define PAYLOAD_LEN_CMD_REPORT_OPENDOOR 21
typedef struct lpp3{
	u8 door1;
	u8 dtype2;
	u8 drole3;
	u8 dopenret4;
	u8 dcard5[ICCARD_ID_SIZE];
	u8 dalarm6;
	u8 cardver7;
	u8 keyver8;
	u8 livestate9;
	u8 room_addr10[ROOM_ADDR_SIZE];
	u8 activated_state11;//0 1
	u8 reserved12[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REPORT_OPENDOOR];
}lpp_CMD_REPORT_OPENDOOR_t;


//ACK_REPORT_OPENDOOR = 0x15,
#define PAYLOAD_LEN_ACK_REPORT_OPENDOOR 21
typedef struct lpp3_{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;//0 1
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_ACK_REPORT_OPENDOOR];
}lpp_ACK_REPORT_OPENDOOR_t;


//CMD_REPORT_DOOR_STATE = 0X32,
#define PAYLOAD_LEN_CMD_REPORT_DOOR_STATE 21
typedef struct lpp4{
	u8 doorstate1;
	u8 dtype2;
	u8 drole3;
	u8 dopenret4;
	u8 dcard5[4];
	u8 dalarm6;
	u8 cardver7;
	u8 keyver8;
	u8 livestate9;
	u8 room_addr10[ROOM_ADDR_SIZE];
	u8 activated_state11;
	u8 reserved12[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REPORT_DOOR_STATE];
}lpp_CMD_REPORT_DOOR_STATE_t;



//CMD_REPORT_LOW_VOLTAGE = 0X16,
#define PAYLOAD_LEN_CMD_REPORT_LOW_VOLTAGE  16
typedef struct lpp5{
	u8 cell1;//power_adc_t
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REPORT_LOW_VOLTAGE];
}lpp_CMD_REPORT_LOW_VOLTAGE_t;

//ACK_REPORT_LOW_VOLTAGE = 0x17,
#define PAYLOAD_LEN_ACK_REPORT_LOW_VOLTAGE 21
typedef struct lpp5_{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_ACK_REPORT_LOW_VOLTAGE];
}lpp_ACK_REPORT_LOW_VOLTAGE_t;


//CMD_REQUEST_ROOM_ADDR =0X18,
#define PAYLOAD_LEN_CMD_REQUEST_ROOM_ADDR 21
typedef struct lpp6{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REQUEST_ROOM_ADDR];
}lpp_CMD_REQUEST_ROOM_ADDR_t;



//CMD_UPDATE_ROOM_ADDR = 0x19,
#define PAYLOAD_LEN_CMD_UPDATE_ROOM_ADDR 21
typedef struct lpp6_{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_UPDATE_ROOM_ADDR];
}lpp_CMD_UPDATE_ROOM_ADDR_t;



//ACK_UPDATE_ROOM_ADDR = 0X20,
#define PAYLOAD_LEN_ACK_UPDATE_ROOM_ADDR 21
typedef struct lpp6__{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_ACK_UPDATE_ROOM_ADDR];
}lpp_ACK_UPDATE_ROOM_ADDR_t;



//CMD_REQUEST_ROOM_STATUS= 0X21,
#define PAYLOAD_LEN_CMD_REQUEST_ROOM_STATUS 21
typedef struct lpp7{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REQUEST_ROOM_STATUS];
}lpp_CMD_REQUEST_ROOM_STATUS_t;


//CMD_UPDATE_ROOM_STATUS = 0x22,
#define PAYLOAD_LEN_CMD_UPDATE_ROOM_STATUS 21
typedef struct lpp7_{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_UPDATE_ROOM_STATUS];
}lpp_CMD_UPDATE_ROOM_STATUS_t;


//ACK_UPDATE_ROOM_STATUS = 0X23,
#define PAYLOAD_LEN_ACK_UPDATE_ROOM_STATUS 21
typedef struct lpp7__{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_ACK_UPDATE_ROOM_STATUS];
}lpp_ACK_UPDATE_ROOM_STATUS_t;


//CMD_REQUEST_ICCARD_WHITELIST = 0X24,
#define PAYLOAD_LEN_CMD_REQUEST_ICCARD_WHITELIST 21
typedef struct lpp8{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REQUEST_ICCARD_WHITELIST];
}lpp_CMD_REQUEST_ICCARD_WHITELIST_t;

//CMD_UPDATE_ICCARD_WHITELIST = 0X25,
#define PAYLOAD_LEN_CMD_UPDATE_ICCARD_WHITELIST 22
#define MAX_CARD_NUM 5
typedef struct lpp8_{
	u8 cardver1;
	u8 vtype2;//01 add, 02 delete, 00, clean all
	u8 vcard3[ICCARD_ID_SIZE*MAX_CARD_NUM];
}lpp_CMD_UPDATE_ICCARD_WHITELIST;


//ACK_UPDATE_ICCARD_WHITELIST = 0X26,
#define PAYLOAD_LEN_ACK_UPDATE_ICCARD_WHITELIST 21
typedef struct lpp8__{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_ACK_UPDATE_ICCARD_WHITELIST];
}lpp_ACK_UPDATE_ICCARD_WHITELIST_t;



//CMD_REQUEST_ICCARD_KEY = 0X27,
#define  PAYLOAD_LEN_CMD_REQUEST_ICCARD_KEY 21
typedef struct lpp9{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_CMD_REQUEST_ICCARD_KEY];
}lpp_CMD_REQUEST_ICCARD_KEY_t;


//CMD_UPDATE_ICCARD_KEY = 0x28,
#define PAYLOAD_LEN_CMD_UPDATE_ICCARD_KEY 20
typedef struct lpp9_{
	u8 keyver1;
	u8 keylen2;//6 12 18
	u8 key3[KEYA_SIZE];
	u8 key4[KEYA_SIZE];
	u8 key5[KEYA_SIZE];
	u8 reserved6[LOCK_DATA_PAYLOAD_SIZE_MAX - PAYLOAD_LEN_CMD_UPDATE_ICCARD_KEY];
}lpp_CMD_UPDATE_ICCARD_KEY_t;


//ACK_UPDATE_ICCARD_KEY=0X29,
#define  PAYLOAD_LEN_ACK_UPDATE_ICCARD_KEY 21
typedef struct lpp9__{
	u8 time1[TIME_SIZE];
	u8 heartlen2;
	u8 cardver3;
	u8 cardnum4;
	u8 keyver5;
	u8 livestate6;
	u8 room_addr7[ROOM_ADDR_SIZE];
	u8 wait_time8;
	u8 activated_state9;
	u8 reserved10[LOCK_DATA_PAYLOAD_SIZE_MAX-PAYLOAD_LEN_ACK_UPDATE_ICCARD_KEY];
}lpp_ACK_UPDATE_ICCARD_KEY_t;



typedef union{
	u8       							lpp0[LOCK_DATA_PAYLOAD_SIZE_MAX];

	lpp_CMD_REPORT_HEARTBEAT_TIME_t		lpp_report_heartbeat_time;// 0x12
	lpp_CMD_UPDATE_HEARTBEAT_TIME_t		lpp_update_heartbeat_time;// 0x13

	lpp_CMD_REPORT_OPENDOOR_t 			lpp_report_opendoor;// 0x14
	lpp_ACK_REPORT_OPENDOOR_t			lpp_ack_report_opendoor;// 0x15
	
	lpp_CMD_REPORT_LOW_VOLTAGE_t 		lpp_report_low_voltage;// 0x16
	lpp_ACK_REPORT_LOW_VOLTAGE_t		lpp_ack_report_low_voltage;// 0x17

	lpp_CMD_REQUEST_ROOM_ADDR_t 		lpp_request_room_addr;// 0x18
	lpp_CMD_UPDATE_ROOM_ADDR_t			lpp_update_room_addr;// 0x19
	lpp_ACK_UPDATE_ROOM_ADDR_t			lpp_ack_update_room_addr;// 0x20

	lpp_CMD_REQUEST_ROOM_STATUS_t 		lpp_request_room_status;// 0x21
	lpp_CMD_UPDATE_ROOM_STATUS_t		lpp_update_room_status;// 0x22
	lpp_ACK_UPDATE_ROOM_STATUS_t		lpp_ack_update_room_status;// 0x23
		
	lpp_CMD_REQUEST_ICCARD_WHITELIST_t 	lpp_request_iccard_whitelist;// 0x24
	lpp_CMD_UPDATE_ICCARD_WHITELIST     lpp_update_iccard_whitelist;// 0x25
	lpp_ACK_UPDATE_ICCARD_WHITELIST_t   lpp_ack_update_iccard_whitelist;// 0x26

	lpp_CMD_REQUEST_ICCARD_KEY_t		lpp_request_iccard_key;// 0x27
	lpp_CMD_UPDATE_ICCARD_KEY_t			lpp_update_iccard_key;// 0x28
	lpp_ACK_UPDATE_ICCARD_KEY_t			lpp_ack_update_iccard_key;// 0x29

	lpp_CMD_REPORT_TOUCH_t 				lpp_report_touch;// 0x30
	lpp_CMD_ROMOTE_OPEN_DOOR_t			lpp_remote_open_door;// 0x31

	lpp_CMD_REPORT_DOOR_STATE_t 		lpp_report_door_state;// 0x32	
}lpp_u;

/*----------------------------------------------------------------------------*/

typedef struct lock_protocol1{
	u32 rnd;
	u16 header;
	u8 len;
	u8 cmd;
	lpp_u lpp;
	u16 crc;
}lpro_t1;


typedef struct lock_protocol2{
	u8 rnd[LOCK_RND_SIZE];
	u8 header[LOCK_HEADER_SIZE];
	u8 len[LOCK_LEN_SIZE];
	u8 cmd[LOCK_CMD_SIZE];
	lpp_u lpp;
	u8 crc[LOCK_CRC_SIZE];
}lpro_t2;


typedef union{
	u8      lpro0[LOCK_PROTOCOL_SIZE];// from rnd to crc
	lpro_t1 lpro1;
	lpro_t2 lpro2;
}lpro_u;

/*----------------------------------------------------------------------------*/

typedef struct lock_packet1{
	u32    lock_addr;
	u8     lock_channel;
	lpro_u lpro;
	
}lpkt_t1;

typedef struct lock_packet2{
	u8     lock_addr[RF_ADDR_SIZE];
	u8     lock_channel[RF_CHANNEL_SIZE];
	lpro_u lpro;
}lpkt_t2;


typedef union{//from lock addr to crc
	u8 		lpkt0[LOCK_PACKET_SIZE];
	lpkt_t1 lpkt1;
	lpkt_t2 lpkt2;
	
}lpkt_u;

/*----------------------------------------------------------------------------*/

typedef struct server_packet1{
	u32    gw_addr;
	u8     gw_channel;
	lpkt_u lpkt;
	u16    tail;//0x0d 0x0a
}sp_t1;

typedef struct server_packet2{
	u8     gw_addr[RF_ADDR_SIZE];
	u8     gw_channel[RF_CHANNEL_SIZE];
	lpkt_u lpkt;
	u8     tail[SERVER_TAIL_SIZE];
}sp_t2;

typedef union{//from gateway addr to enter
	u8    sp0[SERVER_PACKET_SIZE];
	sp_t1 sp1;
	sp_t2 sp2;	
}sp_u;

/*----------------------------------------------------------------------------*/

typedef struct rf_addr_channel1{
	u32 addr;
	u8 channel;
}rfac_t1;


typedef struct rf_addr_channel2{
	u8 addr[RF_ADDR_SIZE];
	u8 channel[RF_CHANNEL_SIZE];
}rfac_t2;

typedef union{
	u8      rfac0[RF_ADDR_SIZE + RF_CHANNEL_SIZE];
	rfac_t1 rfac1;
	rfac_t2 rfac2;
}rfac_u;



#pragma pack()/*stop alignment*/


/*----------------------------------------------------------------------------*/




//#define SH_GET_API_ID(p_api)    (p_api[SH_API_ID_OFFSET])
//#define SH_GET_API_VER(p_api)   (p_api[SH_API_VER_OFFSET])
//#define SH_GET_API_NELEM(p_api) (p_api[SH_API_NELEM_OFFSET])
//#define SH_GET_API_LEN(p_api)   (p_api[SH_API_LEN_LSB] |(p_api[SH_API_LEN_MSB] << 8))
//#define SH_GET_API_MTPASP(p_api)(p_api[SH_API_MTPSAP_OFFSET])


#define SH_GET_4_BYTE_LE(p_byte)  (((((u8*)(p_byte))[0]) << 0) | \
                                ((((u8*)(p_byte))[1]) << 8) | \
                                ((((u8*)(p_byte))[2]) << 16) | \
                                ((((u8*)(p_byte))[3]) << 24))
#define SH_GET_4_BYTE_BE(p_byte)  (((((u8*)(p_byte))[3]) << 0) | \
                                ((((u8*)(p_byte))[2]) << 8) | \
                                ((((u8*)(p_byte))[1]) << 16) | \
                                ((((u8*)(p_byte))[0]) << 24))

#define SH_GET_3_BYTE_LE(p_byte)  (((((u8*)(p_byte))[0]) << 0) | \
                       ((((u8*)(p_byte))[1]) << 8) | \
                         ((((u8*)(p_byte))[2]) << 16))

#define SH_GET_2_BYTE_LE(p_byte)  (((((u8*)(p_byte))[0]) << 0) | \
                                ((((u8*)(p_byte))[1]) << 8))
#define SH_GET_2_BYTE_BE(p_byte)  (((((u8*)(p_byte))[1]) << 0) | \
                                ((((u8*)(p_byte))[0]) << 8))
#define SH_GET_ONE_BYTE_LE(p_byte) (((u8*)(p_byte))[0])



#define SH_PUT_4_BYTE_LE(p_byte, val) \
	do \
	{ \
		(p_byte)[0] =(u8)(val); \
		(p_byte)[1] =(u8)((val) >> 8); \
		(p_byte)[2] =(u8)((val) >> 16); \
		(p_byte)[3] =(u8)((val) >> 24); \
	} while(0)
   
#define SH_PUT_4_BYTE_BE(p_byte, val) \
   do \
   { \
     (p_byte)[3] =(u8)(val); \
     (p_byte)[2] =(u8)((val) >> 8); \
     (p_byte)[1] =(u8)((val) >> 16); \
     (p_byte)[0] =(u8)((val) >> 24); \
   } while(0)

#define SH_PUT_3_BYTE_LE(p_byte, val) \
	do \
	{ \
		(p_byte)[0] =(u8)(val); \
		(p_byte)[1] =(u8)((val) >> 8); \
		(p_byte)[2] =(u8)((val) >> 16); \
	} while(0)

#define SH_PUT_2_BYTE_LE(p_byte, val) \
	do \
	{ \
		(p_byte)[0] =(u8)(val); \
		(p_byte)[1] =(u8)((val) >> 8); \
	} while(0)

#define SH_PUT_2_BYTE_BE(p_byte, val) \
	do \
	{ \
		(p_byte)[1] =(u8)(val); \
		(p_byte)[0] =(u8)((val) >> 8); \
	} while(0)


#if (FOR_LPC824)//big endian
#define SH_PUT_4_BYTE(p_byte,val) SH_PUT_4_BYTE_BE(p_byte,val)
#define SH_PUT_2_BYTE(p_byte, val) SH_PUT_2_BYTE_BE(p_byte, val)

#define SH_GET_4_BYTE(p_byte) SH_GET_4_BYTE_BE(p_byte)
#define SH_GET_2_BYTE(p_byte) SH_GET_2_BYTE_BE(p_byte)

#else// little endian
#define SH_PUT_4_BYTE(p_byte,val) SH_PUT_4_BYTE_LE(p_byte,val)
#define SH_PUT_2_BYTE(p_byte, val) SH_PUT_2_BYTE_LE(p_byte, val)

#define SH_GET_4_BYTE(p_byte) SH_GET_4_BYTE_LE(p_byte)
#define SH_GET_2_BYTE(p_byte) SH_GET_2_BYTE_LE(p_byte)

#endif

#define SH_PUT_ONE_BYTE_LE(p_byte,val)   (p_byte)[0] =(u8)((val))


/*----------------------------------------------------------------------------*/


#define send2gateway(buf, len) uart0_sendbuf(buf, len)


void lock_addr_channel_init(void);

void gateway_addr_channel_init(void);

errno_t is_valid_packet_from_gateway(u8 *in_buf);

errno_t hwapi10_decode_packet_from_gateway(u8 *in_buf, lpkt_u *out_lp);

errno_t hwapi10_echo_packet_to_gateway(lpkt_u *in_lp);

void hwapi10_ack_error_to_gateway(errno_t error);

void hwapi10_handle_packet_from_gateway(u8 *in_buf);

void handle_gateway_packet_thread(void);


void handle_echo_packet_from_gateway(u8 *in_buf);

u16 crc16(u8 *puchMsg,  u8 usDataLen);
errno_t is_valid_crc(lpro_u * in_lpro);

errno_t is_valid_header(lpro_u * in_lpro);

errno_t encode_lock_packet_to_gateway(u8 cmd, u8 payload_len, lpp_u * in_lpp);

errno_t cmd12_CMD_REPORT_HEARTBEAT_TIME(void);
errno_t cmd14_CMD_REPORT_OPENDOOR(void);
errno_t cmd14_CMD_REPORT_OPENDOOR_ICCARD(iccard_t *p_card);
errno_t cmd16_CMD_REPORT_LOW_VOLTAGE(power_adc_t adc);

errno_t cmd18_CMD_REQUEST_ROOM_ADDR(void);
errno_t cmd20_ACK_UPDATE_ROOM_ADDR(void);

errno_t cmd21_CMD_REQUEST_ROOM_STATUS(void);
errno_t cmd23_ACK_UPDATE_ROOM_STATUS(void);

errno_t cmd24_CMD_REQUEST_ICCARD_WHITELIST(void);
errno_t cmd26_ACK_UPDATE_ICCARD_WHITELIST(void);


errno_t cmd27_CMD_REQUEST_ICCARD_KEY(void);
errno_t cmd29_ACK_UPDATE_ICCARD_KEY(void);


errno_t cmd30_CMD_REPORT_TOUCH(void);
errno_t cmd32_CMD_REPORT_DOOR_STATE(door_state_t state);



errno_t handle_cmd(lpro_u * p_lpro);

void protocol_cmd_process(lpkt_u *in_lp);


void test_echo_gateway_packet_thread(void);
void test_lock_packet_union(void);
void test_hwapi10_ack_error_to_gateway(void);

void test_cmd12_CMD_REPORT_HEARTBEAT_TIME(void);
void test_cmd14_CMD_REPORT_OPENDOOR(void);
void test_cmd14_CMD_REPORT_OPENDOOR_ICCARD(void);

void test_cmd16_CMD_REPORT_LOW_VOLTAGE(void);
void test_cmd18_CMD_REQUEST_ROOM_ADDR(void);
void test_cmd20_ACK_UPDATE_ROOM_ADDR(void);


void test_cmd21_CMD_REQUEST_ROOM_STATUS(void);
void test_cmd23_ACK_UPDATE_ROOM_STATUS(void);
void test_cmd24_CMD_REQUEST_ICCARD_WHITELIST(void);
void test_cmd26_ACK_UPDATE_ICCARD_WHITELIST(void);

void test_cmd27_CMD_REQUEST_ICCARD_KEY(void);
void test_cmd29_ACK_UPDATE_ICCARD_KEY(void);


void debug_log(u8 *debug_buf, u8 size);

#endif
