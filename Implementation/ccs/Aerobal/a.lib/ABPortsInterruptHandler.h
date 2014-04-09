/*
 * ABPortsInterruptHandler.h
 *
 *  Created on: Apr 5, 2014
 *      Author: JJLB
 */

#ifndef ABPORTSINTERRUPTHANDLER_H_
#define ABPORTSINTERRUPTHANDLER_H_

#include <stdint.h>
#include <stdbool.h>
#include "gpio.h"
#include "bmp085Array.h"
#include "anemometer.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

void InterruptHandlerPortA();
void InterruptHandlerPortB();
void InterruptHandlerPortC();
void InterruptHandlerPortD();
void InterruptHandlerPortE();
void InterruptHandlerPortF();

#endif /* ABPORTSINTERRUPTHANDLER_H_ */
