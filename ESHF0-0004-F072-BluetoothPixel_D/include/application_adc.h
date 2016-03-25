/*
 * application_adc.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_ADC_H_
#define INCLUDE_APPLICATION_ADC_H_

#include "main.h"
#include "application_printf.h"
#include "application_general.h"

// ADC
extern ADC_HandleTypeDef    AdcHandle;
/* Variable containing ADC conversions results */
extern volatile uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];
/* Variable to report ADC sequencer status */
volatile extern uint8_t         ubSequenceCompleted;     /* Set when all ranks of the sequence have been converted */
/* Variables for ADC conversions results computation to physical values */
extern uint16_t   	uhADCChannelaToDAC_mVolt;
extern uint16_t   	uhADCChannelbToDAC_mVolt;
extern uint16_t		uhVrefInt_mVolt;
extern int32_t		wTemperature_DegreeCelsius;

void general_adc_init(void);
void general_adc_init_inttemp(void);
void adc_sample(void);

uint32_t* adc_getRandomTemperatures(void);
uint16_t adc_getTemperature(void);
uint16_t adc_getVoltageInternal(void);
uint16_t adc_getADC1(void);
uint16_t adc_getADC2(void);

#endif /* INCLUDE_APPLICATION_ADC_H_ */
