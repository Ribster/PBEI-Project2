#include "main.h"
#include "stm32dev_boards.h"
#include "application_general.h"



void application_shiftreg_init(void);

void application_shiftreg_timerInterrupt(void);

void application_shiftreg_update(void);

void application_shiftreg_testSequence(void);

void application_shiftreg_setValue_decimal(uint32_t newVal);

void application_shiftreg_setValue_double(uint32_t newVal);

void application_shiftreg_setHex(uint8_t Nr);

void application_shiftreg_addHex(uint8_t Nr);

void application_shiftreg_setChar(char val);

void application_shiftreg_setSegment(uint8_t segNr);

void application_shiftreg_setLed(uint8_t nr, bool value);

void application_shiftreg_setNulout(bool newVal);
