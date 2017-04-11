#include "main.h"

uart_t uart0;

void CLEAR_UART_RECV(uart_t *p)
{
	p->rflag=0;
	p->rindex=0;
}

void UART0RecvEnable(void)
{
	LPC_USART0->INTENSET = (1 << 0);
}

void UART0RecvDisable(void)
{
	LPC_USART0->INTENCLR = (1 << 0);
}
void UART0SendEnable(void)
{
	LPC_USART0->INTENSET |= (1 << 2);
}

void UART0SendDisable(void)
{
	LPC_USART0->INTENCLR |= (1 << 2);
}

void UART0Init (void)
{
    LPC_SWM->PINASSIGN[0] &= ~( 0xFFFF << 0 );
    LPC_SWM->PINASSIGN[0] |=  ( 0 << 0 );                               /* P0.0 ~ UART0_RXD rf433m recv */
    LPC_SWM->PINASSIGN[0] |=  ( 6 << 8 );                               /* P0.6 ~ UART0_TXD rf433m send */

    LPC_SYSCON->UARTCLKDIV     = 1;                                     /* UART时钟分频值为 1           */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 14);                             /* 初始化UART AHB时钟           */

    LPC_USART0->BRG = SystemCoreClock * LPC_SYSCON->SYSAHBCLKDIV /
                      (LPC_SYSCON->UARTCLKDIV * 16 * UART0_BPS) - 1;     /* 串口通信波特率               */
    LPC_USART0->CFG = (1 << 0) |                                        /* 使能UART                     */
                      (1 << 2) |                                        /* 8位数据位                    */
                      (0 << 4) |                                        /* 无校验                       */
                      (0 << 6);                                         /* 1位停止位                    */

	UART0RecvEnable();
    NVIC_EnableIRQ(UART0_IRQn);

	CLEAR_UART(&uart0);
}

void uart0_sendbuf(u8* buf, u16 size)
{
	if (buf == NULL || size <=0){
		return;
	}
	memcpy(uart0.sbuf, buf, size);
	uart0.slen = size;
	uart0.sindex = 0;
	UART0SendEnable();
}

void UART0_IRQHandler (void)
{
    if (LPC_USART0->STAT & 0x01) {//recv
		uart0.rbuf[uart0.rindex++] = LPC_USART0->RXDATA;
		if (uart0.rindex >= UART_RBUF_SIZE){
			uart0.rindex = 0;
		}
		uart0.rflag=1;
    }

	if (LPC_USART0->STAT & 0x04) {//send
		if (uart0.slen == 0){
			return;
		}
		LPC_USART0->TXDATA = uart0.sbuf[uart0.sindex++];
		if (uart0.sindex >= uart0.slen) {
			UART0SendDisable();
			uart0.slen=0;
		}
	}
}

void test_uart0_echo(void)
{
	if (uart0.rflag == 1){
		//delay_ms(10);//waiting until the packet done
		uart0_sendbuf(uart0.rbuf, uart0.rindex);
		CLEAR_UART_RECV(&uart0);
	}
}


void test_uart0_send(void)
{
	u8 sbuf[1]={0x01};
	uart0_sendbuf(sbuf,sizeof(sbuf));
	delay_ms(1);
}


void uart0_thread(void)
{
	if (uart0.rflag == 1){
		delay_ms(20);//waiting until the packet done
		
		send2gateway(uart0.rbuf,uart0.rindex);		
		CLEAR_UART_RECV(&uart0);
		
	}
}
