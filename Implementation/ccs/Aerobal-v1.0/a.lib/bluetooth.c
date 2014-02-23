/*
 * bluetooth.c
 *
 *  Created on: Feb 23, 2014
 *      Author: Administrator
 */

#include "bluetooth.h"

/*
 * Unlocks the Port D7 as it is locked on processor
 * power on.
 */
void unlockPD7() {

	//After GPIOLOCK is modified, we are then able to set bits 0:7
	//of hardware register GPIOCR to make the configuration registers
	//modifiable.
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;

	//Set the commit register for PD7 to allow changing the function
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0x80;

	//Enable the alternate function for PD7 (U2TX)
	HWREG(GPIO_PORTD_BASE + GPIO_O_AFSEL) |= 0x80;

	// Turn on the digital enable for PD7
	HWREG(GPIO_PORTD_BASE + GPIO_O_DEN) |= 0x80;

	// Relock the commit register, to prevent further changes
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;
}

/*
 *
 */
void bluetoothInit(){

	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	unlockPD7();
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	IntMasterEnable();
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	UARTFIFODisable(UART2_BASE);
	UARTDisable(UART2_BASE);
	IntDisable(INT_UART2);
	UARTIntDisable(UART2_BASE, UART_INT_RX | UART_INT_RT);
	UARTConfigSetExpClk(UART2_BASE, SysCtlClockGet(), 9600,
		(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
	IntEnable(INT_UART2);
	UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
	UARTFIFOEnable(UART2_BASE);
}


