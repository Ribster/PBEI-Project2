#include "stm32dev_boards.h"
#include "main.h"

void application_uart_init(void);
void application_uart_transmit(char* buffer);
void application_uart_transmitSize(uint8_t* buffer, uint32_t size);
