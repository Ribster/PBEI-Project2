#include "application_shiftreg.h"
#include "application_uart.h"

uint8_t application_shiftreg_hc595Values[3];
uint8_t application_shiftreg_hc165Values[3];
SPI_HandleTypeDef application_shiftreg_spihandle;
char application_shiftreg_asciiptr[16];
TIM_HandleTypeDef    application_shiftreg_timhandle;
uint32_t application_shiftreg_uwPrescalerValue = 0;
uint8_t application_shiftreg_scanDigit = 0;
uint8_t application_shiftreg_decPoint = 0;
bool application_shiftreg_nulOut = false;

void application_shiftreg_init(void){

	GPIO_InitTypeDef  GPIO_InitStruct;

	// HC165_LOAD
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		GPIO_InitStruct.Pin = GPIO_PIN_12;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);

	// HC595_LATCH
		__HAL_RCC_GPIOA_CLK_ENABLE();
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);


		application_shiftreg_spihandle.Instance               = SPI2;

		application_shiftreg_spihandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
		application_shiftreg_spihandle.Init.Direction         = SPI_DIRECTION_2LINES;
		application_shiftreg_spihandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
		application_shiftreg_spihandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
		application_shiftreg_spihandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
		application_shiftreg_spihandle.Init.CRCPolynomial     = 7;
		application_shiftreg_spihandle.Init.DataSize          = SPI_DATASIZE_8BIT;
		application_shiftreg_spihandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
		application_shiftreg_spihandle.Init.NSS               = SPI_NSS_SOFT;
		application_shiftreg_spihandle.Init.TIMode            = SPI_TIMODE_DISABLE;
		application_shiftreg_spihandle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
		application_shiftreg_spihandle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
		application_shiftreg_spihandle.Init.Mode = SPI_MODE_MASTER;

		if(HAL_SPI_Init(&application_shiftreg_spihandle) != HAL_OK)
		{
		/* Initialization Error */
		Error_Handler();
		}



		// update timer
		  /* Compute the prescaler value to have TIMx counter clock equal to 10000 Hz */
		application_shiftreg_uwPrescalerValue = (uint32_t)(SystemCoreClock / 100) - 1;

		  /* Set TIMx instance */
		  application_shiftreg_timhandle.Instance = TIM6;

		  /* Initialize TIMx peripheral as follows:
		       + Period = 10000 - 1
		       + Prescaler = (SystemCoreClock/10000) - 1
		       + ClockDivision = 0
		       + Counter direction = Up
		  */
		  application_shiftreg_timhandle.Init.Period            = 10 - 1;
		  application_shiftreg_timhandle.Init.Prescaler         = application_shiftreg_uwPrescalerValue;
		  application_shiftreg_timhandle.Init.ClockDivision     = 0;
		  application_shiftreg_timhandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
		  application_shiftreg_timhandle.Init.RepetitionCounter = 0;
		  if (HAL_TIM_Base_Init(&application_shiftreg_timhandle) != HAL_OK)
		  {
		    /* Initialization Error */
		    Error_Handler();
		  }

		  /*##-2- Start the TIM Base generation in interrupt mode ####################*/
		  /* Start Channel1 */
		  if (HAL_TIM_Base_Start_IT(&application_shiftreg_timhandle) != HAL_OK)
		  {
		    /* Starting Error */
		    Error_Handler();
		  }
}

void application_shiftreg_timerInterrupt(void){
	if(application_shiftreg_scanDigit >= 4) application_shiftreg_scanDigit =0;

	application_shiftreg_setSegment(application_shiftreg_scanDigit+1);
	if(*(application_shiftreg_asciiptr+application_shiftreg_scanDigit) != 0){
		application_shiftreg_setChar(*(application_shiftreg_asciiptr+application_shiftreg_scanDigit));
	} else {

		if(application_shiftreg_nulOut){
			application_shiftreg_setChar('0');
		} else {
			application_shiftreg_setChar(' ');
		}
	}

	if(application_shiftreg_decPoint == (application_shiftreg_scanDigit+1)){
		application_shiftreg_addHex(0x80);
	}

	application_shiftreg_update();
	application_shiftreg_scanDigit++;
}


void application_shiftreg_update(void){

	// LATCH
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);

	// LOAD
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_12,GPIO_PIN_SET);

	if(HAL_SPI_TransmitReceive(&application_shiftreg_spihandle, (uint8_t*)application_shiftreg_hc595Values, (uint8_t *)application_shiftreg_hc165Values, 3, 100) == HAL_OK){

	}

	// LATCH
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
}


