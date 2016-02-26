/**
  ******************************************************************************
  * @file    GPIO/GPIO_IOToggle/Src/main.c
  * @author  MCD Application Team
  * @version V1.3.0
  * @date    26-June-2015
  * @brief   This example describes how to configure and use GPIOs through
  *          the STM32F0xx HAL API.
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
#include "string.h"
#include <stdlib.h>

// bt constants
const char bt_command_setRGB[] = "setRGB";
const char bt_command_setRTC[] = "setRTC";
const char bt_command_setRGBCount[] = "setRGBCount";
const char bt_command_setAnimation[] = "setAnimation";
const char bt_command_setPrintfEnabled[] = "setPrintfEnabled";
const char bt_command_setColorPattern[] = "setColorPattern";
//const char bt_command_set[] = "set";

const char bt_command_getRGB[] = "getRGB";
const char bt_command_getRTC[] = "getRTC";
const char bt_command_getRGBCount[] = "getRGBCount";
const char bt_command_getRGBMax[] = "getRGBMax";
const char bt_command_getAnalog[] = "getAnalog";
const char bt_command_getTemp[] = "getTemp";
const char bt_command_getAnimation[] = "getAnimation";
const char bt_command_getAnimationOn[] = "getAnimationOn";
const char bt_command_getFirmwareVersion[] = "getFirmwareVersion";
const char bt_command_getPrintfOn[] = "getPrintfOn";
const char bt_command_getColorPattern[] = "getColorPattern";
const char bt_command_getFlashParams[] = "getFlashParams";
//const char bt_command_get[] = "get";

const char* bt_commandlist[] = {
		bt_command_setRGB,
		bt_command_setRTC,
		bt_command_setRGBCount,
		bt_command_setAnimation,
		bt_command_setPrintfEnabled,
		bt_command_setColorPattern,
		bt_command_getRGB,
		bt_command_getRTC,
		bt_command_getRGBCount,
		bt_command_getRGBMax,
		bt_command_getAnalog,
		bt_command_getTemp,
		bt_command_getAnimation,
		bt_command_getAnimationOn,
		bt_command_getFirmwareVersion,
		bt_command_getPrintfOn,
		bt_command_getColorPattern,
		bt_command_getFlashParams
};



// ws2812 constants
const uint8_t ws2812b_lowPWM = 20;
const uint8_t ws2812b_highPWM = 41;

// flash data
application_parameters flash_applicationParams;


void flash_initParams(void);
void flash_synchronizeData(void);
application_parameters flash_readApplicationParams(void);
bt_parameters flash_readBtParams(void);
void flash_writeData(void);

// bluetooth data
uint8_t bt_rgbColors[450];

bool bt_fullStringReceived = false;


UART_HandleTypeDef UartHandle;

char* bt_getCommandPtr(char* incoming);
void bt_handleIncoming(char* incoming);
void bt_sendFlashParams(void);

static GPIO_InitTypeDef  GPIO_InitStruct;
/* UART handler declaration */
UART_HandleTypeDef UartHandle_printf;
__IO ITStatus UartReady = RESET;

static void SystemClock_Config(void);
static void Error_Handler(void);

void uart_sendPrintf(char* string);

void uart_sendDMA(char* info);
void uart_fullStringReceived(void);

uint8_t uart_receivebuffer[200];
uint8_t uart_receivenumber = 0;
uint8_t uart_receiveChar;

void ws2812_setColorOne(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b);
inline void ws2812_setColor(uint32_t pixelFrom, uint32_t pixelTo, uint8_t r, uint8_t g, uint8_t b);
void ws2812_setColorAll(uint8_t r, uint8_t g, uint8_t b);

void ws2812_setColorOne_color(uint32_t pixel, uint32_t color);
void ws2812_setColor_color(uint32_t pixelFrom, uint32_t pixelTo, uint32_t color);
void ws2812_setColorAll_color(uint32_t color);

uint32_t ws2812_getWheel(uint8_t wheelPos);
uint32_t ws2812_getColor(uint8_t red, uint8_t green, uint8_t blue);



TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Capture Compare buffer */
uint8_t aCCValue_Buffer[3640];

/* Timer Period*/
uint32_t uhTimerPeriod  = 0;


SPI_HandleTypeDef SpiHandle;

uint8_t FLASH_txbuffer[530];
uint8_t FLASH_rxbuffer[530];




