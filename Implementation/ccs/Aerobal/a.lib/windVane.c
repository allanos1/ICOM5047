/*
 * windVane.c
 *
 *  Created on: Mar 30, 2014
 *      Author: Anthony
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
	adcSetSequencerSize(wind_vane_adc,1);
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
	if(refreshSize <= 0 || refreshSize > WIND_VANE_BUFFER_SIZE){
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
 * Need Piecewise Characterization Documentation.
 *
 */
float windVaneGetAngle(){
	float x = windVaneGetADCRawValue();

	if(x <= 356) return 0.1271*x - 0.2542;
	else if(x <= 753) return 0.1134*x + 4.6474;
	else if(x <= 1133) return 0.1184*x + 0.8289;
	else if(x <= 1514) return 0.1181*x + 1.1811;
	else if(x <= 1908) return 0.1142*x + 7.0812;
	else if(x <= 2315) return 0.1106*x + 14.042;
	else if(x <= 2705) return 0.1154*x + 2.8846;
	else return 0.1136*x + 7.6136;
}
