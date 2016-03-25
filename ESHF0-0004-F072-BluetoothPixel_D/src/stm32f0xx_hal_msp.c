/**
  ******************************************************************************
  * @file    UART/UART_TwoBoards_ComDMA/Src/stm32f0xx_hal_msp.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    26-June-2015
  * @brief   HAL MSP module.    
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "application_general.h"
#include "application_ws2812.h"
#include "application_ledPWM.h"
#include "application_usb.h"

/** @addtogroup STM32F0xx_HAL_Examples
  * @{
  */

/** @defgroup UART_TwoBoards_ComDMA
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief UART MSP Initialization 
  *        This function configures the hardware resources used in this example: 
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration  
  *           - DMA configuration for transmission request by peripheral 
  *           - NVIC configuration for DMA interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	if(huart == &UartHandle){

		  static DMA_HandleTypeDef hdma_tx;
		  static DMA_HandleTypeDef hdma_rx;

		  GPIO_InitTypeDef  GPIO_InitStruct;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* Enable GPIO TX/RX clock */
		  USARTx_TX_GPIO_CLK_ENABLE();
		  USARTx_RX_GPIO_CLK_ENABLE();


		  /* Enable USARTx clock */
		  USARTx_CLK_ENABLE();

		  /* Enable DMA clock */
		  DMAx_CLK_ENABLE();

		  /*##-2- Configure peripheral GPIO ##########################################*/
		  /* UART TX GPIO pin configuration  */
		  GPIO_InitStruct.Pin       = USARTx_TX_PIN;
		  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull      = GPIO_PULLUP;
		  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = USARTx_TX_AF;

		  HAL_GPIO_Init(USARTx_TX_GPIO_PORT, &GPIO_InitStruct);

		  /* UART RX GPIO pin configuration  */
		  GPIO_InitStruct.Pin = USARTx_RX_PIN;
		  GPIO_InitStruct.Alternate = USARTx_RX_AF;

		  HAL_GPIO_Init(USARTx_RX_GPIO_PORT, &GPIO_InitStruct);

		  /*##-3- Configure the DMA ##################################################*/
		  /* Configure the DMA handler for Transmission process */
		  hdma_tx.Instance                 = USARTx_TX_DMA_STREAM;
		  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
		  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
		  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
		  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		  hdma_tx.Init.Mode                = DMA_NORMAL;
		  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;

		  HAL_DMA_Init(&hdma_tx);

		  /* Associate the initialized DMA handle to the UART handle */
		  __HAL_LINKDMA(huart, hdmatx, hdma_tx);

		  /* Configure the DMA handler for reception process */
		  hdma_rx.Instance                 = USARTx_RX_DMA_STREAM;
		  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
		  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
		  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
		  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
		  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
		  hdma_rx.Init.Mode                = DMA_CIRCULAR;
		  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;

		  HAL_DMA_Init(&hdma_rx);

		  /* Associate the initialized DMA handle to the the UART handle */
		  __HAL_LINKDMA(huart, hdmarx, hdma_rx);

		  /*##-4- Configure the NVIC for DMA #########################################*/
		  /* NVIC configuration for DMA transfer complete interrupt (USARTx_TX) */
		  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 1, 3);
		  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

		  /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		  HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 1, 2);
		  HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);

		  /* NVIC for USART, to catch the TX complete */
		  HAL_NVIC_SetPriority(USARTx_IRQn, 1, 1);
		  HAL_NVIC_EnableIRQ(USARTx_IRQn);
	} else if (huart == &UartHandle_printf){
		GPIO_InitTypeDef  GPIO_InitStruct;

				  /*##-1- Enable peripherals and GPIO Clocks #################################*/
				  /* Enable GPIO TX/RX clock */
				  USARTx_printf_TX_GPIO_CLK_ENABLE();
				  USARTx_printf_RX_GPIO_CLK_ENABLE();


				  /* Enable USARTx clock */
				  USARTx_printf_CLK_ENABLE();

				  /*##-2- Configure peripheral GPIO ##########################################*/
				  /* UART TX GPIO pin configuration  */
				  GPIO_InitStruct.Pin       = USARTx_printf_TX_PIN;
				  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
				  GPIO_InitStruct.Pull      = GPIO_PULLUP;
				  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
				  GPIO_InitStruct.Alternate = USARTx_printf_TX_AF;

				  HAL_GPIO_Init(USARTx_printf_TX_GPIO_PORT, &GPIO_InitStruct);

				  /* UART RX GPIO pin configuration  */
				  GPIO_InitStruct.Pin = USARTx_printf_RX_PIN;
				  GPIO_InitStruct.Alternate = USARTx_printf_RX_AF;

				  HAL_GPIO_Init(USARTx_printf_RX_GPIO_PORT, &GPIO_InitStruct);
	}

}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{

	if(huart == &UartHandle){
		  /*##-1- Reset peripherals ##################################################*/
		  USARTx_FORCE_RESET();
		  USARTx_RELEASE_RESET();

		  /*##-2- Disable peripherals and GPIO Clocks #################################*/
		  /* Configure UART Tx as alternate function  */
		  HAL_GPIO_DeInit(USARTx_TX_GPIO_PORT, USARTx_TX_PIN);
		  /* Configure UART Rx as alternate function  */
		  HAL_GPIO_DeInit(USARTx_RX_GPIO_PORT, USARTx_RX_PIN);

		  /*##-3- Disable the DMA #####################################################*/
		  /* De-Initialize the DMA channel associated to reception process */
		  if(huart->hdmarx != 0)
		  {
		    HAL_DMA_DeInit(huart->hdmarx);
		  }
		  /* De-Initialize the DMA channel associated to transmission process */
		  if(huart->hdmatx != 0)
		  {
		    HAL_DMA_DeInit(huart->hdmatx);
		  }

		  /*##-4- Disable the NVIC for DMA ###########################################*/
		  HAL_NVIC_DisableIRQ(USARTx_DMA_TX_IRQn);
		  HAL_NVIC_DisableIRQ(USARTx_DMA_RX_IRQn);
	} else if (huart == &UartHandle_printf){
		  /*##-1- Reset peripherals ##################################################*/
		  USARTx_printf_FORCE_RESET();
		  USARTx_printf_RELEASE_RESET();

		  /*##-2- Disable peripherals and GPIO Clocks #################################*/
		  /* Configure UART Tx as alternate function  */
		  HAL_GPIO_DeInit(USARTx_printf_TX_GPIO_PORT, USARTx_printf_TX_PIN);
		  /* Configure UART Rx as alternate function  */
		  HAL_GPIO_DeInit(USARTx_printf_RX_GPIO_PORT, USARTx_printf_RX_PIN);
	}

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{

	if(htim == &TimHandle){
		  GPIO_InitTypeDef   GPIO_InitStruct;
		  static DMA_HandleTypeDef  hdma_tim;

		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* TIMx clock enable */
		  TIMx_CLK_ENABLE();

		  /* Enable GPIO Channel3/3N Clocks */
		  TIMx_CHANNEL1_GPIO_CLK_ENABLE();

		  /* Enable DMA clock */
		  DMAx_CLK_ENABLE();


		  /* Configure TIM1_Channel3 in output, push-pull & alternate function mode */
		  GPIO_InitStruct.Pin = GPIO_PIN_CHANNEL1;
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		  GPIO_InitStruct.Alternate = GPIO_AF_TIMx;
		  HAL_GPIO_Init(TIMx_GPIO_CHANNEL1_PORT, &GPIO_InitStruct);


		  /* Set the parameters to be configured */
		  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;
		  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;
		  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;
		  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD ;
		  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE ;
		  hdma_tim.Init.Mode = DMA_CIRCULAR;
		  hdma_tim.Init.Priority = DMA_PRIORITY_HIGH;

		  /* Set hdma_tim instance */
		  hdma_tim.Instance = TIMx_CC1_DMA_INST;

		  /* Link hdma_tim to hdma[3] (channel3) */
		  __HAL_LINKDMA(htim, hdma[1], hdma_tim);

		  /* Initialize TIMx DMA handle */
		  HAL_DMA_Init(htim->hdma[1]);

		  /*##-2- Configure the NVIC for DMA #########################################*/
		  /* NVIC configuration for DMA transfer complete interrupt */
		  HAL_NVIC_SetPriority(TIMx_DMA_IRQn, 2, 0);
		  HAL_NVIC_EnableIRQ(TIMx_DMA_IRQn);
	} else if(htim == &TimHandle_LEDPWM1){
		  GPIO_InitTypeDef   GPIO_InitStruct;
		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* TIMx Peripheral clock enable */
		  TIMx_PWM1_CLK_ENABLE();

		  /* Enable GPIO Channels Clock */
		  TIMx_PWM1_CHANNEL_GPIO_PORT();

		  /* Configure PA.05 (TIM2_Channel1), PB.03 (TIM2_Channel2), PB.10 (TIM2_Channel3),
		     PB.11 (TIM2_Channel4) in output, push-pull, alternate function mode
		  */
		  /* Common configuration for all channels */
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		  GPIO_InitStruct.Alternate = TIMx_PWM1_GPIO_AF_CHANNEL1;
		  GPIO_InitStruct.Pin = TIMx_PWM1_GPIO_PIN_CHANNEL1;
		  HAL_GPIO_Init(TIMx_PWM1_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
	} else if(htim == &TimHandle_LEDPWM2){
		  GPIO_InitTypeDef   GPIO_InitStruct;
		  /*##-1- Enable peripherals and GPIO Clocks #################################*/
		  /* TIMx Peripheral clock enable */
		  TIMx_PWM2_CLK_ENABLE();

		  /* Enable GPIO Channels Clock */
		  TIMx_PWM2_CHANNEL_GPIO_PORT();

		  /* Configure PA.05 (TIM2_Channel1), PB.03 (TIM2_Channel2), PB.10 (TIM2_Channel3),
		     PB.11 (TIM2_Channel4) in output, push-pull, alternate function mode
		  */
		  /* Common configuration for all channels */
		  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		  GPIO_InitStruct.Pull = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		  GPIO_InitStruct.Alternate = TIMx_PWM2_GPIO_AF_CHANNEL1;
		  GPIO_InitStruct.Pin = TIMx_PWM2_GPIO_PIN_CHANNEL1;
		  HAL_GPIO_Init(TIMx_PWM2_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
	}


}

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
GPIO_InitTypeDef  GPIO_InitStruct;

  if(hspi->Instance == SPIx)
  {
    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    SPIx_SCK_GPIO_CLK_ENABLE();
    SPIx_MISO_GPIO_CLK_ENABLE();
    SPIx_MOSI_GPIO_CLK_ENABLE();
    /* Enable SPI clock */
    SPIx_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/
    /* SPI SCK GPIO pin configuration  */
    GPIO_InitStruct.Pin       = SPIFLASH_SCK_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = SPIFLASH_SCK_AF;
    HAL_GPIO_Init(SPIFLASH_SCK_PORT, &GPIO_InitStruct);

    /* SPI MISO GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIFLASH_MISO_PIN;
    GPIO_InitStruct.Alternate = SPIFLASH_MISO_AF;
    HAL_GPIO_Init(SPIFLASH_MISO_PORT, &GPIO_InitStruct);

    /* SPI MOSI GPIO pin configuration  */
    GPIO_InitStruct.Pin = SPIFLASH_MOSI_PIN;
    GPIO_InitStruct.Alternate = SPIFLASH_MOSI_AF;
    HAL_GPIO_Init(SPIFLASH_MOSI_PORT, &GPIO_InitStruct);
  }
}


void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
  if(hspi->Instance == SPIx)
  {
    /*##-1- Reset peripherals ##################################################*/
    SPIx_FORCE_RESET();
    SPIx_RELEASE_RESET();

    /*##-2- Disable peripherals and GPIO Clocks ################################*/
    /* Configure SPI SCK as alternate function  */
    HAL_GPIO_DeInit(SPIFLASH_SCK_PORT, SPIFLASH_SCK_PIN);
    /* Configure SPI MISO as alternate function  */
    HAL_GPIO_DeInit(SPIFLASH_MISO_PORT, SPIFLASH_MISO_PIN);
    /* Configure SPI MOSI as alternate function  */
    HAL_GPIO_DeInit(SPIFLASH_MOSI_PORT, SPIFLASH_MOSI_PIN);
  }
}

void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
  GPIO_InitTypeDef          GPIO_InitStruct;
  static DMA_HandleTypeDef  DmaHandle;
  RCC_OscInitTypeDef        RCC_OscInitStructure;

  /*##-1- Enable peripherals and GPIO Clocks #################################*/
  /* Enable clock of GPIO associated to the peripheral channels */
  ADCx_CHANNELa_GPIO_CLK_ENABLE();
  ADCx_CHANNELb_GPIO_CLK_ENABLE();

  /* Enable clock of ADCx peripheral */
  ADCx_CLK_ENABLE();

  /* Note: In case of usage of asynchronous clock derived from ADC dedicated  */
  /*       HSI RC oscillator 14MHz, with ADC setting                          */
  /*       "AdcHandle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1",                 */
  /*       the clock source has to be enabled at RCC top level using function */
  /*       "HAL_RCC_OscConfig()" (see comments in stm32l1_hal_adc.c header)   */

  /* Enable asynchronous clock source of ADCx */
  /* (place oscillator HSI14 under control of the ADC) */
  HAL_RCC_GetOscConfig(&RCC_OscInitStructure);
  RCC_OscInitStructure.OscillatorType = RCC_OSCILLATORTYPE_HSI14;
  RCC_OscInitStructure.HSI14CalibrationValue = RCC_HSI14CALIBRATION_DEFAULT;
  RCC_OscInitStructure.HSI14State = RCC_HSI14_ADC_CONTROL;
  HAL_RCC_OscConfig(&RCC_OscInitStructure);

  /* Enable clock of DMA associated to the peripheral */
  ADCx_DMA_CLK_ENABLE();

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure GPIO pin of the selected ADC channel */
  GPIO_InitStruct.Pin = ADCx_CHANNELa_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNELa_GPIO_PORT, &GPIO_InitStruct);

  /*##-2- Configure peripheral GPIO ##########################################*/
  /* Configure GPIO pin of the selected ADC channel */
  GPIO_InitStruct.Pin = ADCx_CHANNELb_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADCx_CHANNELb_GPIO_PORT, &GPIO_InitStruct);

  /*##-3- Configure the DMA ##################################################*/
  /* Configure DMA parameters */
  DmaHandle.Instance = ADCx_DMA;

  DmaHandle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  DmaHandle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DmaHandle.Init.MemInc              = DMA_MINC_ENABLE;
  DmaHandle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;   /* Transfer from ADC by half-word to match with ADC configuration: ADC resolution 10 or 12 bits */
  DmaHandle.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;   /* Transfer to memory by half-word to match with buffer variable type: half-word */
  DmaHandle.Init.Mode                = DMA_CIRCULAR;              /* DMA in circular mode to match with ADC configuration: DMA continuous requests */
  DmaHandle.Init.Priority            = DMA_PRIORITY_HIGH;

  /* Deinitialize  & Initialize the DMA for new transfer */
  HAL_DMA_DeInit(&DmaHandle);
  HAL_DMA_Init(&DmaHandle);

  /* Associate the initialized DMA handle to the ADC handle */
  __HAL_LINKDMA(hadc, DMA_Handle, DmaHandle);

  /*##-4- Configure the NVIC #################################################*/

  /* NVIC configuration for DMA interrupt (transfer completion or error) */
  /* Priority: high-priority */
  HAL_NVIC_SetPriority(ADCx_DMA_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(ADCx_DMA_IRQn);


  /* NVIC configuration for ADC interrupt */
  /* Priority: high-priority */
  HAL_NVIC_SetPriority(ADCx_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ADCx_IRQn);
}

