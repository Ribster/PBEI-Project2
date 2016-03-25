/*
 * application_printf.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */
#include "application_printf.h"
#include "application_general.h"

UART_HandleTypeDef UartHandle_printf;
uint8_t uart_receivebuffer[200];
uint8_t uart_receivenumber = 0;
uint8_t uart_receiveChar;

volatile ITStatus UartReady = RESET;


void uart_sendPrintf(char* string){
	if(flash_applicationParams.flash_btParams.bt_printfEnabled){
		HAL_UART_Transmit(&UartHandle_printf, (uint8_t*)string, strlen(string), 5000);
	}
}

void uart_sendDMA(char* info){
    /*##-2- Start the transmission process #####################################*/
    /* While the UART in reception process, user can transmit data through
       "aTxBuffer" buffer */
    if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)info, strlen(info))!= HAL_OK)
    {
      Error_Handler();
    }

    /*##-3- Wait for the end of the transfer ###################################*/
    while (UartReady != SET)
    {
    }

    /* Reset transmission flag */
    UartReady = RESET;
}

void uart_fullStringReceived(void){
	// got full string
	uart_sendPrintf("BT: ");
	uart_sendPrintf((char*)uart_receivebuffer);
	uart_sendPrintf("\r\n");

	bt_handleIncoming((char*)uart_receivebuffer);

//	if(strcmp((char*)"red",(char*)uart_receivebuffer) == 0){
//		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
//		ws2812_setColorAll(255,0,0);
//	} else if(strcmp((char*)"green",(char*)uart_receivebuffer) == 0){
//		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
//		ws2812_setColorAll(0,255,0);
//	} else if(strcmp((char*)"blue",(char*)uart_receivebuffer) == 0){
//		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
//		ws2812_setColorAll(0,0,255);
//	}
}


void general_uart_init(void){

	  // UART printf
	  UartHandle_printf.Instance        = USARTx_printf;

	  UartHandle_printf.Init.BaudRate   = 230400;
	  UartHandle_printf.Init.WordLength = UART_WORDLENGTH_8B;
	  UartHandle_printf.Init.StopBits   = UART_STOPBITS_1;
	  UartHandle_printf.Init.Parity     = UART_PARITY_NONE;
	  UartHandle_printf.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  UartHandle_printf.Init.Mode       = UART_MODE_TX_RX;
	  UartHandle_printf.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	  if(HAL_UART_DeInit(&UartHandle_printf) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  if(HAL_UART_Init(&UartHandle_printf) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  uart_sendPrintf("Testing UART @ 230400\r\n");
}
