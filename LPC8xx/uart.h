#ifndef __UART_H__
#define __UART_H__
#include "main.h"

#include "type.h"
#include "lock_protocol.h"


#define UART0_BPS   115200/*433M*/
#define UART0_BPS_CONFIG_RF433M 9600/*mode3, 9600bps, then you can config the rf433m*/

#define RF_SEND_DELAY_TIME 80//ms


#define UART1_BPS	115200/*GPRS*/
#define UART2_BPS	115200/*WIFI and RJ45*/

#define UART_RBUF_SIZE (LOCK_PACKET_SIZE*GW_PACKET_NUM_MAX)// for server: 24 packet max, for lock: 28 packet max 
#define UART_SBUF_SIZE (LOCK_PACKET_SIZE*2)


typedef struct uart{
	u8 rflag;
	u8 rbusy;
	u16 rindex;
	u8 rbuf[UART_RBUF_SIZE];
	u8 slen;
	u8 sindex;
	u8 sbuf[UART_SBUF_SIZE];
}uart_t;



void UART0Init (void);



void uart0_sendbuf(u8* buf, u16 size);



void CLEAR_UART_RECV(uart_t *p);
#define CLEAR_UART(p) memset(p,0x0,sizeof(uart_t))
#define CLEAR_TYPE(p, type) memset(p, 0x00, sizeof(type))


void UART0RecvEnable(void);


#endif
