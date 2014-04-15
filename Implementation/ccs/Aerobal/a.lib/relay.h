/*
 * relay.h
 *
 *  Created on: Feb 22, 2014
 *      Author: Anthony
 *
 * Module created for the Relay of the Aerobal System.
 * The module is programmed for use with the A5 pin.
 * Uses custom GPIO library.
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "gpio.h"

#define AB_MODULE_RELAY_PORT GPIO_PORTA
#define AB_MODULE_RELAY_PINS 0x20 //PIN A5

void relayInit();
void relayToggle();
void relayOn();
void relayOff();

#endif /* RELAY_H_ */
