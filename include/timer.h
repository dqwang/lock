#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct timer{
	u16 cnt;
	u8 flag;
}timer_t;

void delay_ms (uint32_t ulTime);

void WKTInit (void);
void WKTdelayMs (uint32_t delayInMs);

void test_timer_reset(void);

void test_timer_thread(void);

void open_door_timer_reset(void);
void open_door_timer_set(void);


#endif

