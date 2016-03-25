/*
 * application_rtc.c
 *
 *  Created on: 28-feb.-2016
 *      Author: Robbe
 */
#include "application_rtc.h"

RTC_HandleTypeDef RtcHandle;

/* Buffers used for displaying Time and Date */
uint8_t aShowTime[50] = {0};
uint8_t aShowDate[50] = {0};

void rtc_calenderconfig(void){
	  RTC_DateTypeDef sdatestructure;
	  RTC_TimeTypeDef stimestructure;

	  /*##-1- Configure the Date #################################################*/
	  /* Set Date: Tuesday February 18th 2014 */
	  sdatestructure.Year = 0x14;
	  sdatestructure.Month = RTC_MONTH_FEBRUARY;
	  sdatestructure.Date = 0x18;
	  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;

	  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Configure the Time #################################################*/
	  /* Set Time: 02:00:00 */
	  stimestructure.Hours = 0x02;
	  stimestructure.Minutes = 0x00;
	  stimestructure.Seconds = 0x00;
	  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
	  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
	  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;

	  if (HAL_RTC_SetTime(&RtcHandle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
	  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR0, 0x32F2);
}
void rtc_calenderprint(uint8_t *showtime, uint8_t *showdate){
	  RTC_DateTypeDef sdatestructureget;
	  RTC_TimeTypeDef stimestructureget;

	  /* Get the RTC current Time */
	  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	  /* Get the RTC current Date */
	  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	  /* Display time Format : hh:mm:ss */
	  sprintf((char *)showtime, "%2d:%2d:%2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
	  /* Display date Format : mm-dd-yy */
	  sprintf((char *)showdate, "%2d-%2d-%2d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);

	  uart_sendPrintf((char *)showtime);
	  uart_sendPrintf("\r\n");
	  uart_sendPrintf((char *)showdate);
	  uart_sendPrintf("\r\n");
}

void general_rtc_init(void){
	/*##-1- Configure the RTC peripheral #######################################*/
	  /* Configure RTC prescaler and RTC data registers */
	  /* RTC configured as follows:
	      - Hour Format    = Format 24
	      - Asynch Prediv  = Value according to source clock
	      - Synch Prediv   = Value according to source clock
	      - OutPut         = Output Disable
	      - OutPutPolarity = High Polarity
	      - OutPutType     = Open Drain */
	  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
	  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
	  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
	  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
	  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	  RtcHandle.Instance = RTC;

	  if (HAL_RTC_Init(&RtcHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
	  /* Read the Back Up Register 0 Data */
	  if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR0) != 0x32F2)
	  {
	    /* Configure RTC Calendar */
	    rtc_calenderconfig();
	  }
	  else
	  {
	    /* Check if the Power On Reset flag is set */
	    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
	    {
	      /* Turn on LED2: Power on reset occured */
	      uart_sendPrintf("power on reset occured\r\n");
	    }
	    /* Check if Pin Reset flag is set */
	    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
	    {
	      /* Turn on LED4: External reset occured */
	    	uart_sendPrintf("external reset occured\r\n");
	    }
	    /* Clear source Reset Flag */
	    __HAL_RCC_CLEAR_RESET_FLAGS();
	  }
}
