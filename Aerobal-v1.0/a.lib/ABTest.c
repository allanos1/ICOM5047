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
	lcdCursorLine4();
	lcdWriteString("[");
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
		lcdCursorLine1();
		lcdWriteString(line1);
	}
	if(line2 != ""){
		lcdCursorLine2();
		lcdWriteString(line2);
	}
	if(line3 != ""){
		lcdCursorLine3();
		lcdWriteString(line3);
	}
	if(line4 != ""){
		lcdCursorLine4();
		lcdWriteString(line4);
	}
}

/*
 * Clears a line of the LCD at the specified
 * line number.
 */
void ABTestLogLineClear(char lineNumber){
	if(lineNumber <= LCD_CMD_CURSOR_POSITION_LINE_1){
		lcdCursorLine1();
		lcdWriteString("                    ");
	}
	else if(lineNumber == LCD_CMD_CURSOR_POSITION_LINE_2){
		lcdCursorLine2();
		lcdWriteString("                    ");
	}
	else if(lineNumber <= LCD_CMD_CURSOR_POSITION_LINE_3){
		lcdCursorLine3();
		lcdWriteString("                    ");
	}
	else if(lineNumber <= LCD_CMD_CURSOR_POSITION_LINE_4){
		lcdCursorLine4();
		lcdWriteString("                    ");
	}
}


//*******************
// Test Functions.
//*******************

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
			lcdCursorLine2();
	    	lcdWriteString("P: ");
	    	lcdWriteNumber(bmp085GetPressure());
	    	lcdCursorLine3();
	    	lcdWriteString("T: ");
	    	lcdWriteNumber(bmp085GetTemperature());
	    	lcdCursorLine4();
	    	lcdWriteNumber(counter);
	    	lcdWriteString("/");
	    	lcdWriteNumber(1000);
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
			ABTestLog("","","Testing On... ","");
			relayOn();
			ABTestDelay();
		}
		else{
			ABTestLog("","","Testing Off...","");
			relayOff();
			ABTestDelay();
		}
		rState = !rState;
		ABTestLogLineClear(LCD_CMD_CURSOR_POSITION_LINE_4);
		rCounter ++ ;
	}
	rCounter = 0;
	while(rCounter < 6){
		lcdCursorLine3();
		lcdWriteString("Testing Toggle: ");
		lcdWriteNumber(rCounter);
		relayToggle();
		ABTestDelay();
		ABTestLogLineClear(LCD_CMD_CURSOR_POSITION_LINE_4);
		rCounter ++ ;
	}
	ABTestLog("","Relay Tests       ", "Successful!","");
	ABTestDelay();
	return 1;
}

/* Performs a test for the Bluetooth Module.
 *
 * v0.1
 *
 * Ports:
 *
 */
int ABTestBT(){

}



