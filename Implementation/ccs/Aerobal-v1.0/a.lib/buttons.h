/*
 * buttons.h
 *
 *  Created on: Feb 24, 2014
 *      Author: Administrator
 */

#ifndef BUTTONS_H_
#define BUTTONS_H_

#include "lcd.h"
#include "gpio.h"

#define BUTTONS_ENABLED_BUTTONS 0x3F
#define BUTTONS_STANDARD_DELAY 200000

#define BUTTONS_B0 0x01
#define BUTTONS_B1 0x02
#define BUTTONS_B2 0x04
#define BUTTONS_B3 0x08
#define BUTTONS_B4 0x10
#define BUTTONS_B5 0x20

int buttonsLastPressed ;

void buttonsInit();
void buttonsInterruptHandler();


#endif /* BUTTONS_H_ */
