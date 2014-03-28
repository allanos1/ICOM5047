#include "a.lib/ABTest.h"
#include "a.lib/timers.h"
#include "a.lib/lcdSerial.h"
#include "a.lib/ABTimer.h"

ABTime ref ;
ABTime delta ;


int state = 0;
void GPIOPortDHandler(){
	SysCtlDelay(20000);
	if(state){
		//ABTimerReset();
		//ABTimerStart();
		ref = ABTimerGetReference();
		state = 0;
	}
	else{
		//ABTimerStop();
		delta = ABTimerGetDelta(ABTimerGetReference(),ref);
		state = 1;
	}
	gpioSetInterruptClear(GPIO_PORTD,0x01,0x01);
}

int main(int argc, const char * argv[])
{


	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();

	gpioSetMasterEnable(GPIO_PORTD);
	gpioSetDigitalEnable(GPIO_PORTD,0x01,0x01);
	gpioSetDirection(GPIO_PORTD,0x01,0x00);
	gpioSetInterruptBothEdges(GPIO_PORTD,0x01,0x00);
	gpioSetInterruptEvent(GPIO_PORTD,0x01,0x00);
	gpioSetInterruptMaskDisable(GPIO_PORTD,0x01,0x01);
	gpioHelperInterruptMasterEnable();
	gpioSetInterruptEnable(GPIO_PORTD);

	////
	ABTimerInit(ABTIMER_BASE_TIMER_0,ABTIMER_RESOLUTION_MILLISECOND);
	ABTimerStart();
	/////
	while(1){

		ABTime now = ABTimerGetReference();
		/////
		lcdSerialCursorLine1();
		lcdSerialWriteString("Minutes: ");
		lcdSerialWriteNumber(now.minutes);
		lcdSerialCursorLine2();
		lcdSerialWriteString("Seconds: ");
		lcdSerialWriteNumber(now.seconds);
		lcdSerialCursorLine3();
		lcdSerialWriteString("Milliseconds: ");
		lcdSerialWriteNumber(now.milliseconds);
		lcdSerialCursorLine4();
		if(state){
			lcdSerialWriteNumber(delta.minutes);
			lcdSerialWriteString(":");
			lcdSerialWriteNumber(delta.seconds);
			lcdSerialWriteString(":");
			lcdSerialWriteNumber(delta.milliseconds);
		}
	}

}


