#include "application_rtc.h"
#include "application_general.h"

RTC_HandleTypeDef application_rtc_handle;

uint8_t application_rtc_aShowTime[50] = {0};
uint8_t application_rtc_aShowDate[50] = {0};

bool application_rtc_porOccured = false;
bool application_rtc_exrOccured = false;

void application_rtc_init(void){
	  /*##-1- Configure the RTC peripheral #######################################*/
	  /* Configure RTC prescaler and RTC data registers */
	  /* RTC configured as follows:
	      - Hour Format    = Format 24
	      - Asynch Prediv  = Value according to source clock
	      - Synch Prediv   = Value according to source clock
	      - OutPut         = Output Disable
	      - OutPutPolarity = High Polarity
	      - OutPutType     = Open Drain */
	  application_rtc_handle.Init.HourFormat = RTC_HOURFORMAT_24;
	  application_rtc_handle.Init.AsynchPrediv = 0x7F;
	  application_rtc_handle.Init.SynchPrediv = 0x00FF;
	  application_rtc_handle.Init.OutPut = RTC_OUTPUT_DISABLE;
	  application_rtc_handle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	  application_rtc_handle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	  application_rtc_handle.Instance = RTC;

	  if (HAL_RTC_Init(&application_rtc_handle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /*##-2- Check if Data stored in BackUp register0: No Need to reconfigure RTC#*/
	  /* Read the Back Up Register 0 Data */
	  if (HAL_RTCEx_BKUPRead(&application_rtc_handle, RTC_BKP_DR0) != 0x32F2)
	  {
	    /* Configure RTC Calendar */
	    application_rtc_calenderconfig();
	  }
	  else
	  {
	    /* Check if the Power On Reset flag is set */
	    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST) != RESET)
	    {
	      /* Turn on LED2: Power on reset occured */
	    	application_rtc_porOccured = true;
	    }
	    /* Check if Pin Reset flag is set */
	    if (__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST) != RESET)
	    {
	      /* Turn on LED4: External reset occured */
	    	application_rtc_exrOccured = true;
	    }
	    /* Clear source Reset Flag */
	    __HAL_RCC_CLEAR_RESET_FLAGS();
	  }
}

void application_rtc_calenderconfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;

  /*##-1- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 0x14;
  sdatestructure.Month = RTC_MONTH_FEBRUARY;
  sdatestructure.Date = 0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;

  if(HAL_RTC_SetDate(&application_rtc_handle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
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

  if (HAL_RTC_SetTime(&application_rtc_handle, &stimestructure, RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*##-3- Writes a data in a RTC Backup data Register0 #######################*/
  HAL_RTCEx_BKUPWrite(&application_rtc_handle, RTC_BKP_DR0, 0x32F2);
}

void application_rtc_calendarshow(uint8_t *showtime, uint8_t *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&application_rtc_handle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&application_rtc_handle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%2d:%2d:%2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "%2d-%2d-%2d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year);
}

void application_rtc_calendarshow_shiftreg(uint8_t *showtime, uint8_t *showdate)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&application_rtc_handle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&application_rtc_handle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char *)showtime, "%02d%02d", stimestructureget.Hours, stimestructureget.Minutes);
  /* Display date Format : mm-dd-yy */
  sprintf((char *)showdate, "%02d%02d", sdatestructureget.Date, sdatestructureget.Month);
}
