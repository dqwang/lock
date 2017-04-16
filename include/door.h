#ifndef __DOOR_H__
#define __DOOR_H__

#define OPEN_DOOR_TIME 5000//ms

#define MOTOR_OPEN_TIME 150//ms
#define MOTOR_CLOSE_TIME 150//ms

#define GPIO_LG9110_IA (1 << 18)
#define GPIO_LG9110_IB (1 << 17)

#define GPIO_SIGNAL_IN (1 << 22)

//#define GPIO_LOCKED_IO (1 << 19)//锁体不支持

//IO - GND: UNLOCK; IO x GND: LOCK
#define GPIO_DOOR_UNLOCK (1 << 21)//假锁检测 对应sch: J4-PIN6
//#define GPIO_XX_IO (1 << 20)//锁体不支持 


typedef enum{
	IA_HIGH_IB_LOW,
	IA_LOW_IB_HIGH,
	IA_IB_BOTH_LOW,
	IA_IB_BOTH_HIGH
}lg9110_ctrl_t;

typedef enum{
	DOOR_IS_CLOSED,
	DOOR_IS_OPENED
}door_state_t;

void gpio_lg9110_init(void);
void lg9110_ctrl(lg9110_ctrl_t value);

void open_door(void);

void close_door(void);

void door_thread(void);

void test_door(void);


door_state_t get_door_state(void);
void set_door_state(door_state_t state);

void door_signal_in_init(void);
void door_unlock_init(void);

#endif

