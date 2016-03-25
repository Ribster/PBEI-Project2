/*
 * application_usb.c
 *
 *  Created on: 11-mrt.-2016
 *      Author: Robbe
 */
#include "application_usb.h"
#include "application_general.h"

//USBD_HandleTypeDef  USBD_Device;

void general_usb_init(void){
	// init USB
	TIMx_USB_CLK_ENABLE();
}
