/*
 * application_ws2812.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_WS2812_H_
#define INCLUDE_APPLICATION_WS2812_H_

#include "main.h"
#include "application_flash.h"
#include "application_printf.h"

// ws2812 constants
extern const uint8_t ws2812b_lowPWM;
extern const uint8_t ws2812b_highPWM;

extern TIM_HandleTypeDef	TimHandle;

/* Timer Output Compare Configuration Structure declaration */
extern TIM_OC_InitTypeDef	sConfig;

/* Capture Compare buffer */
extern uint8_t aCCValue_Buffer[3640];

/* Timer Period*/
extern uint32_t uhTimerPeriod;

void ws2812_setColorOne(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b);
void ws2812_setColor(uint32_t pixelFrom, uint32_t pixelTo, uint8_t r, uint8_t g, uint8_t b);
void ws2812_setColorAll(uint8_t r, uint8_t g, uint8_t b);

void ws2812_setColorOne_color(uint32_t pixel, uint32_t color);
void ws2812_setColor_color(uint32_t pixelFrom, uint32_t pixelTo, uint32_t color);
void ws2812_setColorAll_color(uint32_t color);

uint32_t ws2812_getWheel(uint8_t wheelPos);
uint32_t ws2812_getColor(uint8_t red, uint8_t green, uint8_t blue);

void general_wsLED_init(void);

#endif /* INCLUDE_APPLICATION_WS2812_H_ */
