/*
 * application_printf.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_PRINTF_H_
#define INCLUDE_APPLICATION_PRINTF_H_

#include "main.h"
#include "string.h"
#include "application_flash.h"
#include "application_bluetooth.h"

extern volatile ITStatus UartReady;

void uart_sendPrintf(char* string);

void uart_sendDMA(char* info);
void uart_fullStringReceived(void);
void general_uart_init(void);

#endif /* INCLUDE_APPLICATION_PRINTF_H_ */
