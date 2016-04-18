#include "main.h"
#include "stm32dev_boards.h"

void application_ble_init(void);
void application_ble_transmit(char* buffer);
void application_ble_received(void);
void application_ble_parseMessage(void);
