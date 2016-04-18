#include "application_spiflash.h"
#include "application_general.h"
#include "application_uart.h"

SPI_HandleTypeDef application_spiflash_handle;

uint8_t application_spiflash_txbuffer[270];
uint8_t application_spiflash_rxbuffer[270];

void application_spiflash_init(void){
	static GPIO_InitTypeDef  GPIO_InitStruct;

	    application_spiflash_handle.Instance               = SPI1;

	    application_spiflash_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	    application_spiflash_handle.Init.Direction         = SPI_DIRECTION_2LINES;
	    application_spiflash_handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	    application_spiflash_handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	    application_spiflash_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	    application_spiflash_handle.Init.CRCPolynomial     = 7;
	    application_spiflash_handle.Init.DataSize          = SPI_DATASIZE_8BIT;
	    application_spiflash_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	    application_spiflash_handle.Init.NSS               = SPI_NSS_SOFT;
	    application_spiflash_handle.Init.TIMode            = SPI_TIMODE_DISABLE;
	    application_spiflash_handle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
	    application_spiflash_handle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
	    application_spiflash_handle.Init.Mode = SPI_MODE_MASTER;

	    application_uart_transmit("Starting SPI flash init");

	    if(HAL_SPI_Init(&application_spiflash_handle) != HAL_OK)
	    {
	      /* Initialization Error */
	      Error_Handler();
	    }


	  	//	14			AF0	PA4				I/O		SPI1_NSS		SPIFLASH_CS		TO DO
	  	//	15			AF0	PA5				I/O		SPI1_SCK		SPIFLASH_SCK	TO DO
	  	//	17			AF0	PA7				I/O		SPI1_MOSI		SPIFLASH_MOSI	TO DO
	  	//	18				PB0				Output	GPIO_Output		SPIFLASH_WP		TO DO
	  	//	19				PB1				Output	GPIO_Output		SPIFLASH_HOLD	TO DO
	  	//	40			AF0	PB4				I/O		SPI1_MISO		SPIFLASH_MISO	TO DO

	    // init the spi flash
	    __HAL_RCC_GPIOA_CLK_ENABLE();
	    __HAL_RCC_GPIOB_CLK_ENABLE();

			GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
			GPIO_InitStruct.Pull  = GPIO_NOPULL;
			GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

			// init the HOLD
			GPIO_InitStruct.Pin = GPIO_PIN_1;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);

	    	// init the CS
			GPIO_InitStruct.Pin = GPIO_PIN_4;
			HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4,GPIO_PIN_SET);

	    	// init the WP
			GPIO_InitStruct.Pin = GPIO_PIN_0;
			HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);





		stm32dev_flash_s25fl208_setPrintfRoutine(application_uart_transmit);

		application_uart_transmit("...init");
	    stm32dev_flash_s25fl208_init(&application_spiflash_handle, application_spiflash_rxbuffer, application_spiflash_txbuffer, 270, 270, GPIOA, GPIO_PIN_4);



	    stm32dev_flash_s25fl208_sendCommand_releaseDeepPowerDown();

	    //stm32dev_flash_s25fl208_testSPI();

	    //stm32dev_flash_s25fl208_sendCommand_writeStatusRegister(0x00);

	    stm32dev_flash_s25fl208_waitForProgrammingDone();

	    //flash_synchronizeData();

	    application_uart_transmit("... Done\r\n");
}

void application_spiflash_synchronizedata(void){
	stm32dev_flash_s25fl208_waitForProgrammingDone();
}