/**
  * @brief ADC MSP de-initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable clock of ADC peripheral
  *          - Revert GPIO associated to the peripheral channels to their default state
  *          - Revert DMA associated to the peripheral to its default state
  *          - Revert NVIC associated to the peripheral interruptions to its default state
  * @param hadc: ADC handle pointer
  * @retval None
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
  /*##-1- Reset peripherals ##################################################*/
  ADCx_FORCE_RESET();
  ADCx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks ################################*/
  /* De-initialize GPIO pin of the selected ADC channel */
  HAL_GPIO_DeInit(ADCx_CHANNELa_GPIO_PORT, ADCx_CHANNELa_PIN);

  /*##-3- Disable the DMA ####################################################*/
  /* De-Initialize the DMA associated to the peripheral */
  if(hadc->DMA_Handle != NULL)
  {
    HAL_DMA_DeInit(hadc->DMA_Handle);
  }

  /*##-4- Disable the NVIC ###################################################*/
  /* Disable the NVIC configuration for DMA interrupt */
  HAL_NVIC_DisableIRQ(ADCx_DMA_IRQn);

  /* Disable the NVIC configuration for ADC interrupt */
  HAL_NVIC_DisableIRQ(ADCx_IRQn);
}


/**
  * @brief I2C MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - DMA configuration for transmission request by peripheral
  *           - NVIC configuration for DMA interrupt request enable
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  RCC_PeriphCLKInitTypeDef  RCC_PeriphCLKInitStruct;

  /*##-1- Configure the I2C clock source. The clock is derived from the SYSCLK #*/
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2Cx;
  RCC_PeriphCLKInitStruct.I2c1ClockSelection = RCC_I2CxCLKSOURCE_SYSCLK;
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);

  /*##-2- Enable peripherals and GPIO Clocks #################################*/
  /* Enable GPIO TX/RX clock */
  I2Cx_SCL_GPIO_CLK_ENABLE();
  I2Cx_SDA_GPIO_CLK_ENABLE();
  /* Enable I2Cx clock */
  I2Cx_CLK_ENABLE();

  /*##-3- Configure peripheral GPIO ##########################################*/
  /* I2C TX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_HIGH;
  GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;

  HAL_GPIO_Init(I2Cx_SCL_GPIO_PORT, &GPIO_InitStruct);

  /* I2C RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = I2Cx_SDA_PIN;
  GPIO_InitStruct.Alternate = I2Cx_SCL_SDA_AF;

  HAL_GPIO_Init(I2Cx_SDA_GPIO_PORT, &GPIO_InitStruct);

  /*##-4- Configure the NVIC for I2C ########################################*/
  /* NVIC for I2Cx */
  HAL_NVIC_SetPriority(I2Cx_IRQn, 4, 1);
  HAL_NVIC_EnableIRQ(I2Cx_IRQn);
}

