#include "application_adc.h"
  ADC_HandleTypeDef    application_adc_handle;
  /* Variable used to get converted value */
  __IO uint16_t application_adc_values[4];

  void application_adc_init(void){
	  ADC_ChannelConfTypeDef sConfig;
	    /*##-1- Configure the ADC peripheral #######################################*/
	    application_adc_handle.Instance          = ADC1;

	    if (HAL_ADC_DeInit(&application_adc_handle) != HAL_OK)
	    {
	      /* ADC de-initialization Error */
	      Error_Handler();
	    }

	    application_adc_handle.Init.ClockPrescaler        = ADC_CLOCK_SYNC_PCLK_DIV4;
	    application_adc_handle.Init.Resolution            = ADC_RESOLUTION_12B;
	    application_adc_handle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	    application_adc_handle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;                       /* Sequencer disabled (ADC conversion on only 1 channel: channel set on rank 1) */
	    application_adc_handle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
	    application_adc_handle.Init.LowPowerAutoWait      = DISABLE;
	    application_adc_handle.Init.LowPowerAutoPowerOff  = DISABLE;
	    application_adc_handle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 conversion at each conversion trig */
	    application_adc_handle.Init.DiscontinuousConvMode = ENABLE;                       /* Parameter discarded because sequencer is disabled */
	    application_adc_handle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
	    application_adc_handle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE;
	    application_adc_handle.Init.DMAContinuousRequests = ENABLE;
	    application_adc_handle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;



	    if (HAL_ADC_Init(&application_adc_handle) != HAL_OK)
	    {
	      /* ADC initialization Error */
	      Error_Handler();
	    }

	    /*##-2- Configure ADC regular channel ######################################*/
	    sConfig.Channel      = ADC_CHANNEL_0;
	    sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
	    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;


	    if (HAL_ADC_ConfigChannel(&application_adc_handle, &sConfig) != HAL_OK)
	    {
	      /* Channel Configuration Error */
	      Error_Handler();
	    }


	    /*##-2- Configure ADC regular channel ######################################*/
	    sConfig.Channel      = ADC_CHANNEL_1;
	    sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;
	    sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;


	    if (HAL_ADC_ConfigChannel(&application_adc_handle, &sConfig) != HAL_OK)
	    {
	      /* Channel Configuration Error */
	      Error_Handler();
	    }

	    /* Configuration of channel on ADCx regular group on sequencer rank 2 */
	    /* Replicate previous rank settings, change only channel */
	    /* Note: On STM32F0xx, rank is defined by channel number. ADC Channel       */
	    /*       ADC_CHANNEL_TEMPSENSOR is on ADC channel 16, there is 1 other      */
	    /*       channel enabled with lower channel number. Therefore,              */
	    /*       ADC_CHANNEL_TEMPSENSOR will be converted by the sequencer as the   */
	    /*       2nd rank.                                                          */
	    sConfig.Channel      = ADC_CHANNEL_TEMPSENSOR;

	    if (HAL_ADC_ConfigChannel(&application_adc_handle, &sConfig) != HAL_OK)
	    {
	      /* Channel Configuration Error */
	      Error_Handler();
	    }

	    /* Configuration of channel on ADCx regular group on sequencer rank 3 */
	    /* Replicate previous rank settings, change only channel */
	    /* Note: On STM32F0xx, rank is defined by channel number. ADC Channel       */
	    /*       ADC_CHANNEL_VREFINT is on ADC channel 17, there is are 2 other     */
	    /*       channels enabled with lower channel number. Therefore,             */
	    /*       ADC_CHANNEL_VREFINT will be converted by the sequencer as the      */
	    /*       3rd rank.                                                          */
	    sConfig.Channel      = ADC_CHANNEL_VREFINT;

	    if (HAL_ADC_ConfigChannel(&application_adc_handle, &sConfig) != HAL_OK)
	    {
	      /* Channel Configuration Error */
	      Error_Handler();
	    }
  }
  void application_adc_sample(void){
	  for(uint8_t i = 0; i<4; i++){
		  /*##-3- Start the conversion process #######################################*/
		  if (HAL_ADC_Start(&application_adc_handle) != HAL_OK)
		  {
		    /* Start Conversation Error */
		    Error_Handler();
		  }

		  /*##-4- Wait for the end of conversion #####################################*/
		  /*  For simplicity reasons, this example is just waiting till the end of the
		      conversion, but application may perform other tasks while conversion
		      operation is ongoing. */
		  if (HAL_ADC_PollForConversion(&application_adc_handle, 10) != HAL_OK)
		  {
		    /* End Of Conversion flag not set on time */
		    Error_Handler();
		  }
		  else
		  {
		    /* ADC conversion completed */
		    /*##-5- Get the converted value of regular channel  ########################*/
			  application_adc_values[i] = HAL_ADC_GetValue(&application_adc_handle);
		  }
	  }
  }
