/*
 * buttons.c
 *
 * Buttons
 *
 *  Created on: Feb 24, 2014
 *      Author: Anthony Llanos Velazquez
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

	buttonsDisable();
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

/*
 * Returns whether the button B0 was pressed.
 */
uint32_t buttonsWasPressedB0(){
	return (buttonsLastPressed & BUTTONS_B0) == BUTTONS_B0;
}

/*
 * Returns whether the button B1 was pressed.
 */
uint32_t buttonsWasPressedB1(){
	return (buttonsLastPressed & BUTTONS_B1) == BUTTONS_B1;
}

/*
 * Returns whether the button B2 was pressed.
 */
uint32_t buttonsWasPressedB2(){
	return (buttonsLastPressed & BUTTONS_B2) == BUTTONS_B2;
}

/*
 * Returns whether the button B3 was pressed.
 */
uint32_t buttonsWasPressedB3(){
	return (buttonsLastPressed & BUTTONS_B3) == BUTTONS_B3;
}

/*
 * Returns whether the button B4 was pressed.
 */
uint32_t buttonsWasPressedB4(){
	return (buttonsLastPressed & BUTTONS_B4) == BUTTONS_B4;
}

/*
 * Returns whether the button B5 was pressed.
 */
uint32_t buttonsWasPressedB5(){
	return (buttonsLastPressed & BUTTONS_B5) == BUTTONS_B5;
}

/*
 * Returns whether a button was pressed. Must be reset
 * with buttonsEnable() to really be useful.
 */
int buttonsWasPressed(){
	return buttonsLastPressed != 0;
}

/*
 * Disables the interrupts on the buttons (all of them).
 */
void buttonsDisable(){
	gpioSetInterruptMaskDisable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
}


/*
 * Enables the buttons by eliminating the interrupt flags and the
 * last pressed flag, then disabling the mask of the buttons.
 */
void buttonsEnable(){
	buttonsLastPressed = 0;
	gpioSetInterruptClear(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,BUTTONS_ENABLED_BUTTONS);
	gpioSetInterruptMaskDisable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,BUTTONS_ENABLED_BUTTONS);
}

/*
 * Masks the interrupts of the buttons.
 */
void buttonsMask(){
	gpioSetInterruptMaskDisable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x20);
}

/*
 * Unmasks the interrupts of the buttons.
 */
void buttonsUnmask(){
	gpioSetInterruptMaskDisable(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,BUTTONS_ENABLED_BUTTONS);
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

	gpioSetPullUpSelect(GPIO_PORTB,0x3F,0x3F);

	gpioSetInterruptBothEdges(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
	gpioSetInterruptEvent(GPIO_PORTB,BUTTONS_ENABLED_BUTTONS,0x00);
	gpioHelperInterruptMasterEnable();
	gpioSetInterruptEnable(GPIO_PORTB);
	buttonsEnable();
}
