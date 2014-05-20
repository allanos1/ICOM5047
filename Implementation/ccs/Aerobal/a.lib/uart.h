/*
 * uart.h
 *
 *  Created on: Mar 10, 2014
 *      Author: Anthony
 */

#ifndef UART_H_
#define UART_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "gpio.h"

#define UART0 0x00 //RX: A0  TX: A1
#define UART1 0x01 //RX: C4  TX: C5
#define UART2 0x02 //RX: D6  TX: D7
#define UART3 0x03 //RX: C6  TX: C7
#define UART4 0x04 //RX: C4  TX: C5
#define UART5 0x05 //RX: E4  TX: E5
#define UART6 0x06 //RX: D4  TX: D5
#define UART7 0x07 //RX: E0  TX: E1

#define UART_BAUD_9600 9600

///////////////////////////////////
// API Layer 1
void uartMasterEnable(uint32_t uart, uint32_t baud);
void uartInterruptClear(uint32_t uart);
bool uartHasAvailable(uint32_t uart);
void uartWriteCharSync(uint32_t uart, unsigned char data);
void uartWriteChar(uint32_t uart, unsigned char data);
int32_t uartGetBufferCharSync(uint32_t uart);
int32_t uartGetBufferChar(uint32_t uart);
void uartIntEnable(uint32_t uart);
void uartIntDisable(uint32_t uart);
////////////////////////////////////////////
// API Layer 2
void uartMasterEnableNoInterrupt(uint32_t uart, uint32_t baud);

#endif /* UART_H_ */