/**
  * @brief I2C MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO, DMA and NVIC configuration to their default state
  * @param hi2c: I2C handle pointer
  * @retval None
  */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{

  /*##-1- Reset peripherals ##################################################*/
  I2Cx_FORCE_RESET();
  I2Cx_RELEASE_RESET();

  /*##-2- Disable peripherals and GPIO Clocks #################################*/
  /* Configure I2C Tx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SCL_GPIO_PORT, I2Cx_SCL_PIN);
  /* Configure I2C Rx as alternate function  */
  HAL_GPIO_DeInit(I2Cx_SDA_GPIO_PORT, I2Cx_SDA_PIN);

    /*##-3- Disable the NVIC for I2C ##########################################*/
  HAL_NVIC_DisableIRQ(I2Cx_IRQn);
}

/**
  * @brief RTC MSP Initialization
  *        This function configures the hardware resources used in this example
  * @param hrtc: RTC handle pointer
  *
  * @note  Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
  *        the RTC clock source; in this case the Backup domain will be reset in
  *        order to modify the RTC Clock source, as consequence RTC registers (including
  *        the backup registers) and RCC_BDCR register are set to their reset values.
  *
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
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

/**
  * @brief RTC MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();

  /*##-2- Disables the PWR Clock and Disables access to the backup domain ###################################*/
  HAL_PWR_DisableBkUpAccess();
  __HAL_RCC_PWR_CLK_DISABLE();
}

/**
  * @brief CRC MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  * @param hcrc: CRC handle pointer
  * @retval None
  */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
  /* CRC Peripheral clock enable */
  __HAL_RCC_CRC_CLK_ENABLE();
}

/**
  * @brief CRC MSP De-Initialization
  *        This function freeze the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  * @param hcrc: CRC handle pointer
  * @retval None
  */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
  /* CRC Peripheral clock disable */
  __HAL_RCC_CRC_CLK_DISABLE();
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
