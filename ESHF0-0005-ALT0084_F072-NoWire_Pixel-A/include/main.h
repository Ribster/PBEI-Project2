#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32dev_flash_s25fl208.h"
#include "stm32dev_boards.h"
#include <stdbool.h>

/* Uncomment the line below to select your USB clock source */
//#define USE_USB_CLKSOURCE_CRSHSI48   1
#define USE_USB_CLKSOURCE_PLL        1

  extern UART_HandleTypeDef application_uart_printf_handle;
  extern __IO ITStatus application_uart_printf_ready;

  extern ADC_HandleTypeDef application_adc_handle;
  /* Variable used to get converted value */
  extern __IO uint16_t application_adc_values[4];

  extern SPI_HandleTypeDef application_shiftreg_spihandle;
  extern TIM_HandleTypeDef application_shiftreg_timhandle;
  extern char application_shiftreg_asciiptr[16];
  extern uint8_t application_shiftreg_decPoint;

  extern I2C_HandleTypeDef application_lm75_handle;

  extern RTC_HandleTypeDef application_rtc_handle;
  extern uint8_t application_rtc_aShowTime[50];
  extern uint8_t application_rtc_aShowDate[50];

  extern TIM_HandleTypeDef application_ws2812b_handle;

  extern SPI_HandleTypeDef application_spiflash_handle;

  extern TIM_HandleTypeDef    application_leddim_handle_1;
  extern TIM_HandleTypeDef    application_leddim_handle_2;
  extern TIM_HandleTypeDef    application_leddim_handle_3;

  extern UART_HandleTypeDef application_ble_handle;
  extern __IO ITStatus application_ble_ready;
  extern uint8_t application_ble_rxbuffer[255];
  extern uint8_t application_ble_charbuffer;
  extern bool application_ble_messagetoparse;

  // GPIO
  	//	Position	AF	Name			Type	Signal			Label			STATE
  	//	1				VBAT			Power
  	//	2				PC13			Output	GPIO_Output		LD_1			DONE
  	//	3				PC14-OSC32_IN	I/O		RCC_OSC32_IN
  	//	4				PC15-OSC32_OUT	I/O		RCC_OSC32_OUT
  	//	5				PF0-OSC_IN		I/O		RCC_OSC_IN
  	//	6				PF1-OSC_OUT		I/O		RCC_OSC_OUT
  	//	7				NRST			I/O
  	//	8				VSSA			Power
  	//	9				VDDA			Power
  	//	10				PA0				I/O		GPIO_Analog		ANA_1			DONE
  	//	11				PA1				I/O		GPIO_Analog		ANA_2			DONE
  	//	12			AF1	PA2				I/O		USART2_TX		PRINTF_TX		DONE
  	//	13			AF1	PA3				I/O		USART2_RX		PRINTF_RX		DONE
  	//	14			AF0	PA4				I/O		SPI1_NSS		SPIFLASH_CS		TO DO
  	//	15			AF0	PA5				I/O		SPI1_SCK		SPIFLASH_SCK	TO DO
  	//	16			AF1	PA6				I/O		S_TIM3_CH1		WS2812B			DONE
  	//	17			AF0	PA7				I/O		SPI1_MOSI		SPIFLASH_MOSI	TO DO
  	//	18				PB0				Output	GPIO_Output		SPIFLASH_WP		TO DO
  	//	19				PB1				Output	GPIO_Output		SPIFLASH_HOLD	TO DO
  	//	20				PB2				I/O		GPXTI2			PB_1			DONE
  	//	21			AF4	PB10			I/O		USART3_TX		ESP_TX
  	//	22			AF4	PB11			I/O		USART3_RX		ESP_RX
  	//	23				VSS				Power
  	//	24				VDD				Power
  	//	25				PB12			Output	GPIO_Output		HC165_LOAD		TO DO
  	//	26			AF0	PB13			I/O		SPI2_SCK		HC_SR_CLK		DONE
  	//	27			AF0	PB14			I/O		SPI2_MISO		HC165_DOUT		DONE
  	//	28			AF0	PB15			I/O		SPI2_MOSI		HC595_DIN		DONE
  	//	29				PA8				Output	GPIO_Output		HC595_LATCH		DONE
  	//	30			AF1	PA9				I/O		USART1_TX		BT_TX			TO DO
  	//	31			AF1	PA10			I/O		USART1_RX		BT_RX			TO DO
  	//	32				PA11			I/O		USB_DM
  	//	33				PA12			I/O		USB_DP
  	//	34				PA13			I/O		SYS_SWDIO
  	//	35				VSS				Power
  	//	36				VDDIO2			Power
  	//	37				PA14			I/O		SYS_SWCLK
  	//	38			AF2	PA15			I/O		S_TIM2_CH1_ETR	LED_PWM3		DONE
  	//	39				PB3				I/O		GPXTI3			nRF_IRQ
  	//	40			AF0	PB4				I/O		SPI1_MISO		SPIFLASH_MISO	TO DO
  	//	41				PB5				Output	GPIO_Output		nRF_CE
  	//	42			AF1	PB6				I/O		I2C1_SCL		LM75_SCL		DONE
  	//	43			AF1	PB7				I/O		I2C1_SDA		LM75_SDA		DONE
  	//	44				BOOT0			Boot
  	//	45			AF2	PB8				I/O		S_TIM16_CH1		LED_PWM1		DONE
  	//	46			AF2	PB9				I/O		S_TIM17_CH1		LED_PWM2		DONE
  	//	47				VSS				Power
  	//	48				VDD				Power

#endif /* __MAIN_H */
