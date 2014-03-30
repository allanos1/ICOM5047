/*
 * adc.h
 *
 * The Analog to Digital Converter Library
 *
 *  Created on: Mar 30, 2014
 *      Author: Juan, Anthony
 */

#ifndef ADC_H_
#define ADC_H_

#include <stdbool.h>
#include <stdint.h>
#include "gpio.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"


#define ADC_0 0x00
#define ADC_1 0x01

#define ADC_PIN_IN00_PE3 0x00
#define ADC_PIN_IN01_PE2 0x01
#define ADC_PIN_IN02_PE1 0x02
#define ADC_PIN_IN03_PE0 0x03
#define ADC_PIN_IN04_PD3 0x04
#define ADC_PIN_IN05_PD2 0x05
#define ADC_PIN_IN06_PD1 0x06
#define ADC_PIN_IN07_PD0 0x07
#define ADC_PIN_IN08_PE5 0x08
#define ADC_PIN_IN09_PE4 0x09
#define ADC_PIN_IN10_PB4 0x0A
#define ADC_PIN_IN11_PB5 0x0B

#define ADC_SEQUENCER_0_8SAMPLES 0
#define ADC_SEQUENCER_1_4SAMPLES 1
#define ADC_SEQUENCER_2_4SAMPLES 2
#define ADC_SEQUENCER_3_1SAMPLES 3

#define ADC_SEQUENCE_TRIGGER_MANUAL ADC_TRIGGER_PROCESSOR

#define ADC_PRIORITY_HIGHEST 0
#define ADC_PRIORITY_HIGH 1
#define ADC_PRIORITY_MED 2
#define ADC_PRIORITY_LOW 3

#define ADC_MUX_0_SHIFT 0x00
#define ADC_MUX_1_SHIFT 0x04
#define ADC_MUX_2_SHIFT 0x08
#define ADC_MUX_3_SHIFT 0x0C
#define ADC_MUX_4_SHIFT 0x10
#define ADC_MUX_5_SHIFT 0x14
#define ADC_MUX_6_SHIFT 0x18
#define ADC_MUX_7_SHIFT 0x1C

#define ADC_MUX_0 0
#define ADC_MUX_1 1
#define ADC_MUX_2 2
#define ADC_MUX_3 3
#define ADC_MUX_4 4
#define ADC_MUX_5 5
#define ADC_MUX_6 6
#define ADC_MUX_7 7

uint32_t adcConfigured ;
uint32_t adcDataStruct[8] ;
uint32_t adcMuxConfig[8];
//////////////////////////////////
// API Layer 0
uint32_t adcGetBase(uint32_t adc);
uint32_t adcPinGetGPIOPort(uint32_t adc_pin_in);
uint32_t adcPinGetGPIOPin(uint32_t adc_pin_in);
uint32_t adcGetMuxShift(uint32_t mux);
void adcEnable(uint32_t adc);
void adcSequenceConfigure(uint32_t adc);
void adcEnableADCPin(uint32_t adc_pin_in);
void adcSequenceEnable(uint32_t adc);
void adcInterruptClear(uint32_t adc);
void adcTrigger(uint32_t adc);
void adcTriggerWait(uint32_t adc);
void adcMuxPinSet(uint32_t adc, uint32_t adc_mux, uint32_t adc_pin_in);
void adcInterruptEnable(uint32_t adc);
void adcFIFOEnable(uint32_t adc);
void adcClockADCEnable();
void adcClockNormalEnable();
////////////////////////////////////
// API Layer 1
void adcInit(uint32_t adc);
void adcMuxPinSet(uint32_t adc, uint32_t adc_mux, uint32_t adc_pin_in);
void adcRefresh();
uint32_t adcDataGet(uint32_t adc_pin_in);

#endif /* ADC_H_ */
