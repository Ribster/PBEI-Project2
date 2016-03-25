/*
 * application_rng.c
 *
 *  Created on: 8-mrt.-2016
 *      Author: Robbe
 */

#include "application_printf.h"
#include "application_rng.h"

CRC_HandleTypeDef   CrcHandle;

void general_crc_init(void){
	/*##-1- Configure the CRC peripheral #######################################*/
	  CrcHandle.Instance = CRC;

	  /* The default polynomial is used */
	  CrcHandle.Init.DefaultPolynomialUse    = DEFAULT_POLYNOMIAL_ENABLE;

	  /* The default init value is used */
	  CrcHandle.Init.DefaultInitValueUse     = DEFAULT_INIT_VALUE_ENABLE;

	  /* The input data are not inverted */
	  CrcHandle.Init.InputDataInversionMode  = CRC_INPUTDATA_INVERSION_NONE;

	  /* The output data are not inverted */
	  CrcHandle.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;

	  /* The input data are 32 bits lenght */
	  CrcHandle.InputDataFormat              = CRC_INPUTDATA_FORMAT_WORDS;

	  if (HAL_CRC_Init(&CrcHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }
}

uint32_t rng_getRandom(void){
	uint32_t *tmp = adc_getRandomTemperatures();

	return HAL_CRC_Accumulate(&CrcHandle, tmp, 8);
}
