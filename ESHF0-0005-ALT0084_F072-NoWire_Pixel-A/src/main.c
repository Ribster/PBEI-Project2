#include "main.h"
#include "application_general.h"
#include "application_gpio.h"
#include "application_uart.h"
#include "application_adc.h"
#include "application_shiftreg.h"
#include "application_lm75.h"
#include "application_rtc.h"
#include "application_ws2812b.h"
#include "application_spiflash.h"
#include "application_leddim.h"
#include "application_ble.h"

int main(void){

  HAL_Init();

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();
  
  application_gpio_init();
  application_uart_init();
  application_adc_init();
  application_shiftreg_init();
  //application_shiftreg_testSequence();
  application_lm75_init();
  application_rtc_init();
  application_ws2812b_init();
  application_spiflash_init();
  application_leddim_init();
  //application_leddim_testSequence();
  application_ble_init();

  application_uart_transmit("...done\r\n");

  uint8_t counter_adcsensor = 0;
  uint8_t counter_sensorscan = 0;

  bool led = false;

  bool date_time_switch = false;

  while (1)
  {
	  for(int i=0; i<100; i++){
		  if(application_ble_messagetoparse == false){
			  HAL_Delay(1);
		  } else {
			  application_ble_parseMessage();
		  }

	  }



	  application_rtc_calendarshow_shiftreg(application_rtc_aShowTime, application_rtc_aShowDate);


	  led = !led;

	  application_shiftreg_setLed(4, led);

	  if(counter_sensorscan++ >= 100){
		  counter_sensorscan = 0; counter_adcsensor++;
	  }


	  if(counter_sensorscan == 99){
		  date_time_switch = !date_time_switch;
	  }

	  //application_shiftreg_setNulout(true);


	  if(date_time_switch){
		  application_shiftreg_decPoint = 2;
		  memcpy(application_shiftreg_asciiptr, application_rtc_aShowTime, 4);
		  application_rtc_calendarshow(application_rtc_aShowTime, application_rtc_aShowDate);
		  //application_uart_transmit((char*)application_rtc_aShowDate);
		  //application_uart_transmit(" - ");
		  //application_uart_transmit((char*)application_rtc_aShowTime);
	  } else {
		  uint32_t tmp = application_lm75_temperature()*10;
		  application_shiftreg_setValue_double(tmp);
		  //application_uart_transmit(stm32dev_general_getDecSigned(tmp));
	  }


	  //application_uart_transmit("\r\n");

//	  if(counter_adcsensor >= 3) counter_adcsensor = 0;
//	  application_adc_sample();
//
//	  if(counter_adcsensor != 2){
//		  	  application_shiftreg_setValue_decimal(application_adc_values[counter_adcsensor]);
//		  	  application_uart_transmit(stm32dev_general_getDec(application_adc_values[counter_adcsensor]));
//	  } else {
//		  uint32_t tmp = application_lm75_temperature()*10;
//	  	  application_shiftreg_setValue_double(tmp);
//	  	  application_uart_transmit(stm32dev_general_getDecSigned(tmp));
//	  }
//
//
//	  application_uart_transmit("\r\n");
  }

}


