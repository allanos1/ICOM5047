/*
 * buttons.c
 *
 *  Created on: Feb 24, 2014
 *      Author: Administrator
 */


#include "buttons.h"

/*********************************
 * Interrupt handler.
 * | extern void buttonsInterruptHandler();
 *********************************
 *
 * Interrupt handler for the buttons. Detects the buttons,
 * determines which one was pushed and stores it in a variable.
 *
 */
void buttonsInterruptHandler(){

	//gpioSetInterruptMaskDisable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
	int button_RIS = gpioGetInterruptRawStatus(GPIO_PORTB, BUTTONS_ENABLED_BUTTONS);
	SysCtlDelay(BUTTONS_STANDARD_DELAY) ;

	if(button_RIS & BUTTONS_B0){
		buttonsLastPressed = BUTTONS_B0;
	}
	else if(button_RIS & BUTTONS_B1){
		buttonsLastPressed = BUTTONS_B1;
	}
	else if(button_RIS & BUTTONS_B2){
		buttonsLastPressed = BUTTONS_B2;
	}
	else if(button_RIS & BUTTONS_B3){
		buttonsLastPressed = BUTTONS_B3;
	}
	else if(button_RIS & BUTTONS_B4){
		buttonsLastPressed = BUTTONS_B4;
	}
	else if(button_RIS & BUTTONS_B5){
		buttonsLastPressed = BUTTONS_B5;
	}

	SysCtlDelay(BUTTONS_STANDARD_DELAY);
	gpioSetInterruptClear(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,BUTTONS_ENABLED_BUTTONS);

}

/* Initis the ports that use the buttons.
 *
 * Ports:
 * PB0 - PB5 : GPIO Inputs.
 *
 */
void buttonsInit(){
	gpioSetMasterEnable(GPIO_PORTB);
	gpioSetDirection(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
	gpioSetDigitalEnable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,BUTTONS_ENABLED_BUTTONS);

	IntMasterEnable();
	IntEnable(INT_GPIOB);
	gpioSetInterruptBothEdges(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
	gpioSetInterruptEvent(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
	gpioSetInterruptMaskDisable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,BUTTONS_ENABLED_BUTTONS);

}
