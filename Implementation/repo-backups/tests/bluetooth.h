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

int bluetoothEventCount;
char* bluetoothString;

void bluetoothInit();
void bluetoothInterruptHandler();



#endif /* BLUETOOTH_H_ */
