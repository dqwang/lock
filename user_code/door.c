#include "main.h"

door_state_t g_door_state = DOOR_IS_CLOSED;

void gpio_lg9110_init(void)
{
	gpio_dir(GPIO_LG9110_IA, GPIO_OUTPUT);
	gpio_dir(GPIO_LG9110_IB, GPIO_OUTPUT);

	gpio_ctrl(GPIO_LG9110_IA,GPIO_LOW);
	gpio_ctrl(GPIO_LG9110_IB,GPIO_LOW);

}


void lg9110_ctrl(lg9110_ctrl_t value)
{
	switch(value){
		case IA_HIGH_IB_LOW:{
			gpio_ctrl(GPIO_LG9110_IA,GPIO_HIGH);
			gpio_ctrl(GPIO_LG9110_IB,GPIO_LOW);
		}break;
		
		case IA_LOW_IB_HIGH:{
			gpio_ctrl(GPIO_LG9110_IA,GPIO_LOW);
			gpio_ctrl(GPIO_LG9110_IB,GPIO_HIGH);
		}break;
		
		case IA_IB_BOTH_LOW:{
			gpio_ctrl(GPIO_LG9110_IA,GPIO_LOW);
			gpio_ctrl(GPIO_LG9110_IB,GPIO_LOW);
		}break;

		case IA_IB_BOTH_HIGH:{
			gpio_ctrl(GPIO_LG9110_IA,GPIO_HIGH);
			gpio_ctrl(GPIO_LG9110_IB,GPIO_HIGH);
		}break;

		default:{

		}break;		
	}
}

void open_door(void)
{
	//debug
	hwapi01_beep_cnt(1,100);

	lg9110_ctrl(IA_LOW_IB_HIGH);
	delay_ms(MOTOR_OPEN_TIME);
	lg9110_ctrl(IA_IB_BOTH_LOW);
}
void close_door(void)
{
	lg9110_ctrl(IA_HIGH_IB_LOW);
	delay_ms(MOTOR_CLOSE_TIME);
	lg9110_ctrl(IA_IB_BOTH_LOW);
}
void door_thread(void)
{
	//timer
}

void test_door(void)
{
	open_door();
	delay_ms(OPEN_DOOR_TIME);
	close_door();
	delay_ms(OPEN_DOOR_TIME);
}

door_state_t get_door_state(void)
{

	return DOOR_IS_CLOSED;
}

void set_door_state(door_state_t state)
{

}



typedef struct pin_int{
	IRQn_Type pin_irq_tpye;//PIN_INT0_IRQn ... PIN_INT7_IRQn
	u8 pintsel_index;//0-7
	u8 pintsel_value;
	u8 pin_isel;//0-7, 0 edge, 1 level
	u8 pin_ienf;//falling
	u8 pin_ienr;//raising
	u8 pin_ist;//
}pin_int_t;

void door_signal_in_init(void)
{
	gpio_dir(GPIO_SIGNAL_IN, GPIO_INPUT);

	/*
	 * PINTSEL
     * 5:0 INTPIN Pin number select for pin interrupt or pattern match engine input.
	 * (PIO0_0 to PIO0_28 correspond to numbers 0 to 28).
	 */
	LPC_SYSCON->PINTSEL[1] = 22;

	/*
	 * ISEL
	 *	7:0 PMODE Selects the interrupt mode for each pin interrupt. Bit n
	 *	configures the pin interrupt selected in PINTSELn.
	 *	0 = Edge sensitive
	 *	1 = Level sensitive
	 */
	
	LPC_PININT->ISEL &= ~0x2;//SET 0, edge

	/*
	 *IENF
	 *0 = Disable falling edge interrupt or set active interrupt level LOW.
	 *1 = Enable falling edge interrupt enabled or set active interrupt level HIGH.
	 */
	LPC_PININT->IENF |= 0x2;

	NVIC_EnableIRQ(PIN_INT1_IRQn);
}

void PIN_INT1_IRQHandler (void)
{
	NVIC_DisableIRQ(PIN_INT1_IRQn);

	/*
	IST
	Pin Interrupt Status registe
	7:0 PSTAT Pin interrupt status. Bit n returns the status, clears the edge
	interrupt, or inverts the active level of the pin selected in
	PINTSELn.
	Read 0: interrupt is not being requested for this interrupt pin.
	Write 0: no operation.
	Read 1: interrupt is being requested for this interrupt pin.
	Write 1 (edge-sensitive): clear rising- and falling-edge
	detection for this pin.
	Write 1 (level-sensitive): switch the active level for this pin (in
	the IENF register).
	*/
	LPC_PININT->IST = 0x2;//clear interrupt status

	hwapi01_beep_cnt(1,100);
	
	NVIC_EnableIRQ(PIN_INT1_IRQn);
}


void door_unlock_init(void)
{
	gpio_dir(GPIO_DOOR_UNLOCK, GPIO_INPUT);

	/*
	 * PINTSEL
     * 5:0 INTPIN Pin number select for pin interrupt or pattern match engine input.
	 * (PIO0_0 to PIO0_28 correspond to numbers 0 to 28).
	 */
	LPC_SYSCON->PINTSEL[2] = 21;

	/*
	 * ISEL
	 *	7:0 PMODE Selects the interrupt mode for each pin interrupt. Bit n
	 *	configures the pin interrupt selected in PINTSELn.
	 *	0 = Edge sensitive
	 *	1 = Level sensitive
	 */
	
	LPC_PININT->ISEL &= ~0x4;//SET 0, edge

	/*
	 *IENF
	 *0 = Disable falling edge interrupt or set active interrupt level LOW.
	 *1 = Enable falling edge interrupt enabled or set active interrupt level HIGH.
	 */
	LPC_PININT->IENF |= 0x4;

	NVIC_EnableIRQ(PIN_INT2_IRQn);
}

void PIN_INT2_IRQHandler (void)
{
	NVIC_DisableIRQ(PIN_INT2_IRQn);

	/*
	IST
	Pin Interrupt Status registe
	7:0 PSTAT Pin interrupt status. Bit n returns the status, clears the edge
	interrupt, or inverts the active level of the pin selected in
	PINTSELn.
	Read 0: interrupt is not being requested for this interrupt pin.
	Write 0: no operation.
	Read 1: interrupt is being requested for this interrupt pin.
	Write 1 (edge-sensitive): clear rising- and falling-edge
	detection for this pin.
	Write 1 (level-sensitive): switch the active level for this pin (in
	the IENF register).
	*/
	LPC_PININT->IST = 0x4;//clear interrupt status

	hwapi01_beep_cnt(1,100);
	
	NVIC_EnableIRQ(PIN_INT2_IRQn);
}




