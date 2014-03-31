/*
 * windVane.h
 *
 *  Created on: Mar 30, 2014
 *      Author: Administrator
 */

#ifndef WINDVANE_H_
#define WINDVANE_H_

#include "ABTypes.h"
#include "adc.h"

#define WIND_VANE_ADC_0 ADC_0
#define WIND_VANE_ADC_1 ADC_1

#define WIND_VANE_ADC_PIN00_PE3 ADC_PIN_IN00_PE3
#define WIND_VANE_ADC_PIN01_PE2 ADC_PIN_IN01_PE2
#define WIND_VANE_ADC_PIN02_PE1 ADC_PIN_IN02_PE1
#define WIND_VANE_ADC_PIN03_PE0 ADC_PIN_IN03_PE0
#define WIND_VANE_ADC_PIN04_PD3 ADC_PIN_IN04_PD3
#define WIND_VANE_ADC_PIN05_PD2 ADC_PIN_IN05_PD2
#define WIND_VANE_ADC_PIN06_PD1 ADC_PIN_IN06_PD1
#define WIND_VANE_ADC_PIN07_PD0 ADC_PIN_IN07_PD0
#define WIND_VANE_ADC_PIN08_PE5 ADC_PIN_IN08_PE5
#define WIND_VANE_ADC_PIN09_PE4 ADC_PIN_IN09_PE4
#define WIND_VANE_ADC_PIN10_PB4 ADC_PIN_IN10_PB4
#define WIND_VANE_ADC_PIN11_PB5 ADC_PIN_IN11_PB5

#define WIND_VANE_ADC_MUX_0 ADC_MUX_0
#define WIND_VANE_ADC_MUX_1 ADC_MUX_1
#define WIND_VANE_ADC_MUX_2 ADC_MUX_2
#define WIND_VANE_ADC_MUX_3 ADC_MUX_3
#define WIND_VANE_ADC_MUX_4 ADC_MUX_4
#define WIND_VANE_ADC_MUX_5 ADC_MUX_5
#define WIND_VANE_ADC_MUX_6 ADC_MUX_6
#define WIND_VANE_ADC_MUX_7 ADC_MUX_7

#define WIND_VANE_BUFFER_SIZE 1000

typedef struct {
	int sum ;
	int buffer[WIND_VANE_BUFFER_SIZE];
	int count;
} WVIntBuffer;

uint32_t windVaneADC;
uint32_t windVaneADCMux;
uint32_t windVaneADCPin;

///////////////////////////////////////////////
// API Layer 0
void windVaneInit(uint32_t wind_vane_adc,uint32_t wind_vane_adc_mux, uint32_t wind_vane_adc_pin);
void windVaneRefresh(int refreshSize);
float windVaneGetADCRawValue();
float windVaneGetAngle();


#endif /* WINDVANE_H_ */
