#include "a.lib/ABTest.h"
#include "a.lib/lcdSerial.h"
#include "a.lib/anemometer.h"
#include "a.lib/adc.h"
#include "a.lib/windVane.h"
#include "a.lib/bmp085Array.h"

void ABTestLCDInit(){
	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
}

void ABTestAnemometer(){
	ABTestLCDInit();
	anemometerInit(ANEMOMETER_PORTD,ANEMOMETER_PIN0);
	anemometerStart();
	ABTime now;
	int count = 0;
	int maxCount = 0;
	int timeCount = 0;
	int maxTimeCount = 0;
	while(1){
		now = anemometerSpeedBufferRefresh();
		lcdSerialCursorLine1();
		count = anemometerGetCount();
		if(count > maxCount){
			maxCount = count;
		}
		lcdSerialWriteString("Max: ");
		lcdSerialWriteNumber(maxCount);
		lcdSerialWriteString(" Cnt: ");
		lcdSerialWriteNumber(count);
		lcdSerialCursorLine2();
		timeCount = now.milliseconds;
		if(timeCount > maxTimeCount){
			maxTimeCount = timeCount;
		}
		lcdSerialWriteString("Max: ");
		lcdSerialWriteNumber(maxTimeCount);
		lcdSerialWriteString(" Time: ");
		lcdSerialWriteNumber(timeCount);
		lcdSerialCursorLine3();
		lcdSerialWriteString("B-Speed KM: ");
		lcdSerialWriteNumberWithBounds(anemometerSpeedConvertKmH(anemometerSpeedBufferGetAverage()),0,5);
		lcdSerialCursorLine4();
		lcdSerialWriteString("B-Speed MI: ");
		lcdSerialWriteNumber(anemometerSpeedConvertMiH(anemometerSpeedBufferGetAverage()));
	}
}

void ABTestADC(){
	ABTestLCDInit();
	adcInit(ADC_0);
	adcMuxPinSet(ADC_0,ADC_MUX_3,ADC_PIN_IN00_PE3);
	int i = 0;
	while(1){
		i++;
		adcRefresh();
		int val = adcDataGet(ADC_PIN_IN00_PE3);
		lcdSerialCursorLine1();
		lcdSerialWriteString("ADC Value: ");
		lcdSerialWriteNumber(val);
		lcdSerialWriteString("  ");
		lcdSerialCursorLine2();
		lcdSerialWriteString("Counter: ");
		lcdSerialWriteNumber(i);
		lcdSerialWriteString("  ");
	}
}

void ABTestWindVane(){
	ABTestLCDInit();
	float val;
	float max;
	float min;
	windVaneInit(WIND_VANE_ADC_0,WIND_VANE_ADC_MUX_0,WIND_VANE_ADC_PIN00_PE3);
	windVaneRefresh(WIND_VANE_BUFFER_SIZE);
	min = windVaneGetAngle();
	int count = 0;
	while(1){
		windVaneRefresh(WIND_VANE_BUFFER_SIZE);
		val = windVaneGetADCRawValue();
		if(!count){
			max = 0;
			min = windVaneGetADCRawValue();
		}
		count = (count+1) % 20;
		if(val > max){
			max = val;
		}
		if(val < min){
			min = val;
		}
		lcdSerialCursorLine1();
		lcdSerialWriteString("WV ADC: ");
		lcdSerialWriteNumber(val);
		lcdSerialWriteString("   ");
		lcdSerialCursorLine2();
		lcdSerialWriteString("H:");
		lcdSerialWriteNumberWithBounds(max,4,1);
		lcdSerialWriteString(" L:");
		lcdSerialWriteNumberWithBounds(min,4,1);
		lcdSerialCursorLine3();
		lcdSerialWriteString("V:");
		lcdSerialWriteNumberWithBounds(adcGetVoltage(windVaneGetADCRawValue()),0,3);
		lcdSerialWriteString(" A:");
		lcdSerialWriteNumberWithBounds(windVaneGetAngle(),0,2);
		lcdSerialCursorLine4();
		lcdSerialWriteString("Refresh: [");
		if(count >= 4) lcdSerialWriteString("=");
		else lcdSerialWriteString(" ");
		if(count >= 8) lcdSerialWriteString("=");
		else lcdSerialWriteString(" ");
		if(count >= 12) lcdSerialWriteString("=");
		else lcdSerialWriteString(" ");
		if(count >= 16) lcdSerialWriteString("=");
		else lcdSerialWriteString(" ");
		lcdSerialWriteString("] ");
		lcdSerialWriteNumber(count);
		lcdSerialWriteString(" ");
	}
}

void ABTestPressureSensorArray(){
	bmp085ArrayInit();
}

int main(int argc, const char * argv[]){

	ABTestPressureSensorArray();
	while(1);
}


