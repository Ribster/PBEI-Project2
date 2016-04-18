#include "application_leddim.h"
#include "application_general.h"

uint32_t application_leddim_uhPrescalerValue = 0;
TIM_HandleTypeDef    application_leddim_handle_1;
TIM_HandleTypeDef    application_leddim_handle_2;
TIM_HandleTypeDef    application_leddim_handle_3;
TIM_OC_InitTypeDef application_leddim_sConfig;

void application_leddim_init(void){
	application_leddim_uhPrescalerValue = (uint32_t)(SystemCoreClock / 40000000) - 1;


	  application_leddim_handle_1.Instance = TIM16;
	  application_leddim_handle_1.Init.Prescaler         = application_leddim_uhPrescalerValue;
	  application_leddim_handle_1.Init.Period            = 1000;
	  application_leddim_handle_1.Init.ClockDivision     = 0;
	  application_leddim_handle_1.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  application_leddim_handle_1.Init.RepetitionCounter = 0;
	  if (HAL_TIM_PWM_Init(&application_leddim_handle_1) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  application_leddim_handle_2.Instance = TIM17;
	  application_leddim_handle_2.Init.Prescaler         = application_leddim_uhPrescalerValue;
	  application_leddim_handle_2.Init.Period            = 1000;
	  application_leddim_handle_2.Init.ClockDivision     = 0;
	  application_leddim_handle_2.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  application_leddim_handle_2.Init.RepetitionCounter = 0;
	  if (HAL_TIM_PWM_Init(&application_leddim_handle_2) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  application_leddim_handle_3.Instance = TIM2;
	  application_leddim_handle_3.Init.Prescaler         = application_leddim_uhPrescalerValue;
	  application_leddim_handle_3.Init.Period            = 1000;
	  application_leddim_handle_3.Init.ClockDivision     = 0;
	  application_leddim_handle_3.Init.CounterMode       = TIM_COUNTERMODE_UP;
	  application_leddim_handle_3.Init.RepetitionCounter = 0;
	  if (HAL_TIM_PWM_Init(&application_leddim_handle_3) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the PWM channels #########################################*/
	  /* Common configuration for all channels */
	  application_leddim_sConfig.OCMode       = TIM_OCMODE_PWM1;
	  application_leddim_sConfig.OCPolarity   = TIM_OCPOLARITY_HIGH;
	  application_leddim_sConfig.OCFastMode   = TIM_OCFAST_DISABLE;
	  application_leddim_sConfig.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
	  application_leddim_sConfig.OCIdleState  = TIM_OCIDLESTATE_RESET;
	  application_leddim_sConfig.OCNIdleState = TIM_OCNIDLESTATE_RESET;

	  /* Set the pulse value for channel 1 */
	  application_leddim_sConfig.Pulse = 0;
	  if (HAL_TIM_PWM_ConfigChannel(&application_leddim_handle_1, &application_leddim_sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  application_leddim_sConfig.Pulse = 0;
	  if (HAL_TIM_PWM_ConfigChannel(&application_leddim_handle_2, &application_leddim_sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  application_leddim_sConfig.Pulse = 0;
	  if (HAL_TIM_PWM_ConfigChannel(&application_leddim_handle_3, &application_leddim_sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(&application_leddim_handle_1, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* PWM Generation Error */
	    Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(&application_leddim_handle_2, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* PWM Generation Error */
	    Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(&application_leddim_handle_3, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* PWM Generation Error */
	    Error_Handler();
	  }
}

void application_leddim_setDimpromille(uint8_t dimchan, uint16_t value){
	TIM_HandleTypeDef *tmp = NULL;

	if(dimchan == 0 || dimchan > 3){
		return;
	}

	if(value > 1001) value = 1001;

	if(dimchan == 1){
		tmp = &application_leddim_handle_1;
	} else if(dimchan == 2){
		tmp = &application_leddim_handle_2;
	} else if(dimchan == 3){
		tmp = &application_leddim_handle_3;
	}

	application_leddim_sConfig.Pulse = 1001 - value;

	  if (HAL_TIM_PWM_ConfigChannel(tmp, &application_leddim_sConfig, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* Configuration Error */
	    Error_Handler();
	  }

	  if (HAL_TIM_PWM_Start(tmp, TIM_CHANNEL_1) != HAL_OK)
	  {
	    /* PWM Generation Error */
	    Error_Handler();
	  }
}

void application_leddim_testSequence(void){


	for(uint8_t j = 0; j<3; j++){
		for(uint16_t i = 0; i<=1001; i++){
			application_leddim_setDimpromille(4-(j+1), 1001-i);
			HAL_Delay(2);
		}
	}

	for(uint8_t j = 0; j<3; j++){
		for(uint16_t i = 0; i<=1001; i++){
			application_leddim_setDimpromille(j+1, i);
			HAL_Delay(2);
		}
	}
}
