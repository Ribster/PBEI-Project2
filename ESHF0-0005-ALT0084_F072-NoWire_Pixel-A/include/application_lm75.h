#include "stm32dev_boards.h"
#include "main.h"

void application_lm75_init(void);
bool application_lm75_present(void);
void application_lm75_setPointerReg(uint8_t pointerReg);
void application_lm75_writeReg_8bit(uint8_t reg, uint8_t value);
void application_lm75_writeReg_16bit(uint8_t reg, uint16_t value);
uint8_t application_lm75_readReg_8bit(uint8_t reg);
uint16_t application_lm75_readReg_16bit(uint8_t reg);
uint8_t application_lm75_readConf(void);
uint16_t application_lm75_readTemp(void);
uint16_t application_lm75_readHys(void);
uint16_t application_lm75_readTos(void);
void application_lm75_writeConf(uint8_t value);
void application_lm75_writeTemp(uint16_t value);
void application_lm75_writeHys(uint16_t value);
void application_lm75_writeTos(uint16_t value);
void application_lm75_shutdown(FunctionalState newstate);
int16_t application_lm75_temperature(void);
