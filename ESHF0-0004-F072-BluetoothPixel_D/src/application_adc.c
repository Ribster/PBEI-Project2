/*
 * application_adc.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */

#include "application_adc.h"

// ADC
ADC_HandleTypeDef    AdcHandle;
/* Variable containing ADC conversions results */
volatile uint16_t   aADCxConvertedValues[ADCCONVERTEDVALUES_BUFFER_SIZE];
/* Variable to report ADC sequencer status */
volatile uint8_t         ubSequenceCompleted = RESET;     /* Set when all ranks of the sequence have been converted */

/* Variables for ADC conversions results computation to physical values */
uint16_t   uhADCChannelaToDAC_mVolt = 0;
uint16_t   uhADCChannelbToDAC_mVolt = 0;
uint16_t   uhVrefInt_mVolt = 0;
 int32_t   wTemperature_DegreeCelsius = 0;



 void general_adc_init(void){
 	  /* Configure the ADCx peripheral */
	  ADC_ChannelConfTypeDef   sConfig;

 	  /* Configuration of AdcHandle init structure: ADC parameters and regular group */
 	  AdcHandle.Instance = ADCx;

 	  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
 	  {
 	    /* ADC initialization error */
 	    Error_Handler();
 	  }

 	  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
 	  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
 	  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
 	  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;    /* Sequencer will convert the number of channels configured below, successively from the lowest to the highest channel number */
 	  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
 	  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
 	  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
 	  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 rank converted at each conversion trig, and because discontinuous mode is enabled */
 	  AdcHandle.Init.DiscontinuousConvMode = ENABLE;                        /* Sequencer of regular group will convert the sequence in several sub-divided sequences */
 	  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
 	  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig of conversion by software start (no external event) */
 	  AdcHandle.Init.DMAContinuousRequests = ENABLE;                        /* ADC-DMA continuous requests to match with DMA configured in circular mode */
 	  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
 	  /* Note: Set long sampling time due to internal channels (VrefInt,          */
 	  /*       temperature sensor) constraints. Refer to device datasheet for     */
 	  /*       min/typ/max values.                                                */
 	  AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_239CYCLES_5;

 	  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
 	  {
 	    /* ADC initialization error */
 	    Error_Handler();
 	  }



 	  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
 	  /* Note: Considering IT occurring after each ADC conversion (IT by DMA end  */
 	  /*       of transfer), select sampling time and ADC clock with sufficient   */
 	  /*       duration to not create an overhead situation in IRQHandler.        */
 	  sConfig.Channel      = ADCx_CHANNELa;
 	  sConfig.Rank         = ADC_RANK_CHANNEL_NUMBER;

 	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
 	  {
 	    /* Channel Configuration Error */
 	    Error_Handler();
 	  }

 	  /* Configuration of channel on ADCx regular group on sequencer rank 1 */
 	  /* Note: Considering IT occurring after each ADC conversion (IT by DMA end  */
 	  /*       of transfer), select sampling time and ADC clock with sufficient   */
 	  /*       duration to not create an overhead situation in IRQHandler.        */
 	  sConfig.Channel      = ADCx_CHANNELb;

 	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
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

 	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
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

 	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
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
	  sConfig.Channel      = ADC_CHANNEL_VBAT;

	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	  {
	    /* Channel Configuration Error */
	    Error_Handler();
	  }

 	  /* Run the ADC calibration */
 	  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
 	  {
 	    /* Calibration Error */
 	    Error_Handler();
 	  }

 	  /* Start ADC conversion on regular group with transfer by DMA */
 	  if (HAL_ADC_Start_DMA(&AdcHandle,
 	                        (uint32_t *)aADCxConvertedValues,
 	                        ADCCONVERTEDVALUES_BUFFER_SIZE
 	                       ) != HAL_OK)
 	  {
 	    /* Start Error */
 	    Error_Handler();
 	  }

 	 //adc_sample();

 }

 void general_adc_init_inttemp(void){
	 /* Configure the ADCx peripheral */
	 	  ADC_ChannelConfTypeDef   sConfig;

	  	  /* Configuration of AdcHandle init structure: ADC parameters and regular group */
	  	  AdcHandle.Instance = ADCx;

	  	  if (HAL_ADC_DeInit(&AdcHandle) != HAL_OK)
	  	  {
	  	    /* ADC initialization error */
	  	    Error_Handler();
	  	  }

	  	  AdcHandle.Init.ClockPrescaler        = ADC_CLOCK_ASYNC_DIV1;
	  	  AdcHandle.Init.Resolution            = ADC_RESOLUTION_12B;
	  	  AdcHandle.Init.DataAlign             = ADC_DATAALIGN_RIGHT;
	  	  AdcHandle.Init.ScanConvMode          = ADC_SCAN_DIRECTION_FORWARD;    /* Sequencer will convert the number of channels configured below, successively from the lowest to the highest channel number */
	  	  AdcHandle.Init.EOCSelection          = ADC_EOC_SINGLE_CONV;
	  	  AdcHandle.Init.LowPowerAutoWait      = DISABLE;
	  	  AdcHandle.Init.LowPowerAutoPowerOff  = DISABLE;
	  	  AdcHandle.Init.ContinuousConvMode    = DISABLE;                       /* Continuous mode disabled to have only 1 rank converted at each conversion trig, and because discontinuous mode is enabled */
	  	  AdcHandle.Init.DiscontinuousConvMode = ENABLE;                        /* Sequencer of regular group will convert the sequence in several sub-divided sequences */
	  	  AdcHandle.Init.ExternalTrigConv      = ADC_SOFTWARE_START;            /* Software start to trig the 1st conversion manually, without external event */
	  	  AdcHandle.Init.ExternalTrigConvEdge  = ADC_EXTERNALTRIGCONVEDGE_NONE; /* Parameter discarded because trig of conversion by software start (no external event) */
	  	  AdcHandle.Init.DMAContinuousRequests = ENABLE;                        /* ADC-DMA continuous requests to match with DMA configured in circular mode */
	  	  AdcHandle.Init.Overrun               = ADC_OVR_DATA_OVERWRITTEN;
	  	  /* Note: Set long sampling time due to internal channels (VrefInt,          */
	  	  /*       temperature sensor) constraints. Refer to device datasheet for     */
	  	  /*       min/typ/max values.                                                */
	  	  AdcHandle.Init.SamplingTimeCommon    = ADC_SAMPLETIME_1CYCLE_5;

	  	  if (HAL_ADC_Init(&AdcHandle) != HAL_OK)
	  	  {
	  	    /* ADC initialization error */
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

	  	  if (HAL_ADC_ConfigChannel(&AdcHandle, &sConfig) != HAL_OK)
	  	  {
	  	    /* Channel Configuration Error */
	  	    Error_Handler();
	  	  }





	  	  /* Run the ADC calibration */
	  	  if (HAL_ADCEx_Calibration_Start(&AdcHandle) != HAL_OK)
	  	  {
	  	    /* Calibration Error */
	  	    Error_Handler();
	  	  }

	  	  /* Start ADC conversion on regular group with transfer by DMA */
	  	  if (HAL_ADC_Start_DMA(&AdcHandle,
	  	                        (uint32_t *)aADCxConvertedValues,
	  	                        ADCCONVERTEDVALUES_BUFFER_SIZE
	  	                       ) != HAL_OK)
	  	  {
	  	    /* Start Error */
	  	    Error_Handler();
	  	  }


 }


 void adc_sample(void){
	    /* Start ADC conversion */
	    /* Since sequencer is enabled in discontinuous mode, this will perform    */
	    /* the conversion of the next rank in sequencer.                          */
	    /* Note: For this example, conversion is triggered by software start,     */
	    /*       therefore "HAL_ADC_Start()" must be called for each conversion.  */
	    /*       Since DMA transfer has been initiated previously by function     */
	    /*       "HAL_ADC_Start_DMA()", this function will keep DMA transfer      */
	    /*       active.                                                          */
	 	ubSequenceCompleted = RESET;

	 	//HAL_Delay(100);

	 	//while(HAL_ADC_GetState(&AdcHandle) != HAL_ADC_STATE_READY);

	    if (HAL_ADC_Start(&AdcHandle) != HAL_OK)
	    {
	      //Error_Handler();
	    }

	    //while(ubSequenceCompleted != SET){
	    //	HAL_Delay(1);
	    //}
 }

 uint32_t* adc_getRandomTemperatures(void){
	 static uint32_t buffer[8];

	 general_adc_init_inttemp();

	 for(uint8_t i=0; i<8; i++){
		 adc_sample();
		 buffer[i] = aADCxConvertedValues[0];
	 }

	 return buffer;

 }

 uint16_t adc_getTemperature(void){
	    adc_sample();

	    return aADCxConvertedValues[2];
 }

 uint16_t adc_getVoltageInternal(void){
	    adc_sample();

	    return aADCxConvertedValues[3];
 }

 uint16_t adc_getADC1(void){
	    adc_sample();

	    return aADCxConvertedValues[0];
 }

 uint16_t adc_getADC2(void){
	    adc_sample();

	    return aADCxConvertedValues[1];
 }
