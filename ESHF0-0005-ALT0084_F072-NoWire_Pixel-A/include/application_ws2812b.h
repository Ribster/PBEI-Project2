#include "main.h"
#include "stm32dev_boards.h"

void application_ws2812b_init(void);
uint32_t application_ws2812b_getColor(uint8_t red, uint8_t green, uint8_t blue);
uint32_t application_ws2812b_getWheel(uint8_t wheelPos);
void application_ws2812b_setColorAll_color(uint32_t color);
void application_ws2812b_setColor_color(uint32_t pixelFrom, uint32_t pixelTo, uint32_t color);
void application_ws2812b_setColorOne_color(uint32_t pixel, uint32_t color);
void application_ws2812b_setColorAll(uint8_t r, uint8_t g, uint8_t b);
void application_ws2812b_setColor(uint32_t pixelFrom, uint32_t pixelTo, uint8_t r, uint8_t g, uint8_t b);
void application_ws2812b_setColorOne(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b);
