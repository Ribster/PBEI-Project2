/*
 * application_flash.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_FLASH_H_
#define INCLUDE_APPLICATION_FLASH_H_

#include "main.h"
#include "stdbool.h"
#include "application_printf.h"

extern application_parameters flash_applicationParams;

extern SPI_HandleTypeDef SpiHandle;

extern uint8_t FLASH_txbuffer[270];
extern uint8_t FLASH_rxbuffer[270];


void flash_initParams(void);
void flash_synchronizeData(void);
application_parameters flash_readApplicationParams(void);
bt_parameters flash_readBtParams(void);
void flash_writeData(void);

void general_spiFlash_init(void);

#endif /* INCLUDE_APPLICATION_FLASH_H_ */
