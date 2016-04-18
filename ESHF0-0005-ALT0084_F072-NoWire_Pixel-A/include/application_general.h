/*
 * application_general.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_GENERAL_H_
#define INCLUDE_APPLICATION_GENERAL_H_

#include "stm32dev_boards.h"

void Error_Handler(void);

void SystemClock_Config(void);

void general_gpio_init(void);

#endif /* INCLUDE_APPLICATION_GENERAL_H_ */
