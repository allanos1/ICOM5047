/*
 * relay.h
 *
 *  Created on: Feb 22, 2014
 *      Author: Anthony
 *
 * Module created for the Relay of the Aerobal System.
 * The module is programmed for use with the A2 and A3 pin.
 * Uses custom GPIO library.
 *
 * The motorAtv library uses this module to change the state
 * of the VFD. A3 is programmed to be the START ('F') pin and
 * A2 is programmed to be the STOP ('R').
 */

#ifndef RELAY_H_
#define RELAY_H_

#include "gpio.h"
#include "ABTime.h"

#define AB_MODULE_RELAY_PORT GPIO_PORTA
#define AB_MODULE_RELAY_PINS 0x0C //PIN A2 A3
#define AB_MODULE_RELAY_PIN_STOP 0x04
#define AB_MODULE_RELAY_PIN_START 0x08

void relayInit();
void relayOn();
void relayOff();

#endif /* RELAY_H_ */
