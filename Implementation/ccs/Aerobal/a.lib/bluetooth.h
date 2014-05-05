/*
 * bluetooth.h
 *
 *  Created on: Feb 23, 2014
 *      Author: Administrator
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
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
#include "uart.h"
#include "string.h"
#include "gpio.h"
#include "motorAtv.h"
#include "ABSensorServer.h"

#define BLUETOOTH_UART0 UART0 //RX: A0  TX: A1
#define BLUETOOTH_UART1 UART1 //RX: C4  TX: C5
#define BLUETOOTH_UART2 UART2 //RX: D6  TX: D7
#define BLUETOOTH_UART3 UART3 //RX: C6  TX: C7
#define BLUETOOTH_UART4 UART4 //RX: C4  TX: C5
#define BLUETOOTH_UART5 UART5 //RX: E4  TX: E5
#define BLUETOOTH_UART6 UART6 //RX: D4  TX: D5
#define BLUETOOTH_UART7 UART7 //RX: E0  TX: E1

#define BLUETOOTH_UART_BAUD_9600 UART_BAUD_9600

#define BLUETOOTH_STATE_1_B 0
#define BLUETOOTH_STATE_2_T 1
#define BLUETOOTH_STATE_3_C 2
#define BLUETOOTH_STATE_4_READ 3


uint32_t bluetoothUART;
int bluetoothEventCount;
uint8_t bluetoothReset;
char bluetoothBuffer[20];
int bluetoothState;
int bluetoothEnabled;
int bluetoothSettingFanStatus ;
int bluetoothSettingFanQuery;
float bluetoothSettingExpWindSpeed ;

///////////////////////////////////
// API Layer 0
void bluetoothInit(uint32_t uart, uint32_t uartBaud);
void bluetoothInterruptHandler();
int bluetoothStateMachine(char c);
void bluetoothEmptyUartBuffer();
int bluetoothStateMachine(char c);
void bluetoothSendAck();
void bluetoothSendErr();
void bluetoothSendTerminator();
void bluetoothSendString(char * s);
int bluetoothIsQuery(char *value);
void bluetoothGetQuery(char * value);
void bluetoothEvaluateBuffer(char *buffer);
int bluetoothGetNumber(char value);

////////////////////////////////////
// API Layer 1
int bluetoothGetSettingFanStatus();
void bluetoothSetSettingFanStatus(int value);
void bluetoothEnable();
void bluetoothDisable();
void bluetoothSetSettingFanQuery(int value);
int bluetoothGetSettingFanQuery();

#endif /* BLUETOOTH_H_ */
