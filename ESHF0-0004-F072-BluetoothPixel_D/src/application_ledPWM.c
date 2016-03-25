/*
 * application_ledPWM.c
 *
 *  Created on: 10-mrt.-2016
 *      Author: Robbe
 */

#include "application_ledPWM.h"
#include "application_general.h"

TIM_HandleTypeDef    TimHandle_LEDPWM1;
TIM_HandleTypeDef    TimHandle_LEDPWM2;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

#define  PERIOD_VALUE       (uint32_t)(666 - 1)  /* Period Value  */
#define  PULSE1_VALUE       (uint32_t)(PERIOD_VALUE/2)        /* Capture Compare 1 Value  */

void general_ledPWM_init(void){
	uint32_t uhPrescalerValue = 0;
	uhPrescalerValue = (uint32_t)(SystemCoreClock / 16000000) - 1;

	// init PWM1
	  /* Initialize TIMx peripheral as follows:
	       + Prescaler = (SystemCoreClock / 16000000) - 1
	       + Period = (666 - 1)
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  TimHandle_LEDPWM1.Instance = TIMx_PWM1;

	  TimHandle_LEDPWM1.Init.Prescaler         = uhPrescalerValue;
	  TimHandle_LEDPWM1.Init.Period            = PERIOD_VALUE;
	  TimHandle_LEDPWM1.Init.ClockDivision     = 0;
	  TimHandle_LEDPWM1.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  TimHandle_LEDPWM1.Init.RepetitionCounter = 0;
	  if (HAL_TIM_PWM_Init(&TimHandle_LEDPWM1) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the PWM channels #########################################*/
	  /* Common configuration for all channels */
	  sConfig.OCMode       = TIM_OCMODE_PWM1;
	  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
	  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	  /* Set the pulse value for channel 1 */
	  sConfig.Pulse = PULSE1_VALUE;
	  if (HAL_TIM_PWM_ConfigChannel(&TimHandle_LEDPWM1, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  /*##-3- Start PWM signals generation #######################################*/
	  /* Start channel 1 */
	  if (HAL_TIM_PWM_Start(&TimHandle_LEDPWM1, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* PWM Generation Error */
	    Error_Handler();
	  }

	// init PWM2
	  /* Initialize TIMx peripheral as follows:
	       + Prescaler = (SystemCoreClock / 16000000) - 1
	       + Period = (666 - 1)
	       + ClockDivision = 0
	       + Counter direction = Up
	  */
	  TimHandle_LEDPWM2.Instance = TIMx_PWM2;

	  TimHandle_LEDPWM2.Init.Prescaler         = uhPrescalerValue;
	  TimHandle_LEDPWM2.Init.Period            = PERIOD_VALUE;
	  TimHandle_LEDPWM2.Init.ClockDivision     = 0;
	  TimHandle_LEDPWM2.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  TimHandle_LEDPWM2.Init.RepetitionCounter = 0;
	  if (HAL_TIM_PWM_Init(&TimHandle_LEDPWM2) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the PWM channels #########################################*/
	  /* Common configuration for all channels */
	  sConfig.OCMode       = TIM_OCMODE_PWM1;
	  sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	  sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	  sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	  sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
	  sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	  /* Set the pulse value for channel 1 */
	  sConfig.Pulse = PULSE1_VALUE;
	  if (HAL_TIM_PWM_ConfigChannel(&TimHandle_LEDPWM2, &sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  /*##-3- Start PWM signals generation #######################################*/
	  /* Start channel 1 */
	  if (HAL_TIM_PWM_Start(&TimHandle_LEDPWM2, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* PWM Generation Error */
	    Error_Handler();
	  }
}
