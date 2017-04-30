#ifndef __POWER_ADC_H__
#define __POWER_ADC_H__
#include "type.h"

#define POWER_ADC_CHANNEL 10//ADC 10

#define LOW_VOLTAGE_ALARM 0X0A6C// 4.3V
#define LOW_VOLTAGE_WARNING 0X0B9C// 4.8V

typedef enum{
	POWER_OK = 0,
	POWER_LOW_WARNING = 0X11,
	POWER_LOW_ALARM = 0X22,
	POWER_INVALID	
}power_adc_t;

void power_adc_alibration( void );

void power_adc_init(void);

void power_adc_start(uint8_t ucChannel);

void power_adc_thread(void);

void test_power_adc(void);

power_adc_t power_adc_check(uint32_t adc_value);
void test_power_adc_check(void);

#endif
