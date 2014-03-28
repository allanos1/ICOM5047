/*
 * ABTest.c
 *
 *  Created on: Feb 22, 2014
 *      Author: Anthony
 */

#include "ABTest.h"

//*******************
// Helper Functions.
//*******************

/* Automatically performs a delay for the system.
 * Exact time not yet known.
 *
 * v0.1
 */
void ABTestDelay(){
	ABTestLogLineClear(LCD_LINE_4);
	lcdWriteStringInLine(4,"[");
	SysCtlDelay(1000000);
	int i = 0;
	for(i = 0; i < 18; i++){
		lcdWriteString("=");
		SysCtlDelay(400000);
	}
	lcdWriteString("]");
	SysCtlDelay(2000000);
}

/* Helper function to perform printouts of the
 * test results.
 *
 * v1.0
 */
void ABTestLog(char* line1, char* line2, char* line3, char* line4){
	if(line1 != ""){
		ABTestLogLineClear(1);
		lcdWriteStringInLine(1,line1);
	}
	if(line2 != ""){
		ABTestLogLineClear(2);
		lcdWriteStringInLine(2,line2);
	}
	if(line3 != ""){
		ABTestLogLineClear(3);
		lcdWriteStringInLine(3,line3);
	}
	if(line4 != ""){
		ABTestLogLineClear(4);
		lcdWriteStringInLine(4,line4);
	}
}

/*
 * Clears a line of the LCD at the specified
 * line number.
 */
void ABTestLogLineClear(char lineNumber){
	if(lineNumber == LCD_LINE_1){
		lcdWriteStringInLine(1,"                    ");
	}
	else if(lineNumber == LCD_LINE_2){
		lcdWriteStringInLine(2,"                    ");
	}
	else if(lineNumber == LCD_LINE_3){
		lcdWriteStringInLine(3,"                    ");
	}
	else if(lineNumber == LCD_LINE_4){
		lcdWriteStringInLine(4,"                    ");
	}
}


//********************************************************************/
// Test Functions.
//********************************************************************/

/* Tester for the LCD.
 * Should be run first as of v0.1. Tests the
 * ability to write by displaying a 'successful'
 * message.
 *
 * v0.1
 *
 * Ports Used:
 * PA2 - Register Select
 * PA3 - Enable
 * PD0-PD3 - LCD D0-D3
 * PC4-PC7 - LCD D4-D7
 */
int ABTestLCD(int init){

	if(init) lcdInit(GPIO_PORTA, GPIO_PORTC, GPIO_PORTD);
	lcdClear();
	ABTestLog("ABTL: LCD","Test Successful","","");
	ABTestDelay();
	return 1;
}


/*
 * Tester for the BMP module. Performs measurements
 * without writing to the LCD to show performance.
 *
 * v0.5
 *
 * Ports:
 * PA6 - BMP SCL
 * PA7 - BMP SDA
 */
int ABTestBMP(int init){
	lcdClear();
	ABTestLog("ABTL: BMP085","Begin Test..","","");
	if(init) bmp085Init();
	int counter = 0;
	while(1){
		bmp085DataRead();
		if(counter %200 == 0){
			lcdWriteStringInLine(2,"P: ");
	    	lcdWriteNumber(bmp085GetPressure());
	    	lcdWriteStringInLine(3,"T: ");
	    	lcdWriteNumber(bmp085GetTemperature());
	    	lcdCursorLine4();
	    	lcdWriteNumber(counter);
	    	lcdWriteString("/");
	    	lcdWriteNumber(1000);
	    	lcdWriteString(" [");
	    	if(counter >= 200) lcdWriteString("."); else lcdWriteString(" ");
	    	if(counter >= 400) lcdWriteString("."); else lcdWriteString(" ");
	    	if(counter >= 600) lcdWriteString("."); else lcdWriteString(" ");
	    	if(counter >= 800) lcdWriteString("."); else lcdWriteString(" ");
	    	lcdWriteString("]");
		}
		if(counter == 1000){
			break;
		}
		counter++;
	}
	lcdClear();
	ABTestLog("ABTL: BMP","Test Successful!","","");
	ABTestDelay();
	return 1;
}

/*
 * Tester for the Relay Module. Tests the on, off,
 * and toggle functions.
 *
 * v0.1
 *
 * Ports:
 * PA5 - Relay In
 *
 */
int ABTestRelay(int init){
	lcdClear();
	ABTestLog("ABTL: Relay","Testing Relay","","");
	if(init) relayInit();
	int rCounter = 0;
	int rState = 0;
	while(rCounter < 4){
		if(rState){
			lcdWriteStringInLine(3,"Test On... ");
			relayOn();
		}
		else{
			lcdWriteStringInLine(3,"Test Off...");
			relayOff();
		}
		lcdWriteString(" ");
		lcdWriteNumber(rCounter+1);
		lcdWriteString("/");
		lcdWriteNumber(4);
		ABTestDelay();
		rState = !rState;
		ABTestLogLineClear(LCD_LINE_4);
		rCounter ++ ;
	}
	rCounter = 0;
	while(rCounter < 3){
		lcdWriteStringInLine(3,"Testing Toggle: ");
		lcdWriteNumber(rCounter+1);
		lcdWriteString("/");
		lcdWriteNumber(3);
		relayToggle();
		ABTestDelay();
		rCounter ++ ;
	}
	relayOff();
	lcdClear();
	ABTestLog("ABTL: Relay","Relay Tests", "Successful!","");
	ABTestDelay();
	return 1;
}

