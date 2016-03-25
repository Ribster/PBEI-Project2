/*
 * application_rtc.h
 *
 *  Created on: 28-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_RTC_H_
#define INCLUDE_APPLICATION_RTC_H_

#include "main.h"
#include "application_general.h"
#include "application_printf.h"

extern RTC_HandleTypeDef RtcHandle;
extern uint8_t aShowTime[50];
extern uint8_t aShowDate[50];

void rtc_calenderconfig(void);
void rtc_calenderprint(uint8_t *showtime, uint8_t *showdate);

void general_rtc_init(void);

#endif /* INCLUDE_APPLICATION_RTC_H_ */
