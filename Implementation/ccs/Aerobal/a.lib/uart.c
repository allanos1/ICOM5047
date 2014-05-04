/*
 * uart.c
 *
 *  Created on: Mar 10, 2014
 *      Author: Anthony
 */


#include "uart.h"

//////////////////////////////////
// API Layer 0

/*
 * Gets the respective mapping of the UARTPeripheral
 * modules in SysCtl.
 */
uint32_t uartGetPeripheral(uint32_t uart){
	switch(uart){
		case UART0: return SYSCTL_PERIPH_UART0;
		case UART1: return SYSCTL_PERIPH_UART1;
		case UART2: return SYSCTL_PERIPH_UART2;
		case UART3: return SYSCTL_PERIPH_UART3;
		case UART4: return SYSCTL_PERIPH_UART4;
		case UART5: return SYSCTL_PERIPH_UART5;
		case UART6: return SYSCTL_PERIPH_UART6;
		case UART7: return SYSCTL_PERIPH_UART7;
		default: return 0;
	}
}

/*
 * Gets the respective mapping for the PinConfigure
 * functionality of the receiver pins.
 */
uint32_t uartGetPinConfigureRX(uint32_t uart){
	switch(uart){
		case UART0: return GPIO_PA0_U0RX;
		case UART1: return GPIO_PC4_U1RX;
		case UART2: return GPIO_PD6_U2RX;
		case UART3: return GPIO_PC6_U3RX;
		case UART4: return GPIO_PC4_U4RX;
		case UART5: return GPIO_PE4_U5RX;
		case UART6: return GPIO_PD4_U6RX;
		case UART7: return GPIO_PE0_U7RX;
		default: return 0;
	}
}

/*
 * Gets the respective mapping for the PinConfigure
 * functionality of the transmitter pins.
 */
uint32_t uartGetPinConfigureTX(uint32_t uart){
	switch(uart){
		case UART0: return GPIO_PA1_U0TX;
		case UART1: return GPIO_PC5_U1TX;
		case UART2: return GPIO_PD7_U2TX;
		case UART3: return GPIO_PC7_U3TX;
		case UART4: return GPIO_PC5_U4TX;
		case UART5: return GPIO_PE5_U5TX;
		case UART6: return GPIO_PD5_U6TX;
		case UART7: return GPIO_PE1_U7TX;
		default: return 0;
	}
}

/*
 * Returns the base of the UART specified.
 *
 */
uint32_t uartGetPinTypeGPIOBase(uint32_t uart){
	switch(uart){
		case UART0: return GPIO_PORTA;
		case UART1: return GPIO_PORTC;
		case UART2: return GPIO_PORTD;
		case UART3: return GPIO_PORTC;
		case UART4: return GPIO_PORTC;
		case UART5: return GPIO_PORTE;
		case UART6: return GPIO_PORTD;
		case UART7: return GPIO_PORTE;
		default: return 0;
	}
}

/*
 * Returns the GPIO Pins assigned on pin type config.
 */
uint32_t uartGetPinTypeGPIOPins(uint32_t uart){
	switch(uart){
		case UART0: return GPIO_PIN_0 | GPIO_PIN_1;
		case UART1: return GPIO_PIN_4 | GPIO_PIN_5;
		case UART2: return GPIO_PIN_6 | GPIO_PIN_7;
		case UART3: return GPIO_PIN_6 | GPIO_PIN_7;
		case UART4: return GPIO_PIN_4 | GPIO_PIN_5;
		case UART5: return GPIO_PIN_4 | GPIO_PIN_5;
		case UART6: return GPIO_PIN_4 | GPIO_PIN_5;
		case UART7: return GPIO_PIN_0 | GPIO_PIN_1;
		default: return 0;
	}
}

/*
 *Returns the base of the UART.
 */
uint32_t uartGetBase(uint32_t uart){
	switch(uart){
		case UART0: return UART0_BASE;
		case UART1: return UART1_BASE;
		case UART2: return UART2_BASE;
		case UART3: return UART3_BASE;
		case UART4: return UART4_BASE;
		case UART5: return UART5_BASE;
		case UART6: return UART6_BASE;
		case UART7: return UART7_BASE;
		default: return 0;
	}
}

/*
 * Returns the UART interrupt global
 * identifier.
 */
