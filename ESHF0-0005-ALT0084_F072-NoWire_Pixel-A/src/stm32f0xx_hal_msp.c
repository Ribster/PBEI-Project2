#include "main.h"
#include "application_general.h"

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart == &application_uart_printf_handle){
		  GPIO_InitTypeDef  GPIO_InitStruct;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* Enable GPIO TX/RX clock */
		  __HAL_RCC_GPIOA_CLK_ENABLE();

		  /* Enable USARTx clock */
		  __HAL_RCC_USART2_CLK_ENABLE();

		  /*##-2- Configure peripheral GPIO ##########################################*/
		  /* UART TX GPIO pin configuration  */
		  GPIO_InitStruct.Pin       = GPIO_PIN_2;
		  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull      = GPIO_PULLUP;
		  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;

		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		  /* UART RX GPIO pin configuration  */
		  GPIO_InitStruct.Pin = GPIO_PIN_3;
		  GPIO_InitStruct.Alternate = GPIO_AF1_USART2;

		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		  /*##-3- Configure the NVIC for UART ########################################*/
		  /* NVIC for USART */
		  HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
		  HAL_NVIC_EnableIRQ(USART2_IRQn);
	} else if(huart == &application_ble_handle){
		  GPIO_InitTypeDef  GPIO_InitStruct;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* Enable GPIO TX/RX clock */
		  __HAL_RCC_GPIOA_CLK_ENABLE();

		  /* Enable USARTx clock */
		  __HAL_RCC_USART1_CLK_ENABLE();

		  /*##-2- Configure peripheral GPIO ##########################################*/
		  /* UART TX GPIO pin configuration  */
		  GPIO_InitStruct.Pin       = GPIO_PIN_9;
		  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull      = GPIO_PULLUP;
		  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;

		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		  /* UART RX GPIO pin configuration  */
		  GPIO_InitStruct.Pin = GPIO_PIN_10;
		  GPIO_InitStruct.Alternate = GPIO_AF1_USART1;

		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		  /*##-3- Configure the NVIC for UART ########################################*/
		  /* NVIC for USART */
		  HAL_NVIC_SetPriority(USART1_IRQn, 0, 2);
		  HAL_NVIC_EnableIRQ(USART1_IRQn);
	}

  	//	30			AF1	PA9				I/O		USART1_TX		BT_TX			TO DO
  	//	31			AF1	PA10			I/O		USART1_RX		BT_RX			TO DO
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if(huart == &application_uart_printf_handle){
		  /*##-1- Reset peripherals ##################################################*/
		  __HAL_RCC_USART2_FORCE_RESET();
		  __HAL_RCC_USART2_RELEASE_RESET();

		  /*##-2- Disable peripherals and GPIO Clocks #################################*/
		  /* Configure UART Tx as alternate function  */
		  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2);
		  /* Configure UART Rx as alternate function  */
		  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);

		  /*##-3- Disable the NVIC for UART ##########################################*/
		  HAL_NVIC_DisableIRQ(USART2_IRQn);
	} else if(huart == &application_ble_handle){
		  /*##-1- Reset peripherals ##################################################*/
		  __HAL_RCC_USART1_FORCE_RESET();
		  __HAL_RCC_USART1_RELEASE_RESET();

		  /*##-2- Disable peripherals and GPIO Clocks #################################*/
		  /* Configure UART Tx as alternate function  */
		  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
		  /* Configure UART Rx as alternate function  */
		  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

		  /*##-3- Disable the NVIC for UART ##########################################*/
		  HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
	if(hadc == &application_adc_handle){
		  GPIO_InitTypeDef          GPIO_InitStruct;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* ADC Periph clock enable */
		  __HAL_RCC_ADC1_CLK_ENABLE();
		  /* Enable GPIO clock ****************************************/
		  __HAL_RCC_GPIOA_CLK_ENABLE();

		  /*##-2- Configure peripheral GPIO ##########################################*/
		  /* ADC Channel GPIO pin configuration */
		  GPIO_InitStruct.Pin = GPIO_PIN_0;
		  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		  /*##-2- Configure peripheral GPIO ##########################################*/
		  /* ADC Channel GPIO pin configuration */
		  GPIO_InitStruct.Pin = GPIO_PIN_1;
		  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
	if(hadc == &application_adc_handle){
		  /*##-1- Reset peripherals ##################################################*/
			__HAL_RCC_ADC1_FORCE_RESET();
			__HAL_RCC_ADC1_RELEASE_RESET();

		  /*##-2- Disable peripherals and GPIO Clocks ################################*/
		  /* De-initialize the ADC Channel GPIO pin */
		  HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
	}

}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
GPIO_InitTypeDef  GPIO_InitStruct;

  if(hspi == &application_shiftreg_spihandle)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
	  __HAL_RCC_GPIOB_CLK_ENABLE();
    /* Enable SPI clock */
	  __HAL_RCC_SPI2_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = GPIO_PIN_13;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_14;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Alternate = GPIO_AF0_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  } else if (hspi == &application_shiftreg_spihandle){
	  	//	14			AF0	PA4				I/O		SPI1_NSS		SPIFLASH_CS		TO DO
	  	//	15			AF0	PA5				I/O		SPI1_SCK		SPIFLASH_SCK	TO DO
	  	//	17			AF0	PA7				I/O		SPI1_MOSI		SPIFLASH_MOSI	TO DO
	  	//	18				PB0				Output	GPIO_Output		SPIFLASH_WP		TO DO
	  	//	19				PB1				Output	GPIO_Output		SPIFLASH_HOLD	TO DO
	  	//	40			AF0	PB4				I/O		SPI1_MISO		SPIFLASH_MISO	TO DO

	    /*##-1- Enable peripherals and GPIO Clocks #################################*/
	    /* Enable GPIO TX/RX clock */
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOB_CLK_ENABLE();

	    /* Enable SPI clock */
	  __HAL_RCC_SPI1_CLK_ENABLE();

	    /*##-2- Configure peripheral GPIO ##########################################*/
	    /* SPI SCK GPIO pin configuration  */
	    GPIO_InitStruct.Pin       = GPIO_PIN_5;
	    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
	    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
	    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
	    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	    /* SPI MISO GPIO pin configuration  */
	    GPIO_InitStruct.Pin = GPIO_PIN_4;
	    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    /* SPI MOSI GPIO pin configuration  */
	    GPIO_InitStruct.Pin = GPIO_PIN_7;
	    GPIO_InitStruct.Alternate = GPIO_AF0_SPI1;
	    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}


