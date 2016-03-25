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
#include "application_general.h"
#include "application_bluetooth.h"
#include "application_flash.h"
#include "application_printf.h"
#include "application_ws2812.h"
#include "application_adc.h"
#include "application_lm75.h"
#include "application_rtc.h"
#include "application_rng.h"
#include "application_ledPWM.h"
#include "application_usb.h"

/* UART handler declaration */





/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void){
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
  general_uart_init();

  general_crc_init();
  uart_sendPrintf("\t[INIT] CRC\r\n");

  general_gpio_init();
  uart_sendPrintf("\t[INIT] GPIO\r\n");

  general_bluetooth_init();
  uart_sendPrintf("\t[INIT] BLE\r\n");

  general_spiFlash_init();
  uart_sendPrintf("\t[INIT] SPI FLASH\r\n");

  general_wsLED_init();
  uart_sendPrintf("\t[INIT] WS2812B\r\n");

  general_adc_init();
  uart_sendPrintf("\t[INIT] ADC\r\n");

  general_lm75_init();
  uart_sendPrintf("\t[INIT] LM75\r\n");

  general_rtc_init();
  uart_sendPrintf("\t[INIT] RTC\r\n");

  general_ledPWM_init();
  uart_sendPrintf("\t[INIT] LEDPWM\r\n");

  general_usb_init();
  uart_sendPrintf("\t[INIT] USB\r\n");

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

    uint8_t i,j,k;
    int32_t p;


    if(flash_applicationParams.flash_btParams.bt_animationSelected == true){

    	if(flash_applicationParams.flash_btParams.bt_animationNumber == 1){
    		// rainbow animation
          	for(j=0; j<flash_applicationParams.flash_btParams.bt_rgbcount; j++) {

          		if(flash_applicationParams.flash_btParams.bt_animationSelected == true){
          			for(i=1; i<=flash_applicationParams.flash_btParams.bt_rgbcount; i++) {
						if(flash_applicationParams.flash_btParams.bt_animationSelected == true){
							ws2812_setColorOne_color((uint8_t)i, ws2812_getWheel(((i+j)*20) & 255));
							HAL_Delay(10);
						}

					}
          		}
          		HAL_Delay(10);

          	}
    	} else if (flash_applicationParams.flash_btParams.bt_animationNumber == 2){
    		// comet
    		for(i=0; i<(flash_applicationParams.flash_btParams.bt_rgbcount/2); i++){
				ws2812_setColorOne_color((uint8_t)(flash_applicationParams.flash_btParams.bt_rgbcount/2)-i, ws2812_getWheel(((i)*20) & 255));
				ws2812_setColorOne_color((uint8_t)(flash_applicationParams.flash_btParams.bt_rgbcount/2)+i+1, ws2812_getWheel(((i)*20) & 255));
				HAL_Delay(200);
    		}

    		for(i=0; i<(flash_applicationParams.flash_btParams.bt_rgbcount/2); i++){
				ws2812_setColorOne_color((uint8_t)i+1, 0);
				ws2812_setColorOne_color((uint8_t)(flash_applicationParams.flash_btParams.bt_rgbcount)-i, 0);
				HAL_Delay(200);
    		}
    	} else if (flash_applicationParams.flash_btParams.bt_animationNumber == 3){
    		i = 6; // length of comet
    		j = 200; // speed of comet

    		// 0	0
    		// 1	255/6 * 1 	= 43
    		// 2	255/6 * 2 	= 85
    		// 3	255/6 * 3 	= 128
    		// 4	255/6 * 4 	= 170
    		// 5	255/6 * 5 	= 213
    		// 6	255 		= 255

    		for(k=1; k<=flash_applicationParams.flash_btParams.bt_rgbcount; k++){
    			// start with the first
    			ws2812_setColorOne_color((uint8_t)k, ws2812_getWheel(((k)*20) & 255));
    			if(k>=i){
    				ws2812_setColorOne_color((uint8_t)k-i, 0);
    				//ws2812_setColorOne_color((uint8_t)k, ws2812_getWheel(((k)*20) & 255));
    			}

    			if(k==flash_applicationParams.flash_btParams.bt_rgbcount){
    				for(p=0; p<=i; p++){
        				ws2812_setColorOne_color((uint8_t)k-i, 0);
        				k++;
        				HAL_Delay(j);
    				}
    			}

    			HAL_Delay(j);
    		}
    	} else if (flash_applicationParams.flash_btParams.bt_animationNumber == 4){
    		i = 5; // length of comet
    		j = 100; // speed of comet

    		// get random
    		i = rng_getRandom() % ((flash_applicationParams.flash_btParams.bt_rgbcount*3)/2);
    		j = rng_getRandom() % 175;


    		// under limit
    		if(i < 3) i = 3;
    		if(j < 40) j = 40;

    		// 0	0
    		// 1	255/6 * 1 	= 43
    		// 2	255/6 * 2 	= 85
    		// 3	255/6 * 3 	= 128
    		// 4	255/6 * 4 	= 170
    		// 5	255/6 * 5 	= 213
    		// 6	255 		= 255

    		uint8_t r_mix = rng_getRandom()%256;
    		uint8_t g_mix = rng_getRandom()%256;
    		uint8_t b_mix = rng_getRandom()%256;

    		for(k=1; k<=flash_applicationParams.flash_btParams.bt_rgbcount+i; k++){
    			// start with the first


    			for(p=k; p>(k-i); p--){
    				if(p>0){
    					uint8_t colorr = ((r_mix/i)*(p-k+i));
    					uint8_t colorg = ((g_mix/i)*(p-k+i));
    					uint8_t colorb = ((b_mix/i)*(p-k+i));

    					uint32_t newColor = 0;

    					newColor |= colorr << 16;
    					newColor |= colorg << 8;
    					newColor |= colorb;

    					ws2812_setColorOne_color((uint8_t)p, newColor);
    				}
    			}


    			if(k>=i){
    				ws2812_setColorOne_color((uint8_t)k-i, 0);
    				//ws2812_setColorOne_color((uint8_t)k, ws2812_getWheel(((k)*20) & 255));
    			}

    			HAL_Delay(j);
    		}
    	} else if (flash_applicationParams.flash_btParams.bt_animationNumber == 5){

    		uart_sendPrintf(" ");
    		uart_sendPrintf(stm32dev_general_getDec(rng_getRandom()));
    		uart_sendPrintf("\r");
    	}


    } else {
        // adc values
    	uart_sendPrintf(" ");

    	adc_sample();

    	for(uint8_t k=0; k<ADCCONVERTEDVALUES_BUFFER_SIZE; k++){

        	uart_sendPrintf(stm32dev_general_getDec(k));
        	uart_sendPrintf("=");
        	uart_sendPrintf(stm32dev_general_getDec(aADCxConvertedValues[k]));
        	uart_sendPrintf("; ");
        }

        uart_sendPrintf(" Temp: ");
        uart_sendPrintf(stm32dev_general_getDec(lm75_temperature()));
        uart_sendPrintf("\r");
        HAL_Delay(200);

/*        rtc_calenderprint(aShowTime, aShowDate);
        uart_sendPrintf("--------------------------------------\r\n");*/
    }



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

  //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);

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
  	//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
}

/**
  * @brief  Conversion complete callback in non blocking mode
  * @param  AdcHandle : ADC handle
  * @note   This example shows a simple way to report end of conversion
  *         and get conversion result. You can add your own implementation.
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *AdcHandle)
{
  /* Report to main program that ADC sequencer has reached its end */

  //uart_sendPrintf("ADC done \r\n");
	ubSequenceCompleted = SET;
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
}

/**
  * @brief  Conversion DMA half-transfer callback in non blocking mode
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{

}

/**
  * @brief  ADC error callback in non blocking mode
  *        (ADC conversion with interruption or transfer by DMA)
  * @param  hadc: ADC handle
  * @retval None
  */
void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
  /* In case of ADC error, call main error handler */
  Error_Handler();
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED_GREEN: Transfer in transmission process is correct */

}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *I2cHandle)
{
  /* Toggle LED_GREEN: Transfer in reception process is correct */

}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *I2cHandle)
{
 Error_Handler();
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
