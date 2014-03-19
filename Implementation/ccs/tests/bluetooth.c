/*
 * bluetooth.c
 *
 *  Created on: Feb 23, 2014
 *      Author: Administrator
 */

#include "bluetooth.h"

/*
 * Initializes the bluetooth UART module.
 * Test mode hardcoded as of v0.1.
 *
 * Ports:
 * PA0 - UART0 RX
 * PA1 - UART0 TX
 * PF2 - GPIO Output LED.
 *
 */
void bluetoothInit(){

	uartMasterEnable(UART3,UART_BAUD_9600);

	//To track number of events for testing.
	bluetoothEventCount = 0;

}


/*****************************
 * Interrupt Handler
 * | extern void bluetoothInterruptHandler(void);
 *****************************
 *
 * Bluetooth Interrupt Handler that empties the FIFO by
 * writing back the same characters to UART.
 *
 */
void bluetoothInterruptHandler(void){
	uartInterruptClear(UART3);
	while(uartHasAvailable(UART3)){

		uartWriteChar(UART3,uartGetBufferChar(UART3));

        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);
    }
    bluetoothEventCount++;
}