void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi == &application_shiftreg_spihandle)
  {
    /*##-1- Reset peripherals ##################################################*/
	  __HAL_RCC_SPI2_FORCE_RESET();
	  __HAL_RCC_SPI2_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_14);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_15);
  } else if (hspi == &application_shiftreg_spihandle){
	    /*##-1- Reset peripherals ##################################################*/
	  __HAL_RCC_SPI1_FORCE_RESET();
	  __HAL_RCC_SPI1_RELEASE_RESET();

	    /*##-2- Disable peripherals and GPIO Clocks ################################*/
	    /* Configure SPI SCK as alternate function  */
	    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);
	    /* Configure SPI MISO as alternate function  */
	    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_4);
	    /* Configure SPI MOSI as alternate function  */
	    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_7);
  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	if(htim == &application_shiftreg_timhandle){
		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* TIMx Peripheral clock enable */
			__HAL_RCC_TIM6_CLK_ENABLE();

		  /*##-2- Configure the NVIC for TIMx ########################################*/
		  /* Set the TIMx priority */
		  HAL_NVIC_SetPriority(TIM6_IRQn, 3, 0);

		  /* Enable the TIMx global Interrupt */
		  HAL_NVIC_EnableIRQ(TIM6_IRQn);
	}


}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

	if(htim == &application_ws2812b_handle){
		  GPIO_InitTypeDef   GPIO_InitStruct;
		  static DMA_HandleTypeDef  hdma_tim;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* TIMx clock enable */
		  __HAL_RCC_TIM3_CLK_ENABLE();

		  /* Enable GPIO Channel3/3N Clocks */
		  __HAL_RCC_GPIOA_CLK_ENABLE();

		  /* Enable DMA clock */
		  __HAL_RCC_DMA1_CLK_ENABLE();


		  /* Configure TIM1_Channel3 in output, push-pull & alternate function mode */
		  GPIO_InitStruct.Pin = GPIO_PIN_6;
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


		  /* Set the parameters to be configured */
		  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
		  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
		  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
		  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD ;
		  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE ;
		  hdma_tim.Init.Mode = DMA_CIRCULAR;
		  hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;

		  /* Set hdma_tim instance */
		  hdma_tim.Instance = DMA1_Channel4;

		  /* Link hdma_tim to hdma[3] (channel3) */
		  __HAL_LINKDMA(htim, hdma[1], hdma_tim);

		  /* Initialize TIMx DMA handle */
		  HAL_DMA_Init(htim->hdma[1]);

		  /*##-2- Configure the NVIC for DMA #########################################*/
		  /* NVIC configuration for DMA transfer complete interrupt */
		  HAL_NVIC_SetPriority(DMA1_Channel4_5_IRQn, 3, 0);
		  HAL_NVIC_EnableIRQ(DMA1_Channel4_5_IRQn);
	} else if(htim == &application_leddim_handle_1){
		GPIO_InitTypeDef   GPIO_InitStruct;
		__HAL_RCC_TIM16_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		  /* Configure TIM1_Channel3 in output, push-pull & alternate function mode */
		  GPIO_InitStruct.Pin = GPIO_PIN_8;
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF2_TIM16;
		  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	} else if(htim == &application_leddim_handle_2){
		GPIO_InitTypeDef   GPIO_InitStruct;
		__HAL_RCC_TIM17_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		  /* Configure TIM1_Channel3 in output, push-pull & alternate function mode */
		  GPIO_InitStruct.Pin = GPIO_PIN_9;
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF2_TIM17;
		  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	} else if(htim == &application_leddim_handle_3){
		GPIO_InitTypeDef   GPIO_InitStruct;
		__HAL_RCC_TIM2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		  /* Configure TIM1_Channel3 in output, push-pull & alternate function mode */
		  GPIO_InitStruct.Pin = GPIO_PIN_15;
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
		  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	}



		//	45			AF2	PB8				I/O		S_TIM16_CH1		LED_PWM1		TO DO
		//	46			AF2	PB9				I/O		S_TIM17_CH1		LED_PWM2		TO DO
		//	38			AF2	PA15			I/O		S_TIM2_CH1_ETR	LED_PWM3		TO DO
}

