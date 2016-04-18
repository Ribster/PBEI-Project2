#include "application_lm75.h"
#include "application_general.h"

I2C_HandleTypeDef application_lm75_handle;
bool application_lm75_present_bool = false;

void application_lm75_init(void){
		  /*##-1- Configure the I2C peripheral ######################################*/
	  application_lm75_handle.Instance             = I2C1;

	  application_lm75_handle.Init.Timing          = 0x00A51314;
	  application_lm75_handle.Init.OwnAddress1     = 0xFF;
	  application_lm75_handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	  application_lm75_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  application_lm75_handle.Init.OwnAddress2     = 0xFF;
	  application_lm75_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  application_lm75_handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	  if(HAL_I2C_Init(&application_lm75_handle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /* Enable the Analog I2C Filter */
	  HAL_I2CEx_ConfigAnalogFilter(&application_lm75_handle,I2C_ANALOGFILTER_ENABLE);

	  application_lm75_present_bool = application_lm75_present();
}

bool application_lm75_present(void){
	return stm32dev_i2c_isDeviceConnected(&application_lm75_handle, 0x90);
}

void application_lm75_setPointerReg(uint8_t pointerReg){
	stm32dev_i2c_write_noRegister_polling(&application_lm75_handle, 0x90, pointerReg);
}

void application_lm75_writeReg_8bit(uint8_t reg, uint8_t value){

	stm32dev_i2c_write_polling(&application_lm75_handle, 0x90, reg, value);

}

void application_lm75_writeReg_16bit(uint8_t reg, uint16_t value){

	uint8_t data[2];
	data[0] = (value >> 8) & 0xFF;
	data[1] = value & 0xFF;

	stm32dev_i2c_writeMulti_polling(&application_lm75_handle, 0x90, reg, data, 2);

}

uint8_t application_lm75_readReg_8bit(uint8_t reg){

	application_lm75_setPointerReg(reg);

	uint8_t data = 0;
	stm32dev_i2c_read_noRegister_polling(&application_lm75_handle, 0x90, &data);

	//uart_sendPrintf("readData8: ");uart_sendPrintf(stm32dev_general_getDec(data));uart_sendPrintf("\r\n");

	return data;
}

uint16_t application_lm75_readReg_16bit(uint8_t reg){
	uint16_t retVal;
	uint8_t data[] = {0,0};

	application_lm75_setPointerReg(reg);

	stm32dev_i2c_readMulti_noRegister_polling(&application_lm75_handle, 0x90, data, 2);

	//uart_sendPrintf("readData16: ");uart_sendPrintf(stm32dev_general_getDec(data[0]));uart_sendPrintf(", ");uart_sendPrintf(stm32dev_general_getDec(data[1]));uart_sendPrintf("\r\n");

	retVal = data[0];
	retVal = (retVal << 8) | data[1];

	return retVal;
}

uint8_t application_lm75_readConf(void){
	return application_lm75_readReg_8bit(0x01);
}

uint16_t application_lm75_readTemp(void){
	return application_lm75_readReg_16bit(0x00);
}

uint16_t application_lm75_readHys(void){
	return application_lm75_readReg_16bit(0x02);
}

uint16_t application_lm75_readTos(void){
	return application_lm75_readReg_16bit(0x03);
}

void application_lm75_writeConf(uint8_t value){
	application_lm75_writeReg_8bit(0x01, value);
}

void application_lm75_writeTemp(uint16_t value){
	application_lm75_writeReg_16bit(0x00, value);
}

void application_lm75_writeHys(uint16_t value){
	application_lm75_writeReg_16bit(0x02, value);
}

void application_lm75_writeTos(uint16_t value){
	application_lm75_writeReg_16bit(0x03, value);
}

void application_lm75_shutdown(FunctionalState newstate){
	uint8_t value = application_lm75_readConf();
	application_lm75_writeConf(newstate == ENABLE? value | 0x01 : value & 0xFE);
}

int16_t application_lm75_temperature(void){
	uint16_t raw;
	int16_t temp;

	raw = application_lm75_readTemp() >> 7;
	if (raw & 0x0100) {
		// Negative temperature
		temp = -10 * (((~(uint8_t)(raw & 0xFE) + 1) & 0x7F) >> 1) - (raw & 0x01) * 5;
	} else {
		// Positive temperature
		temp = ((raw & 0xFE) >> 1) * 10 + (raw & 0x01) * 5;
	}

	return temp;
}