/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /* This sample code shows how to use GPIO HAL API to toggle LED3, LED4, LED5 and LED6 IOs
    in an infinite loop. */

  /* STM32F0xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 48 MHz */
  SystemClock_Config();
  

  flash_initParams();



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






  // UART printf
  UartHandle_printf.Instance        = USARTx_printf;

  UartHandle_printf.Init.BaudRate   = 230400;
  UartHandle_printf.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle_printf.Init.StopBits   = UART_STOPBITS_1;
  UartHandle_printf.Init.Parity     = UART_PARITY_NONE;
  UartHandle_printf.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle_printf.Init.Mode       = UART_MODE_TX_RX;
  UartHandle_printf.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if(HAL_UART_DeInit(&UartHandle_printf) != HAL_OK)
  {
    Error_Handler();
  }
  if(HAL_UART_Init(&UartHandle_printf) != HAL_OK)
  {
    Error_Handler();
  }

  uart_sendPrintf("Testing UART @ 230400\r\n");




  // BT

  UartHandle.Instance        = USARTx;

  UartHandle.Init.BaudRate   = 9600;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits   = UART_STOPBITS_1;
  UartHandle.Init.Parity     = UART_PARITY_NONE;
  UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  UartHandle.Init.Mode       = UART_MODE_TX_RX;
  UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  if(HAL_UART_Init(&UartHandle) != HAL_OK)
  {
    Error_Handler();
  }

  uart_sendDMA("AT+NAMEFixel\n");

  if (HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)&uart_receiveChar, 1) != HAL_OK){
	  Error_Handler();
  }



  memset(aCCValue_Buffer, 0, 3640);
  ws2812_setColorAll(0,0,0);


  /* Compute the value of ARR regiter to generate signal frequency at 17.57 Khz */
  uhTimerPeriod = (uint32_t)((SystemCoreClock / 800000) - 1);
