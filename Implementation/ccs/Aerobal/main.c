#include "a.lib/ABTest.h"
#include "a.lib/timers.h"
#include "a.lib/lcdSerial.h"
#include "a.lib/ABTimer.h"



ABTime ABTimerTimeMake2(){
	ABTime abt ;
	abt.hours = 0;
	abt.minutes = 0;
	return abt;
}


ABTime* timerIncrease(ABTime* time){
    ABTime * abt4 = (ABTime*) malloc(sizeof(ABTime));
    abt4->hours = (time->hours)+1;
    abt4->minutes = (time->minutes)+1;
    time->hours = time->hours+100;
    return abt4;
}

int main(int argc, const char * argv[])
{

    ABTime time;
    time = ABTimerTimeMake();
    time = ABTimerIncrease(time);
    time = ABTimerIncrease(time);
    time = ABTimerIncrease(time);
    time = ABTimerIncrease(time);
    time = ABTimerIncrease(time);



    ABTime * abt = (ABTime*) malloc(sizeof(ABTime));
    abt->minutes = 3;
    abt->hours = 3;

    ABTime * abt2 = timerIncrease(abt);

    abt->minutes = 2;

	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
	while(1){
	    time = ABTimerIncrease(time);
	    lcdSerialCursorLine1();
		lcdSerialWriteNumber((abt->hours));
		lcdSerialWriteString(" ");
		lcdSerialWriteNumber((abt->minutes));
		lcdSerialWriteString(" ");
		lcdSerialCursorLine2();
		lcdSerialWriteNumber((abt2->hours));
		lcdSerialWriteString(" ");
		lcdSerialWriteNumber((abt2->minutes));
		lcdSerialWriteString(" ");
		lcdSerialCursorLine3();
		lcdSerialWriteNumber((time.milliseconds));
		lcdSerialWriteString(" ");
		lcdSerialWriteNumber((time.microseconds));
		SysCtlDelay(1000);
	}

}


