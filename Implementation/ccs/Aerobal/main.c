#include "a.lib/ABTest.h"
#include "a.lib/lcdSerial.h"
#include "a.lib/anemometer.h"
#include "a.lib/adc.h"

void anemometerTest(){
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

void adcTest(){
	lcdSerialInit(LCDSERIAL_INIT_UART3);
	lcdSerialSetContrast(0x44);
	lcdSerialClear();
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

int main(int argc, const char * argv[]){
	adcTest();
}


