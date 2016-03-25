/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Inc/main.h
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    26-June-2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32dev_flash_s25fl208.h"
#include "stm32dev_boards.h"
#include <stdbool.h>

#include "application_usb.h"

typedef enum bt_animations {
	bt_animations_rainbow = 1
} bt_animations;

typedef enum bt_colorpatterns {
	bt_colorpatterns_rainbow = 1,
	bt_colorpatterns_monochrome = 2
} bt_colorpatterns;

typedef struct bt_parameters {
	uint32_t bt_rgbcount;
	uint32_t bt_rgbmax;
	uint16_t bt_analog1;
	int8_t bt_temp;
	bool bt_animationSelected;
	uint8_t bt_animationNumber;
	uint8_t bt_colorPattern;
	bool bt_printfEnabled;
} bt_parameters;

typedef struct application_parameters{
	uint32_t MCUVersion;
	bt_parameters flash_btParams;
} application_parameters;



// BLUETOOTH FUNCTION
	extern UART_HandleTypeDef UartHandle;
	#define USARTx                           	USART1
	#define USARTx_CLK_ENABLE()              	__HAL_RCC_USART1_CLK_ENABLE()
	#define DMAx_CLK_ENABLE()                	__HAL_RCC_DMA1_CLK_ENABLE()
	#define USARTx_RX_GPIO_CLK_ENABLE()      	__HAL_RCC_GPIOA_CLK_ENABLE()
	#define USARTx_TX_GPIO_CLK_ENABLE()      	__HAL_RCC_GPIOA_CLK_ENABLE()

	#define USARTx_FORCE_RESET()             	__HAL_RCC_USART1_FORCE_RESET()
	#define USARTx_RELEASE_RESET()           	__HAL_RCC_USART1_RELEASE_RESET()

	/* Definition for USARTx Pins */
	#define USARTx_TX_PIN                    	GPIO_PIN_9
	#define USARTx_TX_GPIO_PORT              	GPIOA
	#define USARTx_TX_AF                     	GPIO_AF1_USART1
	#define USARTx_RX_PIN                    	GPIO_PIN_10
	#define USARTx_RX_GPIO_PORT              	GPIOA
	#define USARTx_RX_AF                     	GPIO_AF1_USART1

	/* Definition for USARTx's DMA */
	#define USARTx_TX_DMA_STREAM              	DMA1_Channel2
	#define USARTx_RX_DMA_STREAM              	DMA1_Channel3

	/* Definition for USARTx's NVIC */
	#define USARTx_DMA_TX_IRQn                	DMA1_Channel2_3_IRQn
	#define USARTx_DMA_RX_IRQn                	DMA1_Channel2_3_IRQn
	#define USARTx_DMA_TX_IRQHandler         	DMA1_Channel2_3_IRQHandler
	#define USARTx_DMA_RX_IRQHandler         	DMA1_Channel2_3_IRQHandler

	/* Definition for USARTx's NVIC */
	#define USARTx_IRQn                      	USART1_IRQn
	#define USARTx_IRQHandler                	USART1_IRQHandler


// PRINTF FUNCTION
	extern UART_HandleTypeDef 						UartHandle_printf;
	#define USARTx_printf                    		USART2
	#define USARTx_printf_CLK_ENABLE()              __HAL_RCC_USART2_CLK_ENABLE()
	#define USARTx_printf_RX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
	#define USARTx_printf_TX_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
	#define USARTx_printf_FORCE_RESET()             __HAL_RCC_USART2_FORCE_RESET()
	#define USARTx_printf_RELEASE_RESET()           __HAL_RCC_USART2_RELEASE_RESET()

	#define USARTx_printf_TX_PIN                    GPIO_PIN_2
	#define USARTx_printf_TX_GPIO_PORT              GPIOA
	#define USARTx_printf_TX_AF                     GPIO_AF1_USART2
	#define USARTx_printf_RX_PIN                    GPIO_PIN_3
	#define USARTx_printf_RX_GPIO_PORT              GPIOA
	#define USARTx_printf_RX_AF                     GPIO_AF1_USART2


