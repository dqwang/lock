#include "main.h"

/*********************************************************************************************************
  �궨��
*********************************************************************************************************/
#define LED0             (1 << 20)                                       /* LED - P0.20                  */
#define LED1             (1 << 21)                                       /* LED - P0.21                  */


#define UART0_BPS        115200/*433M*/



#define UART_RBUF_SIZE (64)



typedef struct uart{
	u8 rflag;
	u8 rbusy;
	u8 rindex;
	u8 rbuf[UART_RBUF_SIZE];
	u8 slen;
	u8 sindex;
	u8 sbuf[UART_RBUF_SIZE];
}uart_t;

typedef enum ERRNO{
	EOK = 0,
	E_INVALID_PACKET,
	E_NULL_POINTER,
	E_INVALID_CMD,

	
}errno_t;

/*
typedef struct packet{
	u8 header[2];//0x56 0x59 
	u8 len;//total 
	u8 cmd;
	u8 payload[PAYLOAD_MAX_SIZE];	
}packet_t;

*/


uart_t uart0;





//#define DEBUG_LOG(buf, len) uart0_sendbuf(buf, len)
#define DEBUG_LOG(buf, len)


#define send2server(buf,len) uart0_sendbuf(buf, len)



#define CLEAR_UART(p) memset(p,0x0,sizeof(uart_t))


void myDelay (uint32_t ulTime)
{
    uint32_t i;
    
    while (ulTime--) {
        for (i = 0; i < 2401; i++);
    }
}

void WKTInit (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |=  (1 << 9);                             /* ��ʼ��WKT AHBʱ��            */
    LPC_SYSCON->PRESETCTRL    &= ~(1 << 9);
    LPC_SYSCON->PRESETCTRL    |=  (1 << 9);                             /* ��λWKT                      */
    
    /*
     * WKTʱ��Դѡ��
     * 0��IRC�����ڲ�RCʱ��Դ��750 kHz��    1��LPO�����͹���������10 kHz������ +/-45%��
     * WKT��ʱʱ�����ã�
     * IRC��N(ms) ���� COUNT = 750 * N      LPO��N(ms) ���� COUNT = 10 * N
     */
    LPC_WKT->CTRL  = 0;                                                 /* IRCΪʱ��Դ                  */
}

/*********************************************************************************************************
** Function name:       WKTdelayMs
** Descriptions:        WKT��ʱ
** input parameters:    delayInMs����ʱʱ�䣨��λ��ms��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void WKTdelayMs (uint32_t delayInMs)
{
    LPC_WKT->COUNT = 750 * delayInMs;                                   /* ��ʱ500 ms                   */
    
    while( !(LPC_WKT->CTRL & (1 << 1))) {                               /* �ȴ���ʱ                     */
        ;
    }
    
    LPC_WKT->CTRL |= (1 << 1);                                          /* �����ʱ��־                 */

	//time_ms_close_door++;
}

void GPIOInit (void)
{
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);                              /* ��ʼ��GPIO AHBʱ��           */
	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 7);                              /* ��ʼ��SWM  AHBʱ��           */

    //LPC_GPIO_PORT->DIR[0] |=  LED0;                                      /* ��P0.20��������Ϊ���        */
    //LPC_GPIO_PORT->PIN[0] |=  LED0;                                      /* P0.20�������Ϊ��            */

	//LPC_GPIO_PORT->DIR[0] |=  LED1;
	//LPC_GPIO_PORT->PIN[0] |=  LED1;


#if 0

	LPC_GPIO_PORT->DIR[0] &= ~KEY;


	LPC_SYSCON->PINTSEL[0] = 0x1;                                       /* ����P0.1Ϊ�жϴ�������       */
    LPC_PININT->ISEL   &= ~0x1;                                         /* ���ش���                     */
    LPC_PININT->SIENF   |=  0x1;                                         /* �±��ش���                   */
    
    NVIC_EnableIRQ(PIN_INT0_IRQn);                                      /* ��PININT0�ж�              */
#endif
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
    LPC_SWM->PINASSIGN[0] |=  ( 4 << 0 );                               /* P0.4 ~ UART0_TXD             */
    LPC_SWM->PINASSIGN[0] |=  ( 0 << 8 );                               /* P0.0 ~ UART0_RXD             */
    
    LPC_SYSCON->UARTCLKDIV     = 1;                                     /* UARTʱ�ӷ�ƵֵΪ 1           */
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 14);                             /* ��ʼ��UART AHBʱ��           */
    
    LPC_USART0->BRG = SystemCoreClock * LPC_SYSCON->SYSAHBCLKDIV /
                      (LPC_SYSCON->UARTCLKDIV * 16 * UART0_BPS) - 1;     /* ����ͨ�Ų�����               */
    LPC_USART0->CFG = (1 << 0) |                                        /* ʹ��UART                     */
                      (1 << 2) |                                        /* 8λ����λ                    */
                      (0 << 4) |                                        /* ��У��                       */
                      (0 << 6);                                         /* 1λֹͣλ                    */

	UART0RecvEnable();                        
    NVIC_EnableIRQ(UART0_IRQn);	

	CLEAR_UART(&uart0);
}



void gpio_ctrl(uint32_t gpio, uint32_t value)
{
	if (value == GPIO_LOW)
		LPC_GPIO_PORT->PIN[0] &= ~gpio;
	if (value == GPIO_HIGH)
		LPC_GPIO_PORT->PIN[0] |=  gpio;
}

void led0_on(void)
{
	gpio_ctrl(LED0, GPIO_LOW);
}

void led1_on(void)
{
	gpio_ctrl(LED1, GPIO_LOW);
}

void led0_off(void)
{
	gpio_ctrl(LED0, GPIO_HIGH);
}

void led1_off(void)
{
	gpio_ctrl(LED1, GPIO_HIGH);
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

void uart0_thread(void)
{
	if (uart0.rflag == 1){
		myDelay(10);//waiting until the packet done
		

		#if 0
		//debug
		uart0_sendbuf(uart0.rbuf, uart0.rindex);
		CLEAR_UART(&uart0);
		return;
		#endif

		CLEAR_UART(&uart0);
		//CLEAR_PACKET(&server);
	}
}


int main(void)
{
	myDelay(1000);//system poweron	
	
    SystemInit();                                                       /* ��ʼ��Ŀ��壬����ɾ��       */
    GPIOInit();                                                         /* GPIO��ʼ��                   */
	WKTInit();

	UART0Init();


	
    while (1) {
		uart0_thread();
    }
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
