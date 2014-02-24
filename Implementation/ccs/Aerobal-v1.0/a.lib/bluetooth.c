/*
 * bluetooth.c
 *
 *  Created on: Feb 23, 2014
 *      Author: Administrator
 */

#include "bluetooth.h"

void bluetoothUnlockPD7() {
	//#define GPIO_PORTD_BASE 0x40007000 // GPIO Port D
	//#define GPIO_O_LOCK 0x00000520 // GPIO Lock

	//After GPIOLOCK is modified, we are then able to set bits 0:7
	//of hardware register GPIOCR to make the configuration registers
	// modifiable.

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

/*
	//Setting System Clock to 16Mhz.
	//TODO: Test to see if other clocks work.
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                       SYSCTL_XTAL_16MHZ);

	//Enabling PF2 LED Peripheral and Pin.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	//Unlocking Pin PD7.
	bluetoothUnlockPD7();

	//Enabling Peripherals.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART2);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	//Enabling Interrupts.
	IntMasterEnable();

	//UART Configuration.
	GPIOPinConfigure(GPIO_PD6_U2RX);
	GPIOPinConfigure(GPIO_PD7_U2TX);
	ROM_GPIOPinTypeUART(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);

	//UART Clock Configuration
	ROM_UARTConfigSetExpClk(UART2_BASE, ROM_SysCtlClockGet(), 9600,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	//Enable UART interrupts.
	ROM_IntEnable(INT_UART2);
	ROM_UARTIntEnable(UART2_BASE, UART_INT_RX | UART_INT_RT);
*/
/**/
	//Setting System Clock to 16Mhz.
	//TODO: Test to see if other clocks work.
	ROM_SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN |
	                       SYSCTL_XTAL_16MHZ);

	//Enabling PF2 LED Peripheral and Pin.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);

	//Enabling Peripherals.
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

	//Enabling Interrupts.
	IntMasterEnable();

	//UART Configuration.
	GPIOPinConfigure(GPIO_PA0_U0RX);
	GPIOPinConfigure(GPIO_PA1_U0TX);
	ROM_GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	//UART Clock Configuration
	ROM_UARTConfigSetExpClk(UART0_BASE, ROM_SysCtlClockGet(), 115200,
			(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));

	//Enable UART interrupts.
	ROM_IntEnable(INT_UART0);
	ROM_UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);
/**/
	//To track number of events for testing,
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


	//Interrupt status variable.
    uint32_t ui32Status;
/*
    // Get the interrupt status and clear them.
    // TODO: It appears to return more than one status. Verify.
    ui32Status = ROM_UARTIntStatus(UART2_BASE, true);
    ROM_UARTIntClear(UART2_BASE, ui32Status);

    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART2_BASE)){

        // Read the next character from the UART and write it back to the UART.
        ROM_UARTCharPutNonBlocking(UART2_BASE, ROM_UARTCharGetNonBlocking(UART2_BASE));

        // Blink the LED for 1 millisecond.
        // Each SysCtlDelay is about 3 clocks.
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
*/
/**/
    // Get the interrupt status and clear them.
    // TODO: It appears to return more than one status. Verify.
    ui32Status = ROM_UARTIntStatus(UART0_BASE, true);
    ROM_UARTIntClear(UART0_BASE, ui32Status);

    // Loop while there are characters in the receive FIFO.
    while(ROM_UARTCharsAvail(UART0_BASE)){

        // Read the next character from the UART and write it back to the UART.
        ROM_UARTCharPutNonBlocking(UART0_BASE, ROM_UARTCharGetNonBlocking(UART0_BASE));

        // Blink the LED for 1 millisecond.
        // Each SysCtlDelay is about 3 clocks.
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        SysCtlDelay(SysCtlClockGet() / (1000 * 3));
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);

    }
/**/
    bluetoothEventCount++;
}

