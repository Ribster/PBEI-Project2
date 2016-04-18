#include "application_ws2812b.h"
#include "application_general.h"

const uint8_t application_ws2812b_lowPWM = 20;
const uint8_t application_ws2812b_highPWM = 41;

TIM_HandleTypeDef    application_ws2812b_handle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef application_ws2812b_sConfig;

/* Capture Compare buffer */
uint8_t application_ws2812b_aCCValue_Buffer[3640];

/* Timer Period*/
uint32_t application_ws2812b_uhTimerPeriod  = 0;

uint32_t application_ws2812b_rgbcount = 10;

void application_ws2812b_init(void){
	  memset(application_ws2812b_aCCValue_Buffer, 0, 3640);
	  application_ws2812b_setColorAll(255,0,0);


	  /* Compute the value of ARR regiter to generate signal frequency at 17.57 Khz */
	  application_ws2812b_uhTimerPeriod = (uint32_t)((SystemCoreClock / 800000) - 1);
	//  /* Compute CCR1 value to generate a duty cycle at 75% */
	//  application_ws2812b_aCCValue_Buffer[0] = (uint32_t)(((uint32_t) 75 * (application_ws2812b_uhTimerPeriod - 1)) / 100);
	//  /* Compute CCR2 value to generate a duty cycle at 50% */
	//  application_ws2812b_aCCValue_Buffer[1] = (uint32_t)(((uint32_t) 50 * (application_ws2812b_uhTimerPeriod - 1)) / 100);
	//  /* Compute CCR3 value to generate a duty cycle at 25% */
	//  application_ws2812b_aCCValue_Buffer[2] = (uint32_t)(((uint32_t) 25 * (application_ws2812b_uhTimerPeriod - 1)) / 100);


	  application_ws2812b_handle.Instance = TIM3;

	    application_ws2812b_handle.Init.Period            = application_ws2812b_uhTimerPeriod;
	    application_ws2812b_handle.Init.RepetitionCounter = 3640;
	    application_ws2812b_handle.Init.Prescaler         = 0;
	    application_ws2812b_handle.Init.ClockDivision     = 0;
	    application_ws2812b_handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
	    if (HAL_TIM_PWM_Init(&application_ws2812b_handle) != HAL_OK)
	    {
	      /* Initialization Error */
	      Error_Handler();
	    }


	    /*##-2- Configure the PWM channel 3 ########################################*/
	    application_ws2812b_sConfig.OCMode       = TIM_OCMODE_PWM1;
	    application_ws2812b_sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	    application_ws2812b_sConfig.Pulse        = application_ws2812b_aCCValue_Buffer[0];
	    application_ws2812b_sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	    application_ws2812b_sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	    application_ws2812b_sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
	    application_ws2812b_sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	    if (HAL_TIM_PWM_ConfigChannel(&application_ws2812b_handle, &application_ws2812b_sConfig, TIM_CHANNEL_1) != HAL_OK)
	    {
	      /* Configuration Error */
	      Error_Handler();
	    }



	    /*##-3- Start PWM signal generation in DMA mode ############################*/
	    if (HAL_TIM_PWM_Start_DMA(&application_ws2812b_handle, TIM_CHANNEL_1, (uint32_t*)application_ws2812b_aCCValue_Buffer, 3640) != HAL_OK)
	    {

	      /* Starting Error */
	      Error_Handler();
	    }
}

uint32_t application_ws2812b_getColor(uint8_t red, uint8_t green, uint8_t blue){
	return ((uint32_t)red << 16) | ((uint32_t)green <<  8) | blue;
}

uint32_t application_ws2812b_getWheel(uint8_t wheelPos){
	  if(wheelPos < 85) {
	   return application_ws2812b_getColor(wheelPos * 3, 255 - wheelPos * 3, 0);
	  } else if(wheelPos < 170) {
		  wheelPos -= 85;
	   return application_ws2812b_getColor(255 - wheelPos * 3, 0, wheelPos * 3);
	  } else {
		  wheelPos -= 170;
	   return application_ws2812b_getColor(0, wheelPos * 3, 255 - wheelPos * 3);
	  }
}

void application_ws2812b_setColorAll_color(uint32_t color){
	application_ws2812b_setColorAll((color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));
}

void application_ws2812b_setColor_color(uint32_t pixelFrom, uint32_t pixelTo, uint32_t color){
	application_ws2812b_setColor(pixelFrom, pixelTo, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));
}

void application_ws2812b_setColorOne_color(uint32_t pixel, uint32_t color){
	application_ws2812b_setColorOne(pixel, (color & 0xFF0000) >> 16, (color & 0xFF00) >> 8, (color & 0xFF));
}

void application_ws2812b_setColorAll(uint8_t r, uint8_t g, uint8_t b){
	application_ws2812b_setColor(1,application_ws2812b_rgbcount, r, g, b);
}

void application_ws2812b_setColor(uint32_t pixelFrom, uint32_t pixelTo, uint8_t r, uint8_t g, uint8_t b){
	uint16_t sec = 0;

	for(uint8_t i = 0; i< pixelTo; i++){


			for(uint8_t j = 0; j< 8; j++){
				if(i >= pixelFrom-1){
					if(g & (1<<(7-j))){
						application_ws2812b_aCCValue_Buffer[sec++] = application_ws2812b_highPWM;
					} else {
						application_ws2812b_aCCValue_Buffer[sec++] = application_ws2812b_lowPWM;
					}
				} else {
					sec++;
				}

			}
			for(uint8_t j = 0; j< 8; j++){
				if(i >= pixelFrom-1){
					if(r & (1<<(7-j))){
						application_ws2812b_aCCValue_Buffer[sec++] = application_ws2812b_highPWM;
					} else {
						application_ws2812b_aCCValue_Buffer[sec++] = application_ws2812b_lowPWM;
					}
				} else {
					sec++;
				}
			}
			for(uint8_t j = 0; j< 8; j++){
				if(i >= pixelFrom-1){
					if(b & (1<<(7-j))){
						application_ws2812b_aCCValue_Buffer[sec++] = application_ws2812b_highPWM;
					} else {
						application_ws2812b_aCCValue_Buffer[sec++] = application_ws2812b_lowPWM;
					}
				} else {
					sec++;
				}
			}
	}
}

void application_ws2812b_setColorOne(uint32_t pixel, uint8_t r, uint8_t g, uint8_t b){
	application_ws2812b_setColor(pixel,pixel,r,g,b);
}
