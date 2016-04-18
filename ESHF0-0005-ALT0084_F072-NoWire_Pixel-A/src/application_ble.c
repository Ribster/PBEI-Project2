#include "application_ble.h"
#include "application_general.h"
#include "application_uart.h"

UART_HandleTypeDef application_ble_handle;
__IO ITStatus application_ble_ready = RESET;
uint8_t application_ble_rxbuffer[255];
uint8_t application_ble_charbuffer;
uint8_t application_ble_rxbufferptr = 0;
bool application_ble_messagetoparse = false;

void application_ble_init(void){

  	//	30			AF1	PA9				I/O		USART1_TX		BT_TX			TO DO
  	//	31			AF1	PA10			I/O		USART1_RX		BT_RX			TO DO

	  application_ble_handle.Instance        = USART1;

	  application_ble_handle.Init.BaudRate   = 9600;
	  application_ble_handle.Init.WordLength = UART_WORDLENGTH_8B;
	  application_ble_handle.Init.StopBits   = UART_STOPBITS_1;
	  application_ble_handle.Init.Parity     = UART_PARITY_NONE;
	  application_ble_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  application_ble_handle.Init.Mode       = UART_MODE_TX_RX;
	  application_ble_handle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	  if(HAL_UART_DeInit(&application_ble_handle) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if(HAL_UART_Init(&application_ble_handle) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  /*##-4- Put UART peripheral in reception process ###########################*/
	  if(HAL_UART_Receive_IT(&application_ble_handle, (uint8_t *)&application_ble_charbuffer, 1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  application_ble_transmit("AT+NAMEFixel\n");
}

void application_ble_transmit(char* buffer){

	  if(HAL_UART_Transmit_IT(&application_ble_handle, (uint8_t*)buffer, strlen(buffer))!= HAL_OK)
	  {
	    Error_Handler();
	  }

	  /*##-3- Wait for the end of the transfer ###################################*/
	  while (application_ble_ready != SET)
	  {
	  }

	  /* Reset transmission flag */
	  application_ble_ready = RESET;
}

void application_ble_received(void){
	if(application_ble_charbuffer != '\n'){
		application_ble_rxbuffer[application_ble_rxbufferptr] = application_ble_charbuffer;
		application_ble_rxbufferptr++;

	} else {
		// parse whole message
		//application_ble_parseMessage();
		application_ble_messagetoparse = true;
	}
}

void application_ble_parseMessage(void){
	// parse the whole buffer

	application_uart_transmitSize(application_ble_rxbuffer, application_ble_rxbufferptr);
	application_uart_transmitSize((uint8_t*)"\r\n", 2);

	application_ble_rxbufferptr = 0;

	application_ble_messagetoparse = false;
}
