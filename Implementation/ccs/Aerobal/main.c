#include "a.lib/ABTest.h"
#include "a.lib/lcdSerial.h"
#include "a.lib/anemometer.h"

float getSpeed(int anemometerCount, ABTime now){

	float seconds = (float) now.minutes*60
			+ (float)now.seconds
			+ ((float)now.milliseconds)/1000.0;
	float anemometerRevolutions = ((float)anemometerCount) / 3.0;

	float rev_s = anemometerRevolutions/seconds; //rev/s
	float baseSpeed = 0.098 ; //m/s
	float ms = rev_s *baseSpeed;
	return ms*3.6;

}
int main(int argc, const char * argv[])
{

	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();

	anemometerInit(ANEMOMETER_PORTD,ANEMOMETER_PIN0);
	anemometerStart();
	ABTime now;

	while(1){
		now = anemometerSpeedBufferRefresh();
		lcdSerialCursorLine1();
		lcdSerialWriteString("Count: ");
		lcdSerialWriteNumber(anemometerGetCount());
		lcdSerialCursorLine2();
		lcdSerialWriteString("Time: ");
		lcdSerialWriteNumber(now.minutes);
		lcdSerialWriteString(":");
		lcdSerialWriteNumber(now.seconds);
		lcdSerialWriteString(":");
		lcdSerialWriteNumber(now.milliseconds);
		lcdSerialCursorLine3();
		lcdSerialWriteString("B-Speed KM: ");
		lcdSerialWriteNumber(anemometerSpeedConvertKmH(anemometerSpeedBufferGetAverage()));
		lcdSerialCursorLine4();
		lcdSerialWriteString("B-Speed MI: ");
		lcdSerialWriteNumber(anemometerSpeedConvertMiH(anemometerSpeedBufferGetAverage()));

	}

}


