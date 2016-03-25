/*
 * application_bluetooth.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */
#include "application_bluetooth.h"
#include "application_general.h"
#include "string.h"

// bt constants
const char bt_command_setRGB[] = "setRGB";
const char bt_command_setRTC[] = "setRTC";
const char bt_command_setRGBCount[] = "setRGBCount";
const char bt_command_setAnimation[] = "setAnimation";
const char bt_command_setPrintfEnabled[] = "setPrintfEnabled";
const char bt_command_setColorPattern[] = "setColorPattern";
//const char bt_command_set[] = "set";

const char bt_command_getRGB[] = "getRGB";
const char bt_command_getRTC[] = "getRTC";
const char bt_command_getRGBCount[] = "getRGBCount";
const char bt_command_getRGBMax[] = "getRGBMax";
const char bt_command_getAnalog[] = "getAnalog";
const char bt_command_getTemp[] = "getTemp";
const char bt_command_getAnimation[] = "getAnimation";
const char bt_command_getAnimationOn[] = "getAnimationOn";
const char bt_command_getFirmwareVersion[] = "getFirmwareVersion";
const char bt_command_getPrintfOn[] = "getPrintfOn";
const char bt_command_getColorPattern[] = "getColorPattern";
const char bt_command_getFlashParams[] = "getFlashParams";
//const char bt_command_get[] = "get";

const char* bt_commandlist[] = {
		bt_command_setRGB,
		bt_command_setRTC,
		bt_command_setRGBCount,
		bt_command_setAnimation,
		bt_command_setPrintfEnabled,
		bt_command_setColorPattern,
		bt_command_getRGB,
		bt_command_getRTC,
		bt_command_getRGBCount,
		bt_command_getRGBMax,
		bt_command_getAnalog,
		bt_command_getTemp,
		bt_command_getAnimation,
		bt_command_getAnimationOn,
		bt_command_getFirmwareVersion,
		bt_command_getPrintfOn,
		bt_command_getColorPattern,
		bt_command_getFlashParams
};

// bluetooth data
uint8_t bt_rgbColors[450];

bool bt_fullStringReceived = false;


UART_HandleTypeDef UartHandle;


char* bt_getCommandPtr(char* incoming){
	for(uint8_t i=0; i<COUNTOF(bt_commandlist); i++){
		char* tmpPtr = strstr(incoming,bt_commandlist[i]);
		if(tmpPtr != NULL){
			return (char*)bt_commandlist[i];
		}
	}
	return NULL;
}

void bt_handleIncoming(char* incoming){
	// local vars
    char **tokens;
    size_t numtokens;

	// no space protection
	if(strstr(incoming," ") == NULL){
		return;
	}

	// get command
	char* cmd = bt_getCommandPtr(incoming);
	if(cmd == NULL) return;

	// split string
	tokens = stm32dev_general_strsplit(incoming, "; \t\n", &numtokens);

	if(cmd == bt_command_setRGB){
		if(numtokens == 6){
			int pixelNrFrom = atoi(tokens[1]);
			int pixelNrTo = atoi(tokens[2]);
			int redValue = atoi(tokens[3]);
			int blueValue = atoi(tokens[4]);
			int greenValue = atoi(tokens[5]);

			if(pixelNrFrom == 0 || pixelNrTo == 0){
				ws2812_setColorAll(redValue, blueValue, greenValue);
			} else {
				ws2812_setColor(pixelNrFrom, pixelNrTo, redValue, blueValue, greenValue);
			}
		}
	} else if (cmd == bt_command_getFlashParams){
		bt_sendFlashParams();
	}

	// free everything
	for (size_t i = 1; i < numtokens; i++) {
		free(tokens[i]);
	}
	if (tokens != NULL)
		free(tokens);
}

void bt_sendFlashParams(void){
	uart_sendDMA("Testing the whole command that is too long \n");
}

void general_bluetooth_init(void){

	  // BT

	  UartHandle.Instance        = USARTx;

	  UartHandle.Init.BaudRate   = 9600;
	  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	  UartHandle.Init.StopBits   = UART_STOPBITS_1;
	  UartHandle.Init.Parity     = UART_PARITY_NONE;
	  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	  UartHandle.Init.Mode       = UART_MODE_TX_RX;
	  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

	  if(HAL_UART_Init(&UartHandle) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  uart_sendDMA("AT+NAMEFixel\n");

	  if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)&uart_receiveChar, 1) != HAL_OK){
		  Error_Handler();
	  }

}
