#include "main.h"


int main(void)
{
    volatile unsigned char statues;
	
    SystemInit();
	
    Delay100us(1000);
	SystemCoreClockUpdate(); 
	power_adc_alibration();
	
	GPIOInit();
	
    pcd_Init();
    UART0Init(); 
	
	power_adc_init();
	
	//hwapi07_rf433m_mode3_prepare();

	hwapi08_rf433m_mode0_prepare();

	lock_addr_channel_init();
	gateway_addr_channel_init();
	
	queue_init();

	
#if 1// TDD
	while(1){
		//test_hwapi01_beep_crtl();
		//test_hwapi03_rf433m_power_ctrl();
		//test_uart0_send();
		//test_hwapi03_rf433m_mode();

		//test_hwapi07_rf433m_set_config();
		//test_rf433m_mode0_transport();
		//test_cmd13_update_heartbeat_time();
		//test_echo_gateway_packet_thread();
		
		//test_lock_packet_union();
		//test_hwapi10_ack_error_to_gateway();
		//test_eeprom();//pass
		//handle_gateway_packet_thread();
		//hwapi07_rf433m_get_lock_addr_channel();//fail
		//test_touch();
		//test_power_adc();
		test_power_adc_check();
		
	}
#else
	
	
	statues = 0;
    while(statues == 0)  {                                              /* �ж�SPI�ӿ��Ƿ�����ͨ��      */
         statues = MCU_TO_PCD_TEST();            
    }
    if(statues == TRUE){                                                /* ͨ�������������һ��         */
        hwapi01_beep_crtl(ON);
        Delay100us(10000);
        hwapi01_beep_crtl(OFF);
						
        while (1) {
            //if( (LPC_GPIO_PORT->PIN[0]& CON) == 0 )   {                 /* CON�ӵؽ��봮�ڽ�������      */
              //  UartCommand();                                          /* ��������                     */
            //}
            //else  {                                                     /* ͨ���Զ�Ѱ��                 */
                TyteA_Test();                                           /* ISO14443A�Զ�Ѱ������        */
                //TyteB_Test();                                           /* ISO14443B�Զ�Ѱ������        */
            //}
        }
                
    }

#endif
}

