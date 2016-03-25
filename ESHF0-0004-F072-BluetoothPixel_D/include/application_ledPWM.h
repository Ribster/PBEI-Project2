/*
 * application_ledPWM.h
 *
 *  Created on: 10-mrt.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_LEDPWM_H_
#define INCLUDE_APPLICATION_LEDPWM_H_

#include "main.h"

extern TIM_HandleTypeDef    TimHandle_LEDPWM1;
extern TIM_HandleTypeDef    TimHandle_LEDPWM2;

void general_ledPWM_init(void);

#endif /* INCLUDE_APPLICATION_LEDPWM_H_ */
