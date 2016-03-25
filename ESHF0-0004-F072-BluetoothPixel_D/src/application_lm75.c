/*
 * application_lm75.c
 *
 *  Created on: 26-feb.-2016
 *      Author: Robbe
 */
#include "application_lm75.h"
#include "application_general.h"

I2C_HandleTypeDef I2cHandle;

void lm75_writeReg_8bit(uint8_t reg, uint8_t value){

	stm32dev_i2c_write_polling(&I2cHandle, I2C_ADDRESS, reg, value);

}

void lm75_writeReg_16bit(uint8_t reg, uint16_t value){

	uint8_t data[2];
	data[0] = (value >> 8) & 0xFF;
	data[1] = value & 0xFF;

	stm32dev_i2c_writeMulti_polling(&I2cHandle, I2C_ADDRESS, reg, data, 2);

}

void lm75_setPointerReg(uint8_t pointerReg){
	stm32dev_i2c_write_noRegister_polling(&I2cHandle, I2C_ADDRESS, pointerReg);
}

uint8_t lm75_readReg_8bit(uint8_t reg){

	lm75_setPointerReg(reg);

	uint8_t data = 0;
	stm32dev_i2c_read_noRegister_polling(&I2cHandle, I2C_ADDRESS, &data);

	//uart_sendPrintf("readData8: ");uart_sendPrintf(stm32dev_general_getDec(data));uart_sendPrintf("\r\n");

	return data;
}

uint16_t lm75_readReg_16bit(uint8_t reg){
	uint16_t retVal;
	uint8_t data[] = {0,0};

	lm75_setPointerReg(reg);

	stm32dev_i2c_readMulti_noRegister_polling(&I2cHandle, I2C_ADDRESS, data, 2);

	//uart_sendPrintf("readData16: ");uart_sendPrintf(stm32dev_general_getDec(data[0]));uart_sendPrintf(", ");uart_sendPrintf(stm32dev_general_getDec(data[1]));uart_sendPrintf("\r\n");

	retVal = data[0];
	retVal = (retVal << 8) | data[1];

	return retVal;
}

void lm75_testRegisters(void){
	uint8_t i;
	for(i=0; i<8; i++){
		uart_sendPrintf("register ");uart_sendPrintf(stm32dev_general_getDec(i));uart_sendPrintf(" read: ");uart_sendPrintf(stm32dev_general_getDec(lm75_readReg_8bit(i)));uart_sendPrintf("\r\n");
	}

	for(i=0; i<8; i++){
		uart_sendPrintf("register ");uart_sendPrintf(stm32dev_general_getDec(i));uart_sendPrintf(" read: ");uart_sendPrintf(stm32dev_general_getDec(lm75_readReg_16bit(i)));uart_sendPrintf("\r\n");
	}
}




uint8_t lm75_readConf(void){
	return lm75_readReg_8bit(LM75_REG_CONF);
}

uint16_t lm75_readTemp(void){
	return lm75_readReg_16bit(LM75_REG_TEMP);
}

uint16_t lm75_readHys(void){
	return lm75_readReg_16bit(LM75_REG_THYS);
}

uint16_t lm75_readTos(void){
	return lm75_readReg_16bit(LM75_REG_TOS);
}

void lm75_writeConf(uint8_t value){
	lm75_writeReg_8bit(LM75_REG_CONF, value);
}

void lm75_writeTemp(uint16_t value){
	lm75_writeReg_16bit(LM75_REG_TEMP, value);
}

void lm75_writeHys(uint16_t value){
	lm75_writeReg_16bit(LM75_REG_THYS, value);
}

void lm75_writeTos(uint16_t value){
	lm75_writeReg_16bit(LM75_REG_TOS, value);
}

// application
void lm75_shutdown(FunctionalState newstate){
	uint8_t value = lm75_readConf();
	lm75_writeConf(newstate == ENABLE? value | 0x01 : value & 0xFE);
}

int16_t lm75_temperature(void){
	uint16_t raw;
	int16_t temp;

	raw = lm75_readTemp() >> 7;
	if (raw & 0x0100) {
		// Negative temperature
		temp = -10 * (((~(uint8_t)(raw & 0xFE) + 1) & 0x7F) >> 1) - (raw & 0x01) * 5;
	} else {
		// Positive temperature
		temp = ((raw & 0xFE) >> 1) * 10 + (raw & 0x01) * 5;
	}

	return temp;
}

bool lm75_present(void){
	return stm32dev_i2c_isDeviceConnected(&I2cHandle, I2C_ADDRESS);
}

void general_lm75_init(void){
	  /*##-1- Configure the I2C peripheral ######################################*/
	  I2cHandle.Instance             = I2Cx;

	  I2cHandle.Init.Timing          = I2C_TIMING;
	  I2cHandle.Init.OwnAddress1     = 0xFF;
	  I2cHandle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	  I2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	  I2cHandle.Init.OwnAddress2     = 0xFF;
	  I2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	  I2cHandle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	  if(HAL_I2C_Init(&I2cHandle) != HAL_OK)
	  {
	    /* Initialization Error */
	    Error_Handler();
	  }

	  /* Enable the Analog I2C Filter */
	  HAL_I2CEx_ConfigAnalogFilter(&I2cHandle,I2C_ANALOGFILTER_ENABLE);


	  uart_sendPrintf("LM75 sensor present: ");
	  if(lm75_present()){
		  uart_sendPrintf("YES!");
	  } else {
		  uart_sendPrintf("NO!");
	  }
	  uart_sendPrintf("\r\n");

	  //lm75_testRegisters();

	  uint8_t i = 0;


	  for (i=0; i<1; i++){
		  uint8_t conf = lm75_readConf();
		  uart_sendPrintf("Conf: ");uart_sendPrintf(stm32dev_general_getDec(conf));uart_sendPrintf("\r\n");
	  }

	  for (i=0; i<1; i++){
		  uint16_t hys = lm75_readHys();
		  uart_sendPrintf("Hys: ");uart_sendPrintf(stm32dev_general_getDec(hys));uart_sendPrintf("\r\n");
	  }

	  for (i=0; i<1; i++){
		  uint16_t temp = lm75_readTemp();
		  uart_sendPrintf("Temp: ");uart_sendPrintf(stm32dev_general_getDec(temp));uart_sendPrintf("\r\n");
	  }

	  for (i=0; i<1; i++){
		  uint16_t tos = lm75_readTos();
		  uart_sendPrintf("Tos: ");uart_sendPrintf(stm32dev_general_getDec(tos));uart_sendPrintf("\r\n");
	  }

	  uart_sendPrintf("Temperature: ");uart_sendPrintf(stm32dev_general_getDec(lm75_temperature()));uart_sendPrintf("\r\n");
}
