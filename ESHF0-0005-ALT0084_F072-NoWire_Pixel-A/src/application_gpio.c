#include "application_gpio.h"
#include "stm32dev_boards.h"
#include "application_ws2812b.h"

void application_gpio_init(void){
	  // GPIO INIT

	  GPIO_InitTypeDef  GPIO_InitStruct;

	  // LED
		  __HAL_RCC_GPIOC_CLK_ENABLE();
		  GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		  GPIO_InitStruct.Pull  = GPIO_PULLUP;
		  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

		  GPIO_InitStruct.Pin = GPIO_PIN_13;
		  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		  HAL_GPIO_WritePin(GPIOC,GPIO_PIN_13,GPIO_PIN_RESET);

	  // PUSHBUTTON
		  __HAL_RCC_GPIOB_CLK_ENABLE();
		  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
		  GPIO_InitStruct.Pull = GPIO_NOPULL;
		  GPIO_InitStruct.Pin = GPIO_PIN_2;
		  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		  /* Enable and set EXTI line 0 Interrupt to the lowest priority */
		  HAL_NVIC_SetPriority(EXTI2_3_IRQn, 1, 0);
		  HAL_NVIC_EnableIRQ(EXTI2_3_IRQn);
}

void application_gpio_exti_cb(void){
      HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_13);
      volatile static uint8_t tmp = 0;

      if(tmp == 0){
    	  application_ws2812b_setColorAll(0,0,0);
      } else if(tmp == 1){
    	  application_ws2812b_setColorAll(255,0,0);
      } else if(tmp == 2){
    	  application_ws2812b_setColorAll(0,255,0);
      } else if(tmp == 3){
    	  application_ws2812b_setColorAll(0,0,255);
      }

      if(++tmp == 4) tmp = 0;
}
