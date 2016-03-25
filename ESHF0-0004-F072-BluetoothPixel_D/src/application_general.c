/*
 * application_general.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */
#include "application_general.h"

#include "main.h"
#include "string.h"
#include <stdlib.h>

#include "application_bluetooth.h"
#include "application_flash.h"
#include "application_printf.h"
#include "application_ws2812.h"
#include "application_adc.h"
#include "application_lm75.h"
#include "application_rtc.h"
#include "application_rng.h"

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
	uart_sendPrintf("error handler!!\r\n");
  while(1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSI48)
  *            SYSCLK(Hz)                     = 48000000
  *            HCLK(Hz)                       = 48000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            HSI Frequency(Hz)              = 48000000
  *            PREDIV                         = 2
  *            PLLMUL                         = 2
  *            Flash Latency(WS)              = 1
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;
	  RCC_OscInitTypeDef RCC_OscInitStruct;
	  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

	#if defined (USE_USB_CLKSOURCE_CRSHSI48)
	  static RCC_CRSInitTypeDef RCC_CRSInitStruct;
	#endif

	#if defined (USE_USB_CLKSOURCE_CRSHSI48)

	  /* Enable HSI48 Oscillator to be used as system clock source */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
	  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	  /* Select HSI48 as USB clock source */
	  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
	  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_HSI48;
	  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	  /* Select HSI48 as system clock source and configure the HCLK and PCLK1
	  clock dividers */
	  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI48;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	  /*Configure the clock recovery system (CRS)**********************************/

	  /*Enable CRS Clock*/
	  __HAL_RCC_CRS_CLK_ENABLE();

	  /* Default Synchro Signal division factor (not divided) */
	  RCC_CRSInitStruct.Prescaler = RCC_CRS_SYNC_DIV1;

	  /* Set the SYNCSRC[1:0] bits according to CRS_Source value */
	  RCC_CRSInitStruct.Source = RCC_CRS_SYNC_SOURCE_USB;

	  /* HSI48 is synchronized with USB SOF at 1KHz rate */
	  RCC_CRSInitStruct.ReloadValue =  __HAL_RCC_CRS_RELOADVALUE_CALCULATE(48000000, 1000);
	  RCC_CRSInitStruct.ErrorLimitValue = RCC_CRS_ERRORLIMIT_DEFAULT;

	  /* Set the TRIM[5:0] to the default value*/
	  RCC_CRSInitStruct.HSI48CalibrationValue = 0x20;

	  /* Start automatic synchronization */
	  HAL_RCCEx_CRSConfig (&RCC_CRSInitStruct);

	#elif defined (USE_USB_CLKSOURCE_PLL)

	  /* Enable HSE Oscillator and activate PLL with HSE as source
	  PLLCLK = (8 * 6) / 1) = 48 MHz */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
	  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	  HAL_RCC_OscConfig(&RCC_OscInitStruct);

	  /*Select PLL 48 MHz output as USB clock source */
	  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
	  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLLCLK;
	  HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct);

	  /* Select PLL as system clock source and configure the HCLK and PCLK1
	  clock dividers */
	  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1);

	#endif /*USE_USB_CLKSOURCE_CRSHSI48*/
}



void general_gpio_init(void){

	static GPIO_InitTypeDef  GPIO_InitStruct;

	  /* -1- Enable each GPIO Clock (to be able to program the configuration registers) */
	  __HAL_RCC_GPIOF_CLK_ENABLE();

	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  __HAL_RCC_GPIOC_CLK_ENABLE();

	  /* -2- Configure IOs in output push-pull mode to drive external LEDs */
	  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	  GPIO_InitStruct.Pin = GPIO_PIN_13;
	  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_SET);


	  GPIO_InitStruct.Pin = GPIO_PIN_2;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  GPIO_InitStruct.Pin = GPIO_PIN_13;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  GPIO_InitStruct.Pin = GPIO_PIN_14;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);



	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);



	  // input button

	  GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull  = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	  GPIO_InitStruct.Pin = GPIO_PIN_15;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
