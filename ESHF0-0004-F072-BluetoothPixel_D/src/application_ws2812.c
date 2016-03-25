/*
 * application_ws2812.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */
#include "application_ws2812.h"
#include "application_general.h"

const uint8_t ws2812b_lowPWM = 20;
const uint8_t ws2812b_highPWM = 41;

TIM_HandleTypeDef    TimHandle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Capture Compare buffer */
uint8_t aCCValue_Buffer[3640];

/* Timer Period*/
uint32_t uhTimerPeriod  = 0;

void ws2812_setColorOne(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b){
	ws2812_setColor(pixel,pixel,r,g,b);
}

void ws2812_setColor(uint32_t pixelFrom, uint32_t pixelTo, uint8_t r, uint8_t g, uint8_t b){
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

void general_wsLED_init(void){

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


	    ws2812_setColorAll(0,0,0);

	    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_15) == GPIO_PIN_SET){
	  	  uart_sendPrintf("Selection pin is high!!\r\n");
	  	  //ws2812_setColorAll(0,0,0);

	  	  //flash_applicationParams.flash_btParams.bt_animationSelected = true;
	  	  flash_applicationParams.flash_btParams.bt_animationSelected = true;
	  	  flash_applicationParams.flash_btParams.bt_animationNumber = 4;

	    } else {
	  	  uart_sendPrintf("Selection pin is low!!\r\n");
	  	  //ws2812_setColorAll(255,255,255);

	  	  flash_applicationParams.flash_btParams.bt_animationSelected = true;
	  	  flash_applicationParams.flash_btParams.bt_animationNumber = 1;
	    }
}
