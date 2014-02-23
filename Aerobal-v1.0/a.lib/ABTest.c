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
		SysCtlDelay(700000);
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
int ABTestLCD(){

	lcdInit(GPIO_PORTA, GPIO_PORTC, GPIO_PORTD);
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
int ABTestBMP(){
	lcdClear();
	ABTestLog("ABTL: BMP085","Begin Test..","","");
	bmp085Init();
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
int ABTestRelay(){
	lcdClear();
	ABTestLog("ABTL: Relay","Testing Relay","","");
	relayInit();
	int rCounter = 0;
	int rState = 0;
	while(rCounter < 6){
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
		lcdWriteNumber(6);
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
int ABTestBT(){

	lcdClear();
	bluetoothInit();
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



