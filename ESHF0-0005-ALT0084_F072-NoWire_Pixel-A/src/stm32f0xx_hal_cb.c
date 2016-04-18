#include "stm32dev_boards.h"
#include "main.h"
#include "application_gpio.h"
#include "application_shiftreg.h"
#include "application_ble.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_2)
  {
	  application_gpio_exti_cb();
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle == &application_uart_printf_handle){
		application_uart_printf_ready = SET;
	} else if(UartHandle == &application_ble_handle){
		application_ble_ready = SET;
	}


}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	if(UartHandle == &application_uart_printf_handle){
		application_uart_printf_ready = SET;
	} else if(UartHandle == &application_ble_handle){
		application_ble_ready = SET;
		application_ble_received();
		  if(HAL_UART_Receive_IT(&application_ble_handle, (uint8_t *)&application_ble_charbuffer, 1) != HAL_OK)
		  {
		    Error_Handler();
		  }
	}

}

 void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)
{

}

 void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
 {
   if(htim == &application_shiftreg_timhandle){
	   application_shiftreg_timerInterrupt();
   }
 }
