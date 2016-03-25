/*
 * application_general.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_GENERAL_H_
#define INCLUDE_APPLICATION_GENERAL_H_

#include "stm32dev_boards.h"

extern UART_HandleTypeDef UartHandle_printf;
extern uint8_t uart_receivebuffer[200];
extern uint8_t uart_receivenumber;
extern uint8_t uart_receiveChar;

void Error_Handler(void);

void SystemClock_Config(void);

void general_gpio_init(void);

#endif /* INCLUDE_APPLICATION_GENERAL_H_ */
