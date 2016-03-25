/*
 * application_rng.h
 *
 *  Created on: 8-mrt.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_RNG_H_
#define INCLUDE_APPLICATION_RNG_H_

#include "application_adc.h"
#include "stm32dev_boards.h"

extern CRC_HandleTypeDef   CrcHandle;

void general_crc_init(void);

uint32_t rng_getRandom(void);


#endif /* INCLUDE_APPLICATION_RNG_H_ */
