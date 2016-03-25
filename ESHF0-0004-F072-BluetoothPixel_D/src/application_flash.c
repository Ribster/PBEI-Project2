/*
 * application_flash.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */
#include "application_flash.h"
#include "application_general.h"

// flash data
application_parameters flash_applicationParams;

SPI_HandleTypeDef SpiHandle;

uint8_t FLASH_txbuffer[270];
uint8_t FLASH_rxbuffer[270];

void flash_initParams(void){
	flash_applicationParams.MCUVersion = 160301;

	flash_applicationParams.flash_btParams.bt_rgbcount = 10;
	flash_applicationParams.flash_btParams.bt_rgbmax = 150;
	flash_applicationParams.flash_btParams.bt_analog1 = 0;
	flash_applicationParams.flash_btParams.bt_temp = 0;
	flash_applicationParams.flash_btParams.bt_animationSelected = true;
	flash_applicationParams.flash_btParams.bt_animationNumber = 1;
	flash_applicationParams.flash_btParams.bt_colorPattern = 1;
	flash_applicationParams.flash_btParams.bt_printfEnabled = true;
}

void flash_synchronizeData(void){
	stm32dev_flash_s25fl208_waitForProgrammingDone();

	// read mcu version from flash
	application_parameters app_params = flash_readApplicationParams();


	uart_sendPrintf("MCU version: ");
	uart_sendPrintf(stm32dev_general_getDec(flash_applicationParams.MCUVersion));
	uart_sendPrintf(", flash MCU version: ");
	uart_sendPrintf(stm32dev_general_getDec(app_params.MCUVersion));
	uart_sendPrintf("\r\n");

	uart_sendPrintf("Size of flash_applicationParams: ");
	uart_sendPrintf(stm32dev_general_getDec(sizeof(flash_applicationParams)));
	uart_sendPrintf("\r\n");

	// check if the mcu version matches
	if(app_params.MCUVersion == flash_applicationParams.MCUVersion){
		// version is the same
		flash_applicationParams = app_params;
		uart_sendPrintf("Same MCU version on Flash\r\n");
	} else {
		// version is different
		uart_sendPrintf("New MCU version detected!\r\n");
		flash_writeData();
	}
}


application_parameters flash_readApplicationParams(void){
	application_parameters retVal;
	uint8_t* startData = stm32dev_flash_s25fl208_sendCommand_readData(0, sizeof(application_parameters));
	memcpy(&retVal, startData, sizeof(application_parameters));
	return retVal;
}

bt_parameters flash_readBtParams(void){
	bt_parameters retVal;
	uint8_t* startData = stm32dev_flash_s25fl208_sendCommand_readData(sizeof(application_parameters)+1, sizeof(bt_parameters));
	memcpy(&retVal, startData, sizeof(bt_parameters));
	return retVal;
}

void flash_writeData(void){

	uint8_t* appParams = (uint8_t*)&flash_applicationParams;

	for(uint8_t i=0; i<20; i++){
		uart_sendPrintf("flash_applicationParams[");
		uart_sendPrintf(stm32dev_general_getDec(i));
		uart_sendPrintf("] ");
		uart_sendPrintf(stm32dev_general_getHex8(appParams[i]));
		uart_sendPrintf("\r\n");
	}

    stm32dev_flash_s25fl208_sendCommand_pageProgram(0, sizeof(application_parameters), appParams);
    stm32dev_flash_s25fl208_waitForProgrammingDone();
}

void general_spiFlash_init(void){

	static GPIO_InitTypeDef  GPIO_InitStruct;

    SpiHandle.Instance               = SPIx;

    SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
    SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    SpiHandle.Init.CRCPolynomial     = 7;
    SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
    SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    SpiHandle.Init.NSS               = SPI_NSS_SOFT;
    SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
    SpiHandle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
    SpiHandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
    SpiHandle.Init.Mode = SPI_MODE_MASTER;

    uart_sendPrintf("Starting SPI flash init");

    if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
    {
      /* Initialization Error */
      Error_Handler();
    }

    // init the spi flash
    	SPIx_CS_GPIO_CLK_ENABLE();
    	SPIx_WP_GPIO_CLK_ENABLE();
    	SPIx_HOLD_GPIO_CLK_ENABLE();

		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		// init the HOLD
		GPIO_InitStruct.Pin = SPIFLASH_HOLD_PIN;
		HAL_GPIO_Init(SPIFLASH_HOLD_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(SPIFLASH_HOLD_PORT,SPIFLASH_HOLD_PIN,GPIO_PIN_SET);

    	// init the CS
		GPIO_InitStruct.Pin = SPIFLASH_CS_PIN;
		HAL_GPIO_Init(SPIFLASH_CS_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(SPIFLASH_CS_PORT,SPIFLASH_CS_PIN,GPIO_PIN_SET);

    	// init the WP
		GPIO_InitStruct.Pin = SPIFLASH_WP_PIN;
		HAL_GPIO_Init(SPIFLASH_WP_PORT, &GPIO_InitStruct);
		HAL_GPIO_WritePin(SPIFLASH_WP_PORT,SPIFLASH_WP_PIN,GPIO_PIN_SET);





	stm32dev_flash_s25fl208_setPrintfRoutine(uart_sendPrintf);

	uart_sendPrintf("...init");
    stm32dev_flash_s25fl208_init(&SpiHandle, FLASH_rxbuffer, FLASH_txbuffer, 270, 270, SPIFLASH_CS_PORT, SPIFLASH_CS_PIN);
    uart_sendPrintf("...ok");

    uart_sendPrintf("...writestatus");
    stm32dev_flash_s25fl208_sendCommand_releaseDeepPowerDown();
    uart_sendPrintf("...");
    stm32dev_flash_s25fl208_sendCommand_writeStatusRegister(0x00);
    uart_sendPrintf("...ok");

    //stm32dev_flash_s25fl208_print_deviceInfo(stm32dev_flash_s25fl208_sendCommand_readDeviceIdentifiers());
    //stm32dev_flash_s25fl208_print_statusRegister(stm32dev_flash_s25fl208_sendCommand_readStatusRegister());
    //stm32dev_flash_s25fl208_testSPI();
    //stm32dev_flash_s25fl208_sendCommand_chipErase();
    stm32dev_flash_s25fl208_waitForProgrammingDone();

    uart_sendPrintf("...programmingdone");
    flash_synchronizeData();

    //char textProgram[] = "This is a test!";

    //uint8_t textProgram[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,0};

    //stm32dev_flash_s25fl208_sendCommand_pageProgram(0, sizeof(textProgram), (uint8_t*)textProgram);
    //stm32dev_flash_s25fl208_printSPIBuffer(4);
    //stm32dev_flash_s25fl208_printSPIDataBuffer(sizeof(textProgram), textProgram, 4);
    //stm32dev_flash_s25fl208_waitForProgrammingDone();
    //stm32dev_flash_s25fl208_print_memoryBlock(0,64);

    //stm32dev_flash_s25fl208_print_memoryAll();

    uart_sendPrintf("... Done\r\n");
}