// WS2812 LED STRIP
	#define TIMx                             TIM3
	#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM3_CLK_ENABLE
	//#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA1_CLK_ENABLE
	#define TIMx_CHANNEL1_GPIO_CLK_ENABLE    __HAL_RCC_GPIOA_CLK_ENABLE
	#define TIMx_GPIO_CHANNEL1_PORT          GPIOB
	#define GPIO_PIN_CHANNEL1                GPIO_PIN_4
	#define GPIO_AF_TIMx                     GPIO_AF1_TIM3
	#define TIMx_CC1_DMA_INST                DMA1_Channel4
	#define TIMx_DMA_IRQn                    DMA1_Channel4_5_IRQn
	#define TIMx_DMA_IRQHandler              DMA1_Channel4_5_IRQHandler


// SPI FLASH
	#define SPIx                             SPI1
	#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
	#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
	#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
	#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
	#define SPIx_CS_GPIO_CLK_ENABLE()      	 __HAL_RCC_GPIOA_CLK_ENABLE()
	#define SPIx_WP_GPIO_CLK_ENABLE()      	 __HAL_RCC_GPIOB_CLK_ENABLE()
	#define SPIx_HOLD_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()

	#define SPIx_FORCE_RESET()               __HAL_RCC_SPI1_FORCE_RESET()
	#define SPIx_RELEASE_RESET()             __HAL_RCC_SPI1_RELEASE_RESET()

	#define SPIFLASH_HOLD_PIN				GPIO_PIN_0
	#define SPIFLASH_HOLD_PORT				GPIOB
	#define SPIFLASH_HOLD_AF				GPIO_AF0_SPI1

	#define SPIFLASH_WP_PIN					GPIO_PIN_1
	#define SPIFLASH_WP_PORT				GPIOB
	#define SPIFLASH_WP_AF					GPIO_AF0_SPI1

	#define SPIFLASH_CS_PIN					GPIO_PIN_4
	#define SPIFLASH_CS_PORT				GPIOA
	#define SPIFLASH_CS_AF					GPIO_AF0_SPI1

	#define SPIFLASH_SCK_PIN				GPIO_PIN_5
	#define SPIFLASH_SCK_PORT				GPIOA
	#define SPIFLASH_SCK_AF					GPIO_AF0_SPI1

	#define SPIFLASH_MISO_PIN				GPIO_PIN_6
	#define SPIFLASH_MISO_PORT				GPIOA
	#define SPIFLASH_MISO_AF				GPIO_AF0_SPI1

	#define SPIFLASH_MOSI_PIN				GPIO_PIN_7
	#define SPIFLASH_MOSI_PORT				GPIOA
	#define SPIFLASH_MOSI_AF				GPIO_AF0_SPI1


// ADC analog sampling
	#define ADCx                            ADC1
	#define ADCx_CLK_ENABLE()               __HAL_RCC_ADC1_CLK_ENABLE()

	#define ADCx_FORCE_RESET()              __HAL_RCC_ADC1_FORCE_RESET()
	#define ADCx_RELEASE_RESET()            __HAL_RCC_ADC1_RELEASE_RESET()

	/* Definition of ADCx channels */
	#define ADCx_CHANNELa                   ADC_CHANNEL_0
	#define ADCx_CHANNELb                   ADC_CHANNEL_1

	/* Definition of ADCx channels pins */
	#define ADCx_CHANNELa_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
	#define ADCx_CHANNELa_GPIO_PORT         GPIOA
	#define ADCx_CHANNELa_PIN               GPIO_PIN_0

	#define ADCx_CHANNELb_GPIO_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
	#define ADCx_CHANNELb_GPIO_PORT         GPIOA
	#define ADCx_CHANNELb_PIN               GPIO_PIN_1

	/* Definition of ADCx DMA resources */
	#define ADCx_DMA_CLK_ENABLE()           __HAL_RCC_DMA1_CLK_ENABLE()
	#define ADCx_DMA                        DMA1_Channel1

	#define ADCx_DMA_IRQn                   DMA1_Channel1_IRQn
	#define ADCx_DMA_IRQHandler             DMA1_Channel1_IRQHandler

	/* Definition of ADCx NVIC resources */
	#define ADCx_IRQn                       ADC1_COMP_IRQn
	#define ADCx_IRQHandler                 ADC1_COMP_IRQHandler

	/* ADC parameters */
	#define ADCCONVERTEDVALUES_BUFFER_SIZE ((uint32_t)    5)    /* Size of array containing ADC converted values: set to ADC sequencer number of ranks converted, to have a rank in each address */