uint32_t uartGetUartInterrupt(uint32_t uart){
	switch(uart){
		case UART0: return INT_UART0;
		case UART1: return INT_UART1;
		case UART2: return INT_UART2;
		case UART3: return INT_UART3;
		case UART4: return INT_UART4;
		case UART5: return INT_UART5;
		case UART6: return INT_UART6;
		case UART7: return INT_UART7;
		default: return 0;
	}
}

//////////////////////////////////////
// API Layer 1

/*
 * Configures a uart module with the specified baud rate
 * and a default 8 bit data length, 1 stop bit and no parity.
 */
void uartMasterEnable(uint32_t uart, uint32_t baud){

	//TODO: Check clock dependency.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC
			| SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Enable both uart peripherals: UART and GPIO.
	SysCtlPeripheralEnable(uartGetPeripheral(uart));
	gpioSetMasterEnable(uartGetPinTypeGPIOBase(uart));

	//Enable Processor INterrupts.
	gpioHelperInterruptMasterEnable();

	//UART Config.
	GPIOPinConfigure(uartGetPinConfigureRX(uart));
	GPIOPinConfigure(uartGetPinConfigureTX(uart));
	GPIOPinTypeUART(uartGetPinTypeGPIOBase(uart),uartGetPinTypeGPIOPins(uart));

	//UART Clock Configuration
	UARTConfigSetExpClk(uartGetBase(uart), SysCtlClockGet(), baud,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	//Enable UART Interrupts.
	IntEnable(uartGetUartInterrupt(uart));
	UARTIntEnable(uartGetBase(uart), UART_INT_RX | UART_INT_RT);
}


void uartIntEnable(uint32_t uart){
	UARTIntEnable(uartGetBase(uart),UART_INT_RX | UART_INT_RT);
}

void uartIntDisable(uint32_t uart){
	UARTIntDisable(uartGetBase(uart),UART_INT_RX | UART_INT_RT);
}
/*
 *Clears the interrupts for the UARTS.
 */
void uartInterruptClear(uint32_t uart){
	//Interrupt status variable.
    uint32_t ui32Status;

	// Get the interrupt status and clear them.
	// TODO: It appears to return more than one status. Verify.
    ui32Status = UARTIntStatus(uartGetBase(uart), true);
	UARTIntClear(uartGetBase(uart), ui32Status);
}

/*
 * Checks if the uart has characters available.
 */
bool uartHasAvailable(uint32_t uart){
	return UARTCharsAvail(uartGetBase(uart));
}
/*
 * Writes a character to the buffer and awaits the write
 * completion.
 */
void uartWriteCharSync(uint32_t uart, unsigned char data){
	UARTCharPut(uartGetBase(uart),data);
}

/*
 * Writes a character to the buffer and does not await
 * write completion.
 */
void uartWriteChar(uint32_t uart, unsigned char data){
	UARTCharPutNonBlocking(uartGetBase(uart),data);
}

/*
 * Gets a character from the buffer without halting
 * execution.
 */
int32_t uartGetBufferChar(uint32_t uart){
	return UARTCharGetNonBlocking(uartGetBase(uart));
}

/*
 * Gets a character from the buffer halting execution.
 */
int32_t uartGetBufferCharSync(uint32_t uart){
	return UARTCharGet(uartGetBase(uart));
}


////////////////////////////////////////////
// API Layer 2
void uartMasterEnableNoInterrupt(uint32_t uart, uint32_t baud){

	//TODO: Check clock dependency.
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC
			| SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);

	//Enable both uart peripherals: UART and GPIO.
	SysCtlPeripheralEnable(uartGetPeripheral(uart));
	gpioSetMasterEnable(uartGetPinTypeGPIOBase(uart));

	//Enable Processor INterrupts.
	gpioHelperInterruptMasterEnable();

	//UART Config.
	GPIOPinConfigure(uartGetPinConfigureRX(uart));
	GPIOPinConfigure(uartGetPinConfigureTX(uart));
	GPIOPinTypeUART(uartGetPinTypeGPIOBase(uart),uartGetPinTypeGPIOPins(uart));

	//UART Clock Configuration
	UARTConfigSetExpClk(uartGetBase(uart), SysCtlClockGet(), baud,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