void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
	if(hi2c == &application_lm75_handle){
		  GPIO_InitTypeDef  GPIO_InitStruct;
		  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

		  /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
		  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
		  RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
		  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

		  /*##-2- Enable peripherals and GPIO Clocks #################################*/
		  /* Enable GPIO TX/RX clock */
		  __HAL_RCC_GPIOB_CLK_ENABLE();
		  /* Enable I2Cx clock */
		  __HAL_RCC_I2C1_CLK_ENABLE();

		  /*##-3- Configure peripheral GPIO ##########################################*/
		  /* I2C TX GPIO pin configuration  */
		  GPIO_InitStruct.Pin       = GPIO_PIN_6;
		  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
		  GPIO_InitStruct.Pull      = GPIO_PULLUP;
		  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;

		  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		  /* I2C RX GPIO pin configuration  */
		  GPIO_InitStruct.Pin       = GPIO_PIN_7;
		  GPIO_InitStruct.Alternate = GPIO_AF1_I2C1;

		  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		  /*##-4- Configure the NVIC for I2C ########################################*/
		  /* NVIC for I2Cx */
		  HAL_NVIC_SetPriority(I2C1_IRQn, 4, 1);
		  HAL_NVIC_EnableIRQ(I2C1_IRQn);
	}


}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

	if(hi2c == &application_lm75_handle){
		  /*##-1- Reset peripherals ##################################################*/
		__HAL_RCC_I2C1_FORCE_RESET();
		__HAL_RCC_I2C1_RELEASE_RESET();

		  /*##-2- Disable peripherals and GPIO Clocks #################################*/
		  /* Configure I2C Tx as alternate function  */
		  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);
		  /* Configure I2C Rx as alternate function  */
		  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

		    /*##-3- Disable the NVIC for I2C ##########################################*/
		  HAL_NVIC_DisableIRQ(I2C1_IRQn);
	}

}

void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	if(hrtc == &application_rtc_handle){
		  RCC_OscInitTypeDef        RCC_OscInitStruct;
		  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

		  /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
		  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
		     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
		     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
		       configure the RTC clock source (to be done once after reset).
		     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
		       __HAL_RCC_BACKUPRESET_RELEASE().
		     - Configure the needed RTc clock source */
		  __HAL_RCC_PWR_CLK_ENABLE();
		  HAL_PWR_EnableBkUpAccess();

		  /*##-2- Configue LSE as RTC clock soucre ###################################*/
		  RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
		  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
		  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
		  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
		  {
		    Error_Handler();
		  }

		  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
		  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
		  if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		  {
		    Error_Handler();
		  }

		  /*##-3- Enable RTC peripheral Clocks #######################################*/
		  /* Enable RTC Clock */
		  __HAL_RCC_RTC_ENABLE();
	}

}


void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
	if(hrtc == &application_rtc_handle){
		  /*##-1- Reset peripherals ##################################################*/
		  __HAL_RCC_RTC_DISABLE();

		  /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
		  HAL_PWR_DisableBkUpAccess();
		  __HAL_RCC_PWR_CLK_DISABLE();
	}

}
