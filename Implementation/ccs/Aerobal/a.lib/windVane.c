/*
 * windVane.c
 *
 *  Created on: Mar 30, 2014
 *      Author: Administrator
 */

#include "windVane.h"

WVIntBuffer windVaneBuffer;

/*
 * Initializes the wind vane to be interface with the given adc module,
 * adc multiplexer, and adc pin.
 */
void windVaneInit(uint32_t wind_vane_adc,uint32_t wind_vane_adc_mux, uint32_t wind_vane_adc_pin){
	windVaneADC = wind_vane_adc;
	windVaneADCMux = wind_vane_adc_mux;
	windVaneADCPin = wind_vane_adc_pin;
	adcInit(wind_vane_adc);
	adcMuxPinSet(wind_vane_adc, wind_vane_adc_mux, wind_vane_adc_pin);
	windVaneBuffer.count = 0;
	windVaneBuffer.sum = 0;
}

/*
 * Refreshes the wind vane module library buffer by the amount
 * of values given. If no value is given, then the default buffer
 * size number of values are taken.
 *
 * refreshSize - the amount of values to insert in the buffer.
 */
void windVaneRefresh(int refreshSize){
	adcRefresh();
	if(!refreshSize || refreshSize > WIND_VANE_BUFFER_SIZE){
		refreshSize = WIND_VANE_BUFFER_SIZE;
	}
	int i = 0;
	int value ;
	for(i = 0; i < refreshSize; i++){
		adcRefresh();
		value = adcDataGet(windVaneADCPin);
		windVaneBuffer.sum -= windVaneBuffer.buffer[windVaneBuffer.count];
		windVaneBuffer.sum += value;
		windVaneBuffer.buffer[windVaneBuffer.count] = value;
		windVaneBuffer.count = (windVaneBuffer.count + 1) % WIND_VANE_BUFFER_SIZE;
	}
}

/*
 * Returns the raw value of the current ADC average for the wind vane.
 */
float windVaneGetADCRawValue(){
	return (float) windVaneBuffer.sum / (float) WIND_VANE_BUFFER_SIZE;
}

/*
 * Returns the average angle value read from the wind vane
 * ADC module.
 *
 * Registered Max Value: 2956.0 -> Maps to  3.3V*2953/4096 = 2.3815429
 * Registered Min Value: 0.82 -> Maps to 3.3*0.82/4096 = 6.6mV
 *
 ******* Piecewise Characterization needed!
 */
float windVaneGetAngle(){
	return 0.1189*windVaneGetADCRawValue()+12.535;
}