// LM75
	#define I2Cx                            I2C1
	#define RCC_PERIPHCLK_I2Cx              RCC_PERIPHCLK_I2C1
	#define RCC_I2CxCLKSOURCE_SYSCLK        RCC_I2C1CLKSOURCE_SYSCLK
	#define I2Cx_CLK_ENABLE()               __HAL_RCC_I2C1_CLK_ENABLE()
	#define I2Cx_SDA_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
	#define I2Cx_SCL_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
	#define I2Cx_FORCE_RESET()              __HAL_RCC_I2C1_FORCE_RESET()
	#define I2Cx_RELEASE_RESET()            __HAL_RCC_I2C1_RELEASE_RESET()
	#define I2Cx_SCL_PIN                    GPIO_PIN_6
	#define I2Cx_SCL_GPIO_PORT              GPIOB
	#define I2Cx_SDA_PIN                    GPIO_PIN_7
	#define I2Cx_SDA_GPIO_PORT              GPIOB
	#define I2Cx_SCL_SDA_AF                 GPIO_AF1_I2C1
	#define I2Cx_IRQn                    	I2C1_IRQn
	#define I2Cx_IRQHandler              	I2C1_IRQHandler


// RTC
	#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
	#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */


// PWM TIMER 1
	#define TIMx_PWM1                      		TIM16
	#define TIMx_PWM1_CLK_ENABLE()         		__HAL_RCC_TIM16_CLK_ENABLE()
	#define TIMx_PWM1_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE();
	#define TIMx_PWM1_GPIO_PORT_CHANNEL1        GPIOB
	#define TIMx_PWM1_GPIO_PIN_CHANNEL1         GPIO_PIN_8
	#define TIMx_PWM1_GPIO_AF_CHANNEL1          GPIO_AF2_TIM16


// PWM TIMER 2
	#define TIMx_PWM2                      		TIM17
	#define TIMx_PWM2_CLK_ENABLE()         		__HAL_RCC_TIM17_CLK_ENABLE()
	#define TIMx_PWM2_CHANNEL_GPIO_PORT()       __HAL_RCC_GPIOB_CLK_ENABLE();
	#define TIMx_PWM2_GPIO_PORT_CHANNEL1        GPIOB
	#define TIMx_PWM2_GPIO_PIN_CHANNEL1         GPIO_PIN_9
	#define TIMx_PWM2_GPIO_AF_CHANNEL1          GPIO_AF2_TIM17

// USB
	/* Definition for TIMx clock resources */
	#define TIMx_USB                             TIM14
	#define TIMx_USB_CLK_ENABLE                  __HAL_RCC_TIM14_CLK_ENABLE

	/* Definition for TIMx's NVIC */
	#define TIMx_USB_IRQn                        TIM14_IRQn
	#define TIMx_USB_IRQHandler                  TIM14_IRQHandler

	/* Periodically, the state of the buffer "UserTxBuffer" is checked.
	   The period depends on CDC_POLLING_INTERVAL */
	#define CDC_POLLING_INTERVAL             5 /* in ms. The max is 65 and the min is 1 */

	//extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
