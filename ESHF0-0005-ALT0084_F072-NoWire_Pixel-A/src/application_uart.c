#include "application_uart.h"
#include "application_general.h"

UART_HandleTypeDef application_uart_printf_handle;
__IO ITStatus application_uart_printf_ready = RESET;

void application_uart_init(void){
	  // UART
	  application_uart_printf_handle.Instance        = USART2;

	  application_uart_printf_handle.Init.BaudRate   = 230400;
	  application_uart_printf_handle.Init.WordLength = UART_WORDLENGTH_8B;
	  application_uart_printf_handle.Init.StopBits   = UART_STOPBITS_1;
	  application_uart_printf_handle.Init.Parity     = UART_PARITY_NONE;
	  application_uart_printf_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  application_uart_printf_handle.Init.Mode       = UART_MODE_TX_RX;
	  application_uart_printf_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  if(HAL_UART_DeInit(&application_uart_printf_handle) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if(HAL_UART_Init(&application_uart_printf_handle) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  application_uart_transmit("Testing uart @ 230400\r\n");
}

void application_uart_transmit(char* buffer){

	  if(HAL_UART_Transmit_IT(&application_uart_printf_handle, (uint8_t*)buffer, strlen(buffer))!= HAL_OK)
	  {
	    Error_Handler();
	  }

	  /*##-3- Wait for the end of the transfer ###################################*/
	  while (application_uart_printf_ready != SET)
	  {
	  }

	  /* Reset transmission flag */
	  application_uart_printf_ready = RESET;
}

void application_uart_transmitSize(uint8_t* buffer, uint32_t size){

	  if(HAL_UART_Transmit(&application_uart_printf_handle, buffer, size, 50)!= HAL_OK)
	  {
	    Error_Handler();
	  }

//	  /*##-3- Wait for the end of the transfer ###################################*/
//	  while (application_uart_printf_ready != SET)
//	  {
//	  }
//
//	  /* Reset transmission flag */
//	  application_uart_printf_ready = RESET;
}
