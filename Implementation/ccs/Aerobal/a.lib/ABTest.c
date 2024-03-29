/*
 * ABTest.c
 *
 *  Created on: Feb 22, 2014
 *      Author: Anthony
 */

#include "ABTest.h"

void ABTestLCDInit(){
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
}

void ABTestFanRelay(){
	ABTestLCDInit();
	anemometerInit(ANEMOMETER_PORTF,ANEMOMETER_PIN4);
	anemometerStart();

	lcdSerialWriteString("Fan Relay Test:");
	motorAtvInit();
	gpioSetMasterEnable(GPIO_PORTB);
	gpioSetDirection(GPIO_PORTB,0x07,0x00);
	gpioSetDigitalEnable(GPIO_PORTB,0x07,0x07);
	gpioSetPullUpSelect(GPIO_PORTB,0x07,0x07);
	int state = 0;
	int toggle = 0;
	while(1){
		anemometerSpeedBufferRefresh();

		if(!gpioGetData(GPIO_PORTB,0x01) && !toggle){
			if(!state){
				motorAtvTurnOn();
				state = 1;
				lcdSerialCursorLine2();
				lcdSerialWriteString("Fan: On  ");
				lcdSerialCursorLine3();
				lcdSerialWriteString("B0: Pushed.   ");
			}
			else{
				motorAtvTurnOff();
				lcdSerialCursorLine2();
				lcdSerialWriteString("Fan: Off  ");
				lcdSerialCursorLine3();
				lcdSerialWriteString("B0: Pushed.   ");
				state = 0;
			}
			toggle = 1;
		}
		else if(!gpioGetData(GPIO_PORTB,0x01) && !toggle){
			lcdSerialCursorLine3();
			lcdSerialWriteString("B0: Pushed.   ");
		}
		else{
			lcdSerialCursorLine3();
			lcdSerialWriteString("B0: Not Pushed.");
			toggle = 0;
		}
		lcdSerialCursorLine4();
		lcdSerialWriteString("KM/H: ");
		lcdSerialWriteNumberWithBounds(anemometerSpeedConvertKmH(anemometerSpeedBufferGetAverage()),0,5);

		if(!gpioGetData(GPIO_PORTB,0x02)){
			motorAtvSpeedInc();
		}
		else if(!gpioGetData(GPIO_PORTB,0x04)){
			motorAtvSpeedDec();

		}
	}
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
	anemometerInit(ANEMOMETER_PORTF,ANEMOMETER_PIN4);
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
	adcMuxPinSet(ADC_0,ADC_MUX_0,ADC_PIN_IN03_PE0);
	adcMuxPinSet(ADC_0,ADC_MUX_1,ADC_PIN_IN02_PE1);
	adcMuxPinSet(ADC_0,ADC_MUX_2,ADC_PIN_IN01_PE2);
	adcMuxPinSet(ADC_0,ADC_MUX_3,ADC_PIN_IN00_PE3);
	adcMuxPinSet(ADC_0,ADC_MUX_4,ADC_PIN_IN09_PE4);
	adcMuxPinSet(ADC_0,ADC_MUX_5,ADC_PIN_IN08_PE5);
	adcMuxPinSet(ADC_0,ADC_MUX_6,ADC_PIN_IN07_PD0);
	adcSetSequencerSize(ADC_0, 7);
	int i = 0;
	while(1){
		i++;
		adcRefresh();
		int vE0 = adcDataGet(ADC_PIN_IN03_PE0);
		int vE1 = adcDataGet(ADC_PIN_IN02_PE1);
		int vE2 = adcDataGet(ADC_PIN_IN01_PE2);
		int vE3 = adcDataGet(ADC_PIN_IN00_PE3);
		int vE4 = adcDataGet(ADC_PIN_IN09_PE4);
		int vE5 = adcDataGet(ADC_PIN_IN08_PE5);
		int vD0 = adcDataGet(ADC_PIN_IN07_PD0);

		lcdSerialCursorLine1();
		lcdSerialWriteString("E0: ");
		lcdSerialWriteNumber(vE0);
		lcdSerialWriteString("  ");
		lcdSerialWriteString("D0: ");
		lcdSerialWriteNumber(vD0);
		lcdSerialWriteString(" ");
		lcdSerialCursorLine2();
		lcdSerialWriteString("E1: ");
		lcdSerialWriteNumber(vE1);
		lcdSerialWriteString("  ");
		lcdSerialWriteString("E2: ");
		lcdSerialWriteNumber(vE2);
		lcdSerialWriteString(" ");
		lcdSerialCursorLine3();
		lcdSerialWriteString("E3: ");
		lcdSerialWriteNumber(vE3);
		lcdSerialWriteString("  ");
		lcdSerialWriteString("E4: ");
		lcdSerialWriteNumber(vE4);
		lcdSerialWriteString(" ");
		lcdSerialCursorLine4();
		lcdSerialWriteString("E5: ");
		lcdSerialWriteNumber(vE5);
		lcdSerialWriteString("  ");
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

/*
 *
 */
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




/* Test Case
 *
 * Reads data from the desire quantity of sensors and send it through UART0
 * to the USB connected computer to be access by a serial monitor application.
 *
 */
void ABBmp085ArrayTest(int sensorIndex,int testNumber){
	ABTestLCDInit();
	lcdSerialClear();

	//
	// Begin Juan's Testing...
	// Requires activation of the bmp085ArraySynchronize interrupt
	// handler on GPIO Port D. Unresolved conflict.
	//

	if(testNumber == 0){

		uartMasterEnableNoInterrupt(UART0, UART_BAUD_9600);

		bmp085ArrayInit(GPIO_PORTD, GPIO_PIN_2 , GPIO_PORTD, GPIO_PIN_3, sensorIndex, true);

		lcdSerialClear();
		while(1){

			bmp085ArrayDataRead();

			lcdSerialCursorLine1();
			lcdSerialWriteString("MPSA Test:");
			lcdSerialCursorLine2();
			lcdSerialWriteString("Sensor: ");
			lcdSerialWriteNumber(bmp085ArrayGetCurrentSensor());
			lcdSerialWriteString("   ");
			lcdSerialCursorLine3();
			lcdSerialWriteString("T: ");
			lcdSerialWriteNumber(bmp085ArrayGetTemperature());
			lcdSerialWriteString("   ");
			lcdSerialCursorLine4();
			lcdSerialWriteString("P: ");
			lcdSerialWriteNumber(bmp085ArrayGetPressure());
			lcdSerialWriteString("   ");

			bmp085ArrayNextSensor();
			SysCtlDelay(100000*20);
		}
	}
	else if(testNumber == 1){
		bmp085ArrayInit(GPIO_PORTD, GPIO_PIN_2 , GPIO_PORTD, GPIO_PIN_3, sensorIndex, false);
		bmp085ArraySetCurrentSensor(5);
	}
	else if(testNumber == 2){

		uartMasterEnableNoInterrupt(UART0, UART_BAUD_9600);
		bmp085ArrayInit(GPIO_PORTD, GPIO_PIN_2 , GPIO_PORTD, GPIO_PIN_3, 2 , true);

		int i;
		char data[18];
		char number[2];
		char pressureSensorNumber[20];

		bmp085ArrayDataReadPosition(sensorIndex);
		stringFTOA(bmp085ArrayGetTemperature() , data);
		stringITOA(bmp085ArrayGetCurrentSensor(), number);
		stringConcat( "\nPressure\0", number, pressureSensorNumber);

		for(i = 0 ; pressureSensorNumber[i] != '\0'; i++){
			uartWriteCharSync(UART0, pressureSensorNumber[i]);
		}

		uartWriteCharSync(UART0, *":");

		for(i = 0 ; data[i] != '\0'; i++){
			uartWriteCharSync(UART0, data[i]);
		}
	}
	else if(testNumber == 3){

			uartMasterEnableNoInterrupt(UART0, UART_BAUD_9600);
			bmp085ArrayInit(GPIO_PORTD, GPIO_PIN_2 , GPIO_PORTD, GPIO_PIN_3, sensorIndex , false);
			bmp085StartDataAdquisition(sensorIndex);

		}

}