void application_shiftreg_testSequence(void){
	for(uint8_t j=0; j<3; j++){
		application_shiftreg_hc595Values[j] = 0x00;
	}


	for(uint32_t i = 0; i<9999; i+=10){
		application_shiftreg_setValue_double(i);
		HAL_Delay(100);
	}

//	for(uint8_t j=0; j<3; j++){
//		for(uint8_t i=0; i<8; i++){
//			application_shiftreg_hc595Values[j] |= (1<<i);
//			application_shiftreg_update();
//			HAL_Delay(1000);
//			application_uart_transmit("register ");
//			application_uart_transmit(stm32dev_general_getDec(j+1));
//			application_uart_transmit(" bitpos ");
//			application_uart_transmit(stm32dev_general_getDec(i));
//			application_uart_transmit("\r\n");
//		}
//	}

	//application_shiftreg_hc595Values[2] = 0x03;
	//application_shiftreg_hc595Values[1] = 0x03;

//	application_shiftreg_update();
//	HAL_Delay(200);

//		for(uint8_t j=1; j<5; j++){
//			application_shiftreg_setSegment(j);
//			for(uint8_t i=0; i<8; i++){
//				application_shiftreg_setHex(1<<i);
//				application_shiftreg_update();
//				HAL_Delay(200);
//			}
//		}
//
//	for(uint8_t j=1; j<5; j++){
//		application_shiftreg_setLed(j, true);
//		HAL_Delay(600);
//	}
//
//	for(uint8_t j=1; j<5; j++){
//		application_shiftreg_setLed(j, false);
//		HAL_Delay(600);
//	}
}

void application_shiftreg_setValue_decimal(uint32_t newVal){
	// 1st shiftreg
		// A>H 0x80 -> 0x01
		//	A:	1st dig
		//	B:	2nd dig
		//	C:	seg a
		//	D:	seg b
		//	E:	seg c
		//	F:	seg d
		//	G:	seg e
		//	H:	seg f
	// 2nd shiftreg
		// A>H 0x80 -> 0x01
		//	A:	3rd dig
		//	B:	4th dig
		//	C:	seg g
		//	D:	seg dp
		//	E:	LED
		//	F:	LED
		//	G:	LED
		//	H:	LED
	memcpy(application_shiftreg_asciiptr, stm32dev_general_getDecSigned(newVal), 16);
	application_shiftreg_decPoint = 0;
}

void application_shiftreg_setValue_double(uint32_t newVal){
	memset(application_shiftreg_asciiptr,0,16);

	uint32_t top = (newVal / 100);
	uint8_t bottom = newVal - top*100;
	bool shiftRight_bottom = false;


//	application_uart_transmit("top: ");
//	application_uart_transmit(stm32dev_general_getDec(top));
//	application_uart_transmit(" bottom: ");
//	application_uart_transmit(stm32dev_general_getDec(bottom));
//	application_uart_transmit("\r\n");

	if((bottom - (bottom/10)*10) == 0){
		shiftRight_bottom = true;
	}

	if(shiftRight_bottom){
		application_shiftreg_decPoint = 3;
		memcpy(application_shiftreg_asciiptr+3, stm32dev_general_getDec(bottom), 1);

		// top part
		uint8_t i = 0;
		char* top_ptr = stm32dev_general_getDec(top);
		for(i=0; i<16; i++){
			if(top_ptr[i] == 0x00) break;
		}
		memcpy(application_shiftreg_asciiptr+3-i, top_ptr, i);
	} else {
		application_shiftreg_decPoint = 2;
		memcpy(application_shiftreg_asciiptr+2, stm32dev_general_getDec(bottom), 2);

		// top part
		uint8_t i = 0;
		char* top_ptr = stm32dev_general_getDec(top);
		for(i=0; i<16; i++){
			if(top_ptr[i] == 0x00) break;
		}
		memcpy(application_shiftreg_asciiptr+2-i, top_ptr, i);
	}

}

void application_shiftreg_setHex(uint8_t Nr){
	application_shiftreg_hc595Values[2] |= (0xFC);
	application_shiftreg_hc595Values[1] |= (0x0C);

	application_shiftreg_addHex(Nr);
}

