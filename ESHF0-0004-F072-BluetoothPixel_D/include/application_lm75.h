/*
 * application_lm75.h
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_LM75_H_
#define INCLUDE_APPLICATION_LM75_H_

#include "main.h"
#include "stm32dev_i2c_f0.h"
#include "application_printf.h"

extern I2C_HandleTypeDef I2cHandle;

#define I2C_ADDRESS        0x90

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 48 MHz */
/* This example use TIMING to 0x00A51314 to reach 1 MHz speed (Rise time = 100 ns, Fall time = 100 ns) */
#define I2C_TIMING      0x00A51314

/* LM75 registers */
#define LM75_REG_TEMP                 0x00 // Temperature
#define LM75_REG_CONF                 0x01 // Configuration
#define LM75_REG_THYS                 0x02 // Hysteresis
#define LM75_REG_TOS                  0x03 // Overtemperature shutdown

void lm75_writeReg_8bit(uint8_t reg, uint8_t value);
void lm75_writeReg_16bit(uint8_t reg, uint16_t value);
uint8_t lm75_readReg_8bit(uint8_t reg);
uint16_t lm75_readReg_16bit(uint8_t reg);

void lm75_testRegisters(void);

uint8_t lm75_readConf(void);
uint16_t lm75_readTemp(void);
uint16_t lm75_readHys(void);
uint16_t lm75_readTos(void);

void lm75_writeConf(uint8_t value);
void lm75_writeTemp(uint16_t value);
void lm75_writeHys(uint16_t value);
void lm75_writeTos(uint16_t value);

// application
void lm75_shutdown(FunctionalState newstate);
int16_t lm75_temperature(void);
bool lm75_present(void);

void general_lm75_init(void);

#endif /* INCLUDE_APPLICATION_LM75_H_ */
