/*
 * application_usb.h
 *
 *  Created on: 11-mrt.-2016
 *      Author: Robbe
 */

#ifndef INCLUDE_APPLICATION_USB_H_
#define INCLUDE_APPLICATION_USB_H_

#include "main.h"

// USB

/* Uncomment the line below to select your USB clock source */
//#define USE_USB_CLKSOURCE_CRSHSI48   1
#define USE_USB_CLKSOURCE_PLL        1

#if !defined (USE_USB_CLKSOURCE_PLL) && !defined (USE_USB_CLKSOURCE_CRSHSI48)
 #error "Missing USB clock definition"
#endif

//extern USBD_HandleTypeDef  USBD_Device;

void general_usb_init(void);

#endif /* INCLUDE_APPLICATION_USB_H_ */
