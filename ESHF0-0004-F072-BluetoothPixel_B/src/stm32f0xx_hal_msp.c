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
		  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
		  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);

		  /* NVIC configuration for DMA transfer complete interrupt (USARTx_RX) */
		  HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
		  HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);

		  /* NVIC for USART, to catch the TX complete */
		  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
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
  HAL_NVIC_SetPriority(TIMx_DMA_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIMx_DMA_IRQn);
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
