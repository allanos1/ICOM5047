#include "a.lib/ABTest.h"
#include "a.lib/lcdSerial.h"
#include "a.lib/anemometer.h"
#include "a.lib/adc.h"
#include "a.lib/windVane.h"
#include "a.lib/bmp085Array.h"
#include "a.lib/bluetooth.h"
#include "a.lib/gpio.h"
#include "a.lib/timers.h"
#include "a.lib/binCounter.h"
#include "a.lib/buttons.h"
#include "a.lib/ABUI.h"
#include "a.lib/string.h"
#include "a.lib/ABPortsInterruptHandler.h"

void ABTestLCDInit();
void ABTestBMPSpeed();
void ABTestAnemometer();
void ABTestADC();
void ABTestWindVane();

///////

tI2CMInstance i2cInstance;
tBMP180 bmpInstance;


void ABTestLCDInit(){
	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
}

void ABTestDAC(){

}

void ABTestBluetooth(){
	gpioSetMasterEnable(GPIO_PORTF);
	gpioSetDigitalEnable(GPIO_PORTF,0x0C,0x0C);
	gpioSetDirection(GPIO_PORTF,0x0C,0x0C);
	bluetoothInit(BLUETOOTH_UART4,BLUETOOTH_UART_BAUD_9600);
	while(1);
}


