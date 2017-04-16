#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

#define ON 1
#define OFF 0

#define GPIO_HIGH 1
#define GPIO_LOW 0
#define GPIO_OUTPUT 1
#define GPIO_INPUT 0


void GPIOInit (void);
void gpio_dir(uint32_t gpio, uint32_t dir);
void gpio_ctrl(uint32_t gpio, uint32_t value);


#define GPIO_BEEP	(1 << 14)//beep P0.14

void gpio_init_beep(void);
void hwapi01_beep_crtl(u8 on_off);
void test_hwapi01_beep_crtl(void);
void hwapi01_beep_cnt(u8 cnt, u16 ms);

#endif