//  /* Compute CCR1 value to generate a duty cycle at 75% */
//  aCCValue_Buffer[0] = (uint32_t)(((uint32_t) 75 * (uhTimerPeriod - 1)) / 100);
//  /* Compute CCR2 value to generate a duty cycle at 50% */
//  aCCValue_Buffer[1] = (uint32_t)(((uint32_t) 50 * (uhTimerPeriod - 1)) / 100);
//  /* Compute CCR3 value to generate a duty cycle at 25% */
//  aCCValue_Buffer[2] = (uint32_t)(((uint32_t) 25 * (uhTimerPeriod - 1)) / 100);


  TimHandle.Instance = TIMx;

    TimHandle.Init.Period            = uhTimerPeriod;
    TimHandle.Init.RepetitionCounter = 3640;
    TimHandle.Init.Prescaler         = 0;
    TimHandle.Init.ClockDivision     = 0;
    TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
    if (HAL_TIM_PWM_Init(&TimHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }


    /*##-2- Configure the PWM channel 3 ########################################*/
    sConfig.OCMode       = TIM_OCMODE_PWM1;
    sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
    sConfig.Pulse        = aCCValue_Buffer[0];
    sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
    sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
    sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
    sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
    if (HAL_TIM_PWM_ConfigChannel(&TimHandle, &sConfig, TIM_CHANNEL_1) != HAL_OK)
    {
      /* Configuration Error */
      Error_Handler();
    }



    /*##-3- Start PWM signal generation in DMA mode ############################*/
    if (HAL_TIM_PWM_Start_DMA(&TimHandle, TIM_CHANNEL_1, (uint32_t*)aCCValue_Buffer, 3640) != HAL_OK)
    {

      /* Starting Error */
      Error_Handler();
    }




    SpiHandle.Instance               = SPIx;

    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    SpiHandle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
    SpiHandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
    SpiHandle.Init.Mode = SPI_MODE_MASTER;

    HAL_Delay(1000);

    uart_sendPrintf("Starting SPI flash init\r\n");

    if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    // init the spi flash
    	SPIx_CS_GPIO_CLK_ENABLE();
    	SPIx_WP_GPIO_CLK_ENABLE();
    	SPIx_HOLD_GPIO_CLK_ENABLE();

		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		// init the HOLD
		GPIO_InitStruct.Pin = SPIFLASH_HOLD_PIN;
		HAL_GPIO_Init(SPIFLASH_HOLD_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(SPIFLASH_HOLD_PORT,SPIFLASH_HOLD_PIN,GPIO_PIN_SET);

    	// init the CS
		GPIO_InitStruct.Pin = SPIFLASH_CS_PIN;
		HAL_GPIO_Init(SPIFLASH_CS_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(SPIFLASH_CS_PORT,SPIFLASH_CS_PIN,GPIO_PIN_SET);

    	// init the WP
		GPIO_InitStruct.Pin = SPIFLASH_WP_PIN;
		HAL_GPIO_Init(SPIFLASH_WP_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(SPIFLASH_WP_PORT,SPIFLASH_WP_PIN,GPIO_PIN_SET);




	stm32dev_flash_s25fl208_setPrintfRoutine(uart_sendPrintf);
    stm32dev_flash_s25fl208_init(&SpiHandle, FLASH_rxbuffer, FLASH_txbuffer, 530, 530, SPIFLASH_CS_PORT, SPIFLASH_CS_PIN);

    stm32dev_flash_s25fl208_sendCommand_releaseDeepPowerDown();
    stm32dev_flash_s25fl208_sendCommand_writeStatusRegister(0x00);
    stm32dev_flash_s25fl208_print_deviceInfo(stm32dev_flash_s25fl208_sendCommand_readDeviceIdentifiers());
    stm32dev_flash_s25fl208_print_statusRegister(stm32dev_flash_s25fl208_sendCommand_readStatusRegister());
    //stm32dev_flash_s25fl208_testSPI();
    //stm32dev_flash_s25fl208_sendCommand_chipErase();
    stm32dev_flash_s25fl208_waitForProgrammingDone();

    flash_synchronizeData();

    //char textProgram[] = "This is a test!";

    //uint8_t textProgram[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,0};

    //stm32dev_flash_s25fl208_sendCommand_pageProgram(0, sizeof(textProgram), (uint8_t*)textProgram);
    //stm32dev_flash_s25fl208_printSPIBuffer(4);
    //stm32dev_flash_s25fl208_printSPIDataBuffer(sizeof(textProgram), textProgram, 4);
    //stm32dev_flash_s25fl208_waitForProgrammingDone();
    stm32dev_flash_s25fl208_print_memoryBlock(0,64);

    //stm32dev_flash_s25fl208_print_memoryAll();

    uint32_t i,j;

    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET){
  	  uart_sendPrintf("Selection pin is high!!\r\n");
  	  ws2812_setColorAll(0,0,0);

  	  //flash_applicationParams.flash_btParams.bt_animationSelected = false;
  	  flash_applicationParams.flash_btParams.bt_animationSelected = true;
  	  flash_applicationParams.flash_btParams.bt_animationNumber = 2;

    } else {
  	  uart_sendPrintf("Selection pin is low!!\r\n");
  	  ws2812_setColorAll(255,255,255);

  	  flash_applicationParams.flash_btParams.bt_animationSelected = true;
  	  flash_applicationParams.flash_btParams.bt_animationNumber = 1;
    }

    uart_sendPrintf("Starting main loop\r\n");
  /* -3- Toggle IOs in an infinite loop */
  while (1)
  {
    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    //HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
    /* Insert delay 100 ms */
    HAL_Delay(10);
    UartReady = RESET;

    if(bt_fullStringReceived){
    	uart_fullStringReceived();
    	bt_fullStringReceived = false;
    }

    if(flash_applicationParams.flash_btParams.bt_animationSelected == true){

    	if(flash_applicationParams.flash_btParams.bt_animationNumber == 1){
          	for(j=0; j<flash_applicationParams.flash_btParams.bt_rgbcount; j++) {

          		if(flash_applicationParams.flash_btParams.bt_animationSelected == true){
          			for(i=1; i<=flash_applicationParams.flash_btParams.bt_rgbcount; i++) {
						if(flash_applicationParams.flash_btParams.bt_animationSelected == true){
							ws2812_setColorOne_color((uint8_t)i, ws2812_getWheel(((i+j)*20) & 255));
						}

					}
          		}

          	}
    	} else if (flash_applicationParams.flash_btParams.bt_animationNumber == 2){
    		// comet

    	}


    }
  }
}

void flash_initParams(void){
	flash_applicationParams.MCUVersion = 160203;

	flash_applicationParams.flash_btParams.bt_rgbcount = 10;
	flash_applicationParams.flash_btParams.bt_rgbmax = 150;
	flash_applicationParams.flash_btParams.bt_analog1 = 0;
	flash_applicationParams.flash_btParams.bt_temp = 0;
	flash_applicationParams.flash_btParams.bt_animationSelected = true;
	flash_applicationParams.flash_btParams.bt_animationNumber = 1;
	flash_applicationParams.flash_btParams.bt_colorPattern = 1;
	flash_applicationParams.flash_btParams.bt_printfEnabled = true;
}

void flash_synchronizeData(void){
	stm32dev_flash_s25fl208_waitForProgrammingDone();

	// read mcu version from flash
	application_parameters app_params = flash_readApplicationParams();


	uart_sendPrintf("MCU version: ");
	uart_sendPrintf(stm32dev_general_getDec(flash_applicationParams.MCUVersion));
	uart_sendPrintf(", flash MCU version: ");
	uart_sendPrintf(stm32dev_general_getDec(app_params.MCUVersion));
	uart_sendPrintf("\r\n");

	uart_sendPrintf("Size of flash_applicationParams: ");
	uart_sendPrintf(stm32dev_general_getDec(sizeof(flash_applicationParams)));
	uart_sendPrintf("\r\n");

	// check if the mcu version matches
	if(app_params.MCUVersion == flash_applicationParams.MCUVersion){
		// version is the same
		flash_applicationParams = app_params;
		uart_sendPrintf("Same MCU version on Flash\r\n");
	} else {
		// version is different
		uart_sendPrintf("New MCU version detected!\r\n");
		flash_writeData();
	}
}


application_parameters flash_readApplicationParams(void){
	application_parameters retVal;
	uint8_t* startData = stm32dev_flash_s25fl208_sendCommand_readData(0, sizeof(application_parameters));
	memcpy(&retVal, startData, sizeof(application_parameters));
	return retVal;
}

bt_parameters flash_readBtParams(void){
	bt_parameters retVal;
	uint8_t* startData = stm32dev_flash_s25fl208_sendCommand_readData(sizeof(application_parameters)+1, sizeof(bt_parameters));
	memcpy(&retVal, startData, sizeof(bt_parameters));
	return retVal;
}

void flash_writeData(void){

	uint8_t* appParams = (uint8_t*)&flash_applicationParams;

	for(uint8_t i=0; i<20; i++){
		uart_sendPrintf("flash_applicationParams[");
		uart_sendPrintf(stm32dev_general_getDec(i));
		uart_sendPrintf("] ");
		uart_sendPrintf(stm32dev_general_getHex8(appParams[i]));
		uart_sendPrintf("\r\n");
	}

    stm32dev_flash_s25fl208_sendCommand_pageProgram(0, sizeof(application_parameters), appParams);
    stm32dev_flash_s25fl208_waitForProgrammingDone();
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
static void SystemClock_Config(void)
{
	  RCC_ClkInitTypeDef RCC_ClkInitStruct;
	  RCC_OscInitTypeDef RCC_OscInitStruct;

	  /* Select HSI48 Oscillator as PLL source */
	  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48;
	  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI48;
	  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV2;
	  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
	  if (HAL_RCC_OscConfig(&RCC_OscInitStruct)!= HAL_OK)
	  {
	    Error_Handler();
	  }

	  /* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
	  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
	  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1)!= HAL_OK)
	  {
	    Error_Handler();
	  }
}


/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  UartReady = SET;


}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  UartReady = RESET;

  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

//  // received stuff
  	  // parse received stuff and put in buffer
  	  if(uart_receiveChar == '\n'){
  		uart_receivebuffer[uart_receivenumber++] = 0;
  		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
  		// parse string
  			//uart_fullStringReceived();
  			bt_fullStringReceived = true;
  		// clear
  		uart_receivenumber = 0;
  	  } else {
  		  uart_receivebuffer[uart_receivenumber++] = uart_receiveChar;
  		//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_9);
  		UartReady = SET;
  	  }

  	  //HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)&uart_receiveChar, 1);
//
//  	  UartReady = RESET;
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
  while(1)
  {
  }
}

void uart_sendPrintf(char* string){
	if(flash_applicationParams.flash_btParams.bt_printfEnabled){
		HAL_UART_Transmit(&UartHandle_printf, (uint8_t*)string, strlen(string), 5000);
	}
}

void uart_sendDMA(char* info){
    /*##-2- Start the transmission process #####################################*/
    /* While the UART in reception process, user can transmit data through
       "aTxBuffer" buffer */
    if(HAL_UART_Transmit_DMA(&UartHandle, (uint8_t*)info, strlen(info))!= HAL_OK)
    {
      Error_Handler();
    }

    /*##-3- Wait for the end of the transfer ###################################*/
    while (UartReady != SET)
    {
    }

    /* Reset transmission flag */
    UartReady = RESET;
}

void uart_fullStringReceived(void){
	// got full string
	uart_sendPrintf("BT: ");
	uart_sendPrintf((char*)uart_receivebuffer);
	uart_sendPrintf("\r\n");

	bt_handleIncoming((char*)uart_receivebuffer);

//	if(strcmp((char*)"red",(char*)uart_receivebuffer) == 0){
//		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12);
//		ws2812_setColorAll(255,0,0);
//	} else if(strcmp((char*)"green",(char*)uart_receivebuffer) == 0){
//		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_13);
//		ws2812_setColorAll(0,255,0);
//	} else if(strcmp((char*)"blue",(char*)uart_receivebuffer) == 0){
//		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
//		ws2812_setColorAll(0,0,255);
//	}
}

void ws2812_setColorOne(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b){
	ws2812_setColor(pixel,pixel,r,g,b);
}

inline void ws2812_setColor(uint32_t pixelFrom, uint32_t pixelTo, uint8_t r, uint8_t g, uint8_t b){
	uint16_t sec = 0;

	for(uint8_t i = 0; i< pixelTo; i++){


			for(uint8_t j = 0; j< 8; j++){
				if(i >= pixelFrom-1){
					if(g & (1<<(7-j))){
						aCCValue_Buffer[sec++] = ws2812b_highPWM;
					} else {
						aCCValue_Buffer[sec++] = ws2812b_lowPWM;
					}
				} else {
					sec++;
				}

			}
			for(uint8_t j = 0; j< 8; j++){
				if(i >= pixelFrom-1){
					if(r & (1<<(7-j))){
						aCCValue_Buffer[sec++] = ws2812b_highPWM;
					} else {
						aCCValue_Buffer[sec++] = ws2812b_lowPWM;
					}
				} else {
					sec++;
				}
			}
			for(uint8_t j = 0; j< 8; j++){
				if(i >= pixelFrom-1){
					if(b & (1<<(7-j))){
						aCCValue_Buffer[sec++] = ws2812b_highPWM;
					} else {
						aCCValue_Buffer[sec++] = ws2812b_lowPWM;
					}
				} else {
					sec++;
				}
			}
	}
}

void ws2812_setColorAll(uint8_t r, uint8_t g, uint8_t b){
	ws2812_setColor(1,flash_applicationParams.flash_btParams.bt_rgbcount, r, g, b);
}

void ws2812_setColorOne_color(uint32_t pixel, uint32_t color){
	ws2812_setColorOne(pixel, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));
}

void ws2812_setColor_color(uint32_t pixelFrom, uint32_t pixelTo, uint32_t color){
	ws2812_setColor(pixelFrom, pixelTo, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));
}

void ws2812_setColorAll_color(uint32_t color){
	ws2812_setColorAll((color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));
}

uint32_t ws2812_getWheel(uint8_t wheelPos){
	  if(wheelPos < 85) {
	   return ws2812_getColor(wheelPos * 3, 255 - wheelPos * 3, 0);
	  } else if(wheelPos < 170) {
		  wheelPos -= 85;
	   return ws2812_getColor(255 - wheelPos * 3, 0, wheelPos * 3);
	  } else {
		  wheelPos -= 170;
	   return ws2812_getColor(0, wheelPos * 3, 255 - wheelPos * 3);
	  }
}

uint32_t ws2812_getColor(uint8_t red, uint8_t green, uint8_t blue){
	return ((uint32_t)red << 16) | ((uint32_t)green <<  8) | blue;
}

char* bt_getCommandPtr(char* incoming){
	for(uint8_t i=0; i<COUNTOF(bt_commandlist); i++){
		char* tmpPtr = strstr(incoming,bt_commandlist[i]);
		if(tmpPtr != NULL){
			return (char*)bt_commandlist[i];
		}
	}
	return NULL;
}

void bt_handleIncoming(char* incoming){
	// local vars
    char **tokens;
    size_t numtokens;

	// no space protection
	if(strstr(incoming," ") == NULL){
		return;
	}

	// get command
	char* cmd = bt_getCommandPtr(incoming);
	if(cmd == NULL) return;

	// split string
	tokens = stm32dev_general_strsplit(incoming, "; \t\n", &numtokens);

	if(cmd == bt_command_setRGB){
		if(numtokens == 6){
			int pixelNrFrom = atoi(tokens[1]);
			int pixelNrTo = atoi(tokens[2]);
			int redValue = atoi(tokens[3]);
			int blueValue = atoi(tokens[4]);
			int greenValue = atoi(tokens[5]);

			if(pixelNrFrom == 0 || pixelNrTo == 0){
				ws2812_setColorAll(redValue, blueValue, greenValue);
			} else {
				ws2812_setColor(pixelNrFrom, pixelNrTo, redValue, blueValue, greenValue);
			}
		}
	} else if (cmd == bt_command_getFlashParams){
		bt_sendFlashParams();
	}

	// free everything
	for (size_t i = 1; i < numtokens; i++) {
		free(tokens[i]);
	}
	if (tokens != NULL)
		free(tokens);
}

void bt_sendFlashParams(void){
	uart_sendDMA("Testing the whole command that is too long \n");
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