void application_shiftreg_addHex(uint8_t Nr){

	if(Nr & 0x01){
		application_shiftreg_hc595Values[2] &= ~(0x04);
	}
	if(Nr & 0x02){
		application_shiftreg_hc595Values[2] &= ~(0x08);
	}
	if(Nr & 0x04){
		application_shiftreg_hc595Values[2] &= ~(0x10);
	}
	if(Nr & 0x08){
		application_shiftreg_hc595Values[2] &= ~(0x20);
	}
	if(Nr & 0x10){
		application_shiftreg_hc595Values[2] &= ~(0x40);
	}
	if(Nr & 0x20){
		application_shiftreg_hc595Values[2] &= ~(0x80);
	}
	if(Nr & 0x40){
		application_shiftreg_hc595Values[1] &= ~(0x04);
	}
	if(Nr & 0x80){
		// DP
		application_shiftreg_hc595Values[1] &= ~(0x08);
	}
}

void application_shiftreg_setChar(char val){
	switch(val){
		case ' ':
			application_shiftreg_setHex(0x00);
			break;
		case '.':
			application_shiftreg_setHex(0x80);
			break;
		case '0':
			application_shiftreg_setHex(0x3F);
			break;
		case '1':
			application_shiftreg_setHex(0x06);
			break;
		case '2':
			application_shiftreg_setHex(0x5B);
			break;
		case '3':
			application_shiftreg_setHex(0x4F);
			break;
		case '4':
			application_shiftreg_setHex(0x66);
			break;
		case '5':
			application_shiftreg_setHex(0x6D);
			break;
		case '6':
			application_shiftreg_setHex(0x7D);
			break;
		case '7':
			application_shiftreg_setHex(0x07);
			break;
		case '8':
			application_shiftreg_setHex(0x7F);
			break;
		case '9':
			application_shiftreg_setHex(0x67);
			break;
		case 'A':
			application_shiftreg_setHex(0x77);
			break;
		case 'b':
			application_shiftreg_setHex(0x7C);
			break;
		case 'c':
			application_shiftreg_setHex(0x58);
			break;
		case 'C':
			application_shiftreg_setHex(0x39);
			break;
		case 'd':
			application_shiftreg_setHex(0x5E);
			break;
		case 'E':
			application_shiftreg_setHex(0x79);
			break;
		case 'F':
			application_shiftreg_setHex(0x71);
			break;
		case 'H':
			application_shiftreg_setHex(0x76);
			break;
		case 'i':
			application_shiftreg_setHex(0x04);
			break;
		case 'J':
			application_shiftreg_setHex(0x1E);
			break;
		case 'L':
			application_shiftreg_setHex(0x38);
			break;
		case 'n':
			application_shiftreg_setHex(0x54);
			break;
		case 'o':
			application_shiftreg_setHex(0x5C);
			break;
		case 'P':
			application_shiftreg_setHex(0x73);
			break;
		case 'r':
			application_shiftreg_setHex(0x50);
			break;
		case 'S':
			application_shiftreg_setHex(0x6D);
			break;
		case 'u':
			application_shiftreg_setHex(0x1C);
			break;
		case 'U':
			application_shiftreg_setHex(0x3E);
			break;
		case '-':
			application_shiftreg_setHex(0x30);
			break;
	}
}

void application_shiftreg_setSegment(uint8_t segNr){
	application_shiftreg_hc595Values[1] &= ~(0x03);
	application_shiftreg_hc595Values[2] &= ~(0x03);
	if((segNr >= 1) && (segNr <=4)){
		if(segNr > 2){
			if(segNr == 3){
				application_shiftreg_hc595Values[1] |= (0x01);
			} else {
				application_shiftreg_hc595Values[1] |= (0x02);
			}
		} else{
			if(segNr == 1){
				application_shiftreg_hc595Values[2] |= (0x01);
			} else {
				application_shiftreg_hc595Values[2] |= (0x02);
			}
		}
	}
}

void application_shiftreg_setLed(uint8_t nr, bool value){
	if((nr >= 1) && (nr <=4)){
		if(value){
			application_shiftreg_hc595Values[1] |= (1<<(7-(nr-1)));
		} else {
			application_shiftreg_hc595Values[1] &= ~(1<<(7-(nr-1)));
		}
	}

	application_shiftreg_update();
}

void application_shiftreg_setNulout(bool newVal){
	application_shiftreg_nulOut = newVal;
}
