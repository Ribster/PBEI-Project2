#include "main.h"
#include "stm32dev_boards.h"

void application_rtc_init(void);
void application_rtc_calenderconfig(void);
void application_rtc_calendarshow(uint8_t *showtime, uint8_t *showdate);
void application_rtc_calendarshow_shiftreg(uint8_t *showtime, uint8_t *showdate);
