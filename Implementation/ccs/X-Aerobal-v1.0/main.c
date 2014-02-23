/************************************************/
// System Includes - ARM
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "driverlib/timer.h"
#include "driverlib/sysctl.h"
//#include "inc/hw_nvic.h"
//#include "inc/hw_types.h"
//#include "inc/hw_ints.h"
//#include "driverlib/debug.h"
//#include "driverlib/fpu.h"
//#include "driverlib/gpio.h"
//#include "driverlib/systick.h"
//#include "driverlib/uart.h"
//#include "driverlib/pin_map.h"
//#include "utils/uartstdio.h"
/***********************************************/
// Libraries
#include "a.lib/gpio.h"
#include "a.lib/lcd.h"
#include "a.lib/bmp085.h"
/***********************************************/

int ABTestLCD(){

	lcdInit(GPIO_PORTA, GPIO_PORTC, GPIO_PORTD);
	lcdClear();
	lcdWriteString("ABT: LCD");
	lcdCursorLine2();
	lcdWriteString("Test Complete.");
	lcdCursorLine3();
	SysCtlDelay(200000);
	lcdWriteString("[");
	SysCtlDelay(200000);
	lcdWriteString(".");
	SysCtlDelay(200000);
	lcdWriteString("]");
	return 1;
}

int ABTestBMP(){
	bmp085Init();
	int mNum = 0;
	while(1){
		bmp085DataRead();
		lcdCursorLine1();
	    lcdWriteString("NUM: ");
	    lcdWriteNumber(++mNum);
	    lcdCursorLine2();
	    lcdWriteString("P: ");
	    lcdWriteNumber(bmp085GetPressure());
	    lcdCursorLine3();
	    lcdWriteString("T: ");
	    lcdWriteNumber(bmp085GetTemperature());
	    lcdCursorLine4();
	}
}

int main(void){

	ABTestLCD();
	ABTestBMP();
}


