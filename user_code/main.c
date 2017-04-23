#include "main.h"

int main(void)
{
    SystemInit();
	
    Delay100us(1000);
	SystemCoreClockUpdate(); 
	power_adc_alibration();
	
	GPIOInit();
	WKTInit();
	
    pcd_Init();
	ic_check_spi();
	
	UART0Init(); 
	power_adc_init();
	
	//hwapi07_rf433m_mode3_prepare();
	hwapi08_rf433m_mode0_prepare();

	lock_addr_channel_init();
	gateway_addr_channel_init();
	
	queue_init();

	
	iccard_init();
	//test_timer_reset();
	//close_door();

	
	while(1){
		//test_hwapi01_beep_crtl();
		//test_hwapi03_rf433m_power_ctrl();
		//test_uart0_send();
		//test_hwapi03_rf433m_mode();

		//test_hwapi07_rf433m_set_config();
		//test_rf433m_mode0_transport();
		
		//test_echo_gateway_packet_thread();
		
		//test_lock_packet_union();
		//test_hwapi10_ack_error_to_gateway();
		//test_eeprom();//pass
		
		//hwapi07_rf433m_get_lock_addr_channel();//fail
		//test_touch();
		//test_power_adc();
		//test_power_adc_check();
		//test_door();
		//test_timer_thread();

		//test_ic_spi_is_ok();
		
		
		
		//test_iccard_read_id();
		//test_get_iccard_data();

		//test_cmd14_CMD_REPORT_OPENDOOR();

		door_thread();
		door_state_thread();
		handle_gateway_packet_thread();
		iccard_thread();
	}

}

