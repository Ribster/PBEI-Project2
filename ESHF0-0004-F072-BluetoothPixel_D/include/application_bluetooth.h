/*
 * application_bluetooth.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_BLUETOOTH_H_
#define INCLUDE_APPLICATION_BLUETOOTH_H_

#include "main.h"
#include "stdbool.h"
#include "stddef.h"
#include "string.h"
#include "stdlib.h"

#include "application_ws2812.h"
#include "application_printf.h"

// bluetooth data
extern uint8_t bt_rgbColors[450];

extern bool bt_fullStringReceived;


extern UART_HandleTypeDef UartHandle;

// bt constants
extern const char bt_command_setRGB[];
extern const char bt_command_setRTC[];
extern const char bt_command_setRGBCount[];
extern const char bt_command_setAnimation[];
extern const char bt_command_setPrintfEnabled[];
extern const char bt_command_setColorPattern[];
//const char bt_command_set[] = "set";

extern const char bt_command_getRGB[];
extern const char bt_command_getRTC[];
extern const char bt_command_getRGBCount[];
extern const char bt_command_getRGBMax[];
extern const char bt_command_getAnalog[];
extern const char bt_command_getTemp[];
extern const char bt_command_getAnimation[];
extern const char bt_command_getAnimationOn[];
extern const char bt_command_getFirmwareVersion[];
extern const char bt_command_getPrintfOn[];
extern const char bt_command_getColorPattern[];
extern const char bt_command_getFlashParams[];
//const char bt_command_get[] = "get";

extern const char* bt_commandlist[];

char* bt_getCommandPtr(char* incoming);
void bt_handleIncoming(char* incoming);
void bt_sendFlashParams(void);

void general_bluetooth_init(void);

#endif /* INCLUDE_APPLICATION_BLUETOOTH_H_ */