/* Performs a test for the Bluetooth Module.
 * v0.1
 *
 * Ports:
 * PA0 - UART0 RX
 * PA1 - UART0 TX
 * PF2 - GPIO LED
 */
int ABTestBT(int init){

	lcdClear();
	if(init) bluetoothInit();
	ABTestLog("ABTL: Bluetooth", "Write in terminal to","","");
	int count = -1;
	while(bluetoothEventCount < 5){
		if(count != bluetoothEventCount){
			lcdWriteStringInLine(LCD_LINE_3,"test.  ");
			lcdWriteNumber(bluetoothEventCount);
			lcdWriteString("/");
			lcdWriteNumber(5);
			count = bluetoothEventCount;
		}
		ABTestDelay();
	}
	lcdClear();
	ABTestLog("ABTL: Bluetooth","Test Successful!","","");
	ABTestDelay();

	return 1;
}

/* Performs a test for the DHT11 Humidity and Temperature
 * Sensor Module.
 *
 * v0.1
 *
 * Ports:
 * PA4 - GPIO DHT.
 *
 * Interrupts:
 * Timer 0A
 * | extern void dht11getData();
 *
 * Timer 1A
 * | extern void dht11count1uS()
 *
 * GPIOA IO - PIN 4
 * | extern void readDataBit()
 *
 */

int ABTestDHT(int init){
	lcdClear();
	if(init) dhtSetup();
	ABTestLog("ABTL: DHT11 test","","","");
	int count = 0;
	while(count < 5){
		dht11getData();
		while(dhtIsActive());
		lcdWriteStringInLine(2,"T: ");
		lcdWriteNumber(dht11getTemperature());
		lcdWriteStringInLine(3,"H: ");
		lcdWriteNumber(dht11getHumidity());
		lcdWriteString("   ");
		lcdWriteNumber(count);
		lcdWriteString("/");
		lcdWriteNumber(5);
		ABTestDelay();
		count++ ;
	}
	lcdClear();
	ABTestLog("ABTL: DHT11","Test Successful!","","");
	ABTestDelay();
	return 1;
}


/* Performs the test for the ADC module.
 *
 * v0.1
 *
 * Ports:
 * PE0 - PE4 : ADC inputs.
 *
 */
int ABTestAnalog(int init){
	lcdClear();
	if(init) loadCellSetup();
	ABTestLog("ABTL: ADC","Test ADC Input","Beginning...","");
	ABTestDelay();
	int count = 0;
	float adc0 ;
	float adc1 ;
	float adc2 ;
	lcdClear();
	while(count < 20){
		loadCellgetData();
		adc0 = loadCellgetValues(0,OUNCES); //PE3
		adc1 = loadCellgetValues(1,OUNCES); //PE2
		adc2 = loadCellgetValues(2,OUNCES); //PE1

		lcdWriteStringInLine(1,"ADC0: ");
		lcdWriteNumber(adc0);
		lcdWriteStringInLine(2,"ADC1: ");
		lcdWriteNumber(adc1);
		lcdWriteStringInLine(3,"ADC2: ");
		lcdWriteNumber(adc2);

		lcdWriteStringInLine(4,"Test: ");
		lcdWriteNumber(count);
		lcdWriteString("/");
		lcdWriteNumber(20);
		count++;
	}
	lcdClear();
	ABTestLog("ABTL: ADC", "Test Successful!","","");
	ABTestDelay();
	return 1;
}

/* Performs the test for the Servo module.
 *
 * v0.1
 *
 * Ports:
 * PF2 - GPIO PWM Outputs.
 *
 */
int ABTestServo(int init){
	lcdClear();
	if(init) servoSetupPWM();
	ABTestLog("ABTL: Servo","Testing:","","");
	ABTestDelay();
	int count = 0;
	int angle = 110;
	while(count < 7){
		servosetPositionPWM(angle,100); //Turn at full speed.
		lcdWriteStringInLine(2,"Testing: ");
		lcdWriteNumber(count);
		lcdWriteString("/7");
		lcdWriteStringInLine(3,"Angle: ");
		lcdWriteNumber(angle);
		ABTestDelay();
		angle+= 10;
		count++;
	}
	lcdClear();
	ABTestLog("ABTL: Servo","Test Successful.","","");
	ABTestDelay();
	return 1;
}

int ABTestButtons(int init){
	lcdClear();
	if(init) buttonsInit();
	ABTestLog("ABTL: Buttons","Testing:","","");
	ABTestDelay();
	int count = 0;
	while(count < 10){
		lcdWriteStringInLine(3,"Last: ");
		lcdWriteNumber(buttonsLastPressed);
		ABTestDelay();
		count++;
	}
	lcdClear();
	ABTestLog("ABTL: Buttons","Test Successful.","","");
	ABTestDelay();
	return 1;
}

int ABTestWindVane(int init){
	lcdClear();
	if(init) loadCellSetup();
	ABTestLog("ABTL: Wind Vane","Test ADC Input","Beginning...","");
	ABTestDelay();
	int count = 0;
	float adc0 ;
	lcdClear();
	ABTestLog("ABTL: ADC","","","");
	while(count < 40){
		loadCellgetData();
		adc0 = loadCellgetValues(0,OUNCES); //PE3
		lcdWriteStringInLine(1,"WV: ");
		lcdWriteNumber(adc0);
		lcdWriteStringInLine(4,"Test: ");
		lcdWriteNumber(count);
		lcdWriteString("/");
		lcdWriteNumber(40);
		count++;
	}
	lcdClear();
	ABTestLog("ABTL: Wind Vane", "Test Successful!","","");
	ABTestDelay();
	return 1;
}


