/*
 * ABPortsInterruptHandler.c
 *
 *  Created on: Apr 5, 2014
 *      Author: JJLB
 */

#include "ABPortsInterruptHandler.h"

void InterruptHandlerPortA(){



}

void InterruptHandlerPortB(){



}

void InterruptHandlerPortC(){



}

void InterruptHandlerPortD(){

	//Not handling the clear of the caller interrupt because of backward compatibility.

	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_0){
		anemometerInterruptHandler_Counter();
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_1){
		//function to call
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_2){
		//function to call
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_3){
		bmp085ArraySynchronize();
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_4){
		//function to call
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_5){
		//function to call
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_6){
		//function to call
	}
	if(HWREG(GPIO_PORTD | GPIO_OFFSET_INTERRUPT_RAW_STATUS) & GPIO_PIN_6){
		//function to call
	}
	gpioSetInterruptClear(GPIO_PORTD,0xFF,0xFF);
}

void InterruptHandlerPortE(){



}

void InterruptHandlerPortF(){



}