void ABTestBMPSpeed(){

	gpioSetMasterEnable(GPIO_PORTF);
	gpioSetDirection(GPIO_PORTF,0x0C,0x0C);
	gpioSetDigitalEnable(GPIO_PORTF,0x0C,0x0C);
	ABTestLCDInit();
	gpioSetData(GPIO_PORTF,0x0C,0x04);
	bmp085Init(AB_I2C_MODULE_1);
	gpioSetData(GPIO_PORTF,0x0C,0x08);
	bmp085Init(AB_I2C_MODULE_1);
	float max = 0;
	float min = 1200000.0;
	int togg = 0;
	while(1){
		SysCtlDelay(1000000);
		if(togg){
			togg = 0;
			gpioSetData(GPIO_PORTF,0x0C,0x04);
		}
		else{
			togg = 1;
			gpioSetData(GPIO_PORTF,0x0C,0x08);
		}
		bmp085DataRead(togg);
		float val = bmp085GetPressure();
		if(val > max){
			max = val;
		}
		if(val < min){
			min = val;
		}
		lcdSerialCursorLine1();
		lcdSerialWriteString("P: ");
		lcdSerialWriteNumber(val);
		lcdSerialCursorLine2();
		val = bmp085GetTemperature();
		lcdSerialWriteString("T: ");
		lcdSerialWriteNumber(val);
		lcdSerialCursorLine3();
		lcdSerialWriteString("MinP: ");
		lcdSerialWriteNumber(min);
		lcdSerialCursorLine4();
		lcdSerialWriteString("MaxP: ");
		lcdSerialWriteNumber(max);
	}
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

void ABTimerTestInterruptHandler(){
	gpioSetData(GPIO_PORTF,0x0C,~gpioGetData(GPIO_PORTF,0x0C));
	timerInterruptClear(TIMER_4);
}

void ABTimerTest(){
	gpioSetMasterEnable(GPIO_PORTF);
	gpioSetDirection(GPIO_PORTF,0x0C,0x0C);
	gpioSetDigitalEnable(GPIO_PORTF,0x0C,0x0C);
	timerSetup(TIMER_4,TIMER_CONFIG_PERIODIC,2);
	timerStart(TIMER_4);
	while(1);
}

void ABTestBinaryCounter(){
	binCounterInit(BINCOUNTER_GPIO_PORTD,BINCOUNTER_GPIO_PIN_1,
			BINCOUNTER_GPIO_PORTA,BINCOUNTER_GPIO_PIN_2,
			BINCOUNTER_GPIO_PORTA,BINCOUNTER_GPIO_PIN_3);

	int i = 0;
	while(1){
		binCounterClear();
		SysCtlDelay(10000000);

		for(i = 0; i < 254;i++){
			binCounterIncrease();
			SysCtlDelay(100000);
		}
		for(; i >=0;i--){
			binCounterDecrease();
			SysCtlDelay(100000);
		}

		for(i=127; i >=0;i--){
			binCounterDecrease();
			SysCtlDelay(100000);
		}
		SysCtlDelay(10000000);
	}
}
void ABTestButtonsInit(){
	ABTestLCDInit();
	gpioSetMasterEnable(GPIO_PORTB);
	gpioSetDigitalEnable(GPIO_PORTB,0x3F,0x3F);
	gpioSetDirection(GPIO_PORTB,0x3F,0x00);
}

void ABTestButtonsPool(){
	while(1){
		SysCtlDelay(1000000);
		lcdSerialClear();
		lcdSerialWriteString("Buttons: ");
		lcdSerialWriteNumber(gpioGetData(GPIO_PORTB,0x3F));
	}
}
void ABTestButtonsT1(){
	ABTestButtonsInit();
	ABTestButtonsPool();
}

void ABTestButtonsT2(){
	ABTestButtonsInit();
	gpioSetPullUpSelect(GPIO_PORTB,0x3F,0x3F);
	ABTestButtonsPool();
}

void ABTestButtonsCheckButton(char* msg, int result){
	lcdSerialWriteString(msg);
	if(result){
		lcdSerialWriteString("Y ");
	}
	else{
		lcdSerialWriteString("N ");
	}
}
void ABTestButtons(){
	ABTestLCDInit();
	buttonsInit();
	int pressed;
	//PF0
	//PF4
	gpioSetMasterEnable(GPIO_PORTF);
	gpioSetDirection(GPIO_PORTF,0x11,0x00);
	gpioSetDigitalEnable(GPIO_PORTF,0x11,0x11);
	while(1){
		lcdSerialCursorLine1();
		pressed = buttonsWasPressed();
		ABTestButtonsCheckButton("Buttons Pressed: ", pressed);
		lcdSerialCursorLine2();
		ABTestButtonsCheckButton("B0 ", buttonsWasPressedB0());
		ABTestButtonsCheckButton("B1 ", buttonsWasPressedB1());
		ABTestButtonsCheckButton("B2 ", buttonsWasPressedB2());
		lcdSerialCursorLine3();
		ABTestButtonsCheckButton("B3 ", buttonsWasPressedB3());
		ABTestButtonsCheckButton("B4 ", buttonsWasPressedB4());
		ABTestButtonsCheckButton("B5 ", buttonsWasPressedB5());
		lcdSerialCursorLine4();
		ABTestButtonsCheckButton("F0 ", gpioGetData(GPIO_PORTF,0x01));
		ABTestButtonsCheckButton("F4 ", gpioGetData(GPIO_PORTF,0x10));


		if(pressed){
			lcdSerialWriteString("[");
			SysCtlDelay(2000000);
			lcdSerialWriteString("=");
			SysCtlDelay(2000000);
			lcdSerialWriteString("=");
			SysCtlDelay(2000000);
			lcdSerialWriteString("=");
			SysCtlDelay(2000000);
			lcdSerialWriteString("]");
			SysCtlDelay(2000000);
			lcdSerialClear();
			buttonsEnable();
		}
	}
}



void ABTestUI(){
	ABUIInit();
	while(1){
		ABUIStateMachineBackgroundRun();
	}
}

void ABTestDHT(){
	ABTestLCDInit();
	dhtSetup();
	while(1){
		dht11getData();
		while(dhtIsActive());
		lcdSerialCursorLine1();
		lcdSerialWriteString("Humidity Sensor:");
		lcdSerialCursorLine2();
		lcdSerialWriteString("%: ");
		lcdSerialWriteNumber(dht11getHumidity());
		lcdSerialWriteString("  ");
		lcdSerialCursorLine4();
		lcdSerialWriteString("[Cancel]: Back");
	}
}


void ABTestLCDHex(){
	uint32_t number = 0x00;
	ABTestLCDInit();
	while(1){
		SysCtlDelay(1000000);
		lcdSerialCursorLine1();
		lcdSerialWriteNumber(number++);
	}
}

void ABTestPressureSensorArray(){
	ABTestLCDInit();
	bmp085ArrayTest(2,0);
}

int main(int argc, const char * argv[]){

	//ABTestUI();
	//ABTestDHT();
	//ABTestWindVane();
	//ABTestUI();
	//ABTestAnemometer();
	ABTestPressureSensorArray();
	while(1);

}


