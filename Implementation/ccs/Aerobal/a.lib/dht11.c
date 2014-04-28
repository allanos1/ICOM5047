/*
 * dht11.c
 * v1.0
 * Created on: Nov 6, 2013
 * Author: Juan Lebron
 */
/****************************************************/
/*					Instructions					*/
/****************************************************/
/*
 * Set DHT11 Timer0Interrupt to dht11getData()
 * Set DHT11 Timer1Interrupt to dht11count1uS()
 * Set GPIO_PORT_A Interrupt(pin A) to readDataBit();
 */

#include "dht11.h"

void dht11init();
void dht11getData();
void dht11request();
void dht11response();
void dht11responseStatus();
void dht11dataReading();
void dht11count1uS();
void initTimer0Module();
void initTimer1Module();
void initTimeControlModule();
void endTimeControlModule();
void readDataBit();
void bitTimeElapsed();
void decodeData();
void convertTimetoBits();
void dhtSetActive();
void dhtSetInactive();
int dhtIsActive();


/****************************************************/
/*	Variable Definitions							*/
/****************************************************/

int bitsReceivedTiming[40];
int dataBitsReceived[40];

/****************************************************/
/*				Function Definitions				*/
/****************************************************/

/****Main Functions****/
void dhtSetActive(){
	_dhtActive = 1;
}
void dhtSetInactive(){
	_dhtActive = 0;
}
int dhtIsActive(){
	return _dhtActive;
}

int dht11getHumidity(){
	return humidity;
}

/*
 * Counts the number of digits in a number.
 */
int dhtCountDigits(int number){
	if(number < 10){
		return 1;
	}
	int count = 0;
	while(number > 0){
		number /= 10;
		count++;
	}
	return count;
}
/*
 * Returns  the decimal part of the humidity measurement.
 */
float dhtGetDecimalHumidity(){
	return ((float)decimalHumidity)/dhtPow(10,dhtCountDigits(decimalHumidity));
}

/*
 * Returns the decimal part of the temperature measurement.
 */
float dhtGetDecimalTemperature(){
	return ((float)decimalTemp)/dhtPow(10,dhtCountDigits(decimalTemp));
}

/* Computes the DHT22 formula according to the AdaFruit Library
 * (formula is not specified in datasheet).
 */
float dht22GetHumidity(){
	return ((((float) humidity) * 256.0) + ((float) decimalHumidity))/10.0;
}

/*
 * Computes the DHT22 formula according to the AdaFruit Library.
 * (formula is not specified in datasheet.
 */
float dht22GetTemperature(){
	return (((temp&0x7F)*256.0) + decimalTemp)/10.0;

}
int dht11getTemperature(){
	return temp;
}
int dht11dataValidationStatus(){
	if(sum == sumDataCheck){
		return 1;
	}
	else{
		dht11init();
		return 0;
	};
}

void dhtSetup(){

	dht11ClockSetup();


	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	GPIOPinConfigure(GPIO_PORTA_BASE);
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_4);

	IntMasterEnable();

	//Timer 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	TimerConfigure(TIMER0_BASE, TIMER_CFG_ONE_SHOT);
	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());
	IntEnable(INT_TIMER0A);

	//Timer 3
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER3);
	TimerConfigure(TIMER3_BASE, TIMER_CFG_PERIODIC);
	TimerLoadSet(TIMER3_BASE, TIMER_A, SysCtlClockGet()/1000000);
	IntEnable(INT_TIMER3A);

	dht11init();
}
//****Helper Functions****/

/* Private function.
 *Initialize port PB1 for data Output and set port value to HIGH.
 */
void dht11init(){

	//HWREG(GPIO_PORTB | GPIO_OFFSET_DATA) |= 0x02; //Clean Data
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,GPIO_PIN_4);
	GPIOPinWrite(GPIO_PORTA_BASE,0x10,0x10);

	TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet());

	IntMasterEnable();
	IntEnable(INT_TIMER0A);
	IntEnable(INT_TIMER3A);

	responseStatusOk0 = 0;
	responseStatusOk1 = 0;

	dataBitReadStatus = 0;

	bitTimingStatus = 0;

	process = 0;
	step = 0;

	status0 = 0;
	status1 = 0;

	sum = 0;

	dht11ClockSetup();
}
void dht11getData(){


	dhtSetActive();
	TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

	switch(process){

	case REQUEST:
		dht11request();

		break;

	case RESPONSE:

		dht11response();

		break;

	case CHECKIFRESPONSEISOK:

		dht11responseStatus();

		break;

	case DATAREADING:

		dht11dataReading();
		break;

	}

}

void dht11request(){
	switch
	(step){
	case REQUEST18MS:
		//HWREG(GPIO_PORTA | GPIO_OFFSET_DATA) &= 0xFD;
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0x00);
		TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/55); //Wait 18ms
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerEnable(TIMER0_BASE, TIMER_A);
		step++;
		break;
	case REQUEST40US:
		//HWREG(GPIO_PORTA | GPIO_OFFSET_DATA) |= 0x02;
		GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,GPIO_PIN_4);
		TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/25000); //Wait 40us
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerEnable(TIMER0_BASE, TIMER_A);
		process++;
		step = 0;
		break;
	}
}

void dht11response(){
	//gpioSetData(GPIO_PORTB,0x02, 0x00);

	//gpioSetDirection(GPIO_PORTB,0x02, 0x00);
	//GPIOPinTypeGPIOInput(GPIO_PORTB_BASE,GPIO_PIN_1);
	GPIOPinWrite(GPIO_PORTA_BASE,GPIO_PIN_4,0x00);
	GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_DIR_MODE_IN);
	//GPIOPadConfigSet();
	switch(step){
	case RESPONSE54US:

		TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/37000); //Wait 54us/2 = 27us
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerEnable(TIMER0_BASE, TIMER_A);

		step++;
		break;
	case RESPONSE80US:
		status0 = HWREG(GPIO_PORTA | GPIO_OFFSET_DATA) & 0x10;
		responseStatusOk0 = (!status0) ? 1:0; //if a low on pin 1

		TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/14900);	//Wait 67us
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerEnable(TIMER0_BASE, TIMER_A);

		process++;
		step = 0;
		break;
	}
}

void dht11responseStatus(){
	switch(step){
	case CHECKIFRESPONSEISOKSTEP:
		status1 = HWREG(GPIO_PORTA | GPIO_OFFSET_DATA) & 0x10;
		responseStatusOk1 = (status1) ? 1:0; //if a high on pin 1

		if(responseStatusOk0 == 1 && responseStatusOk1 ==  1){
			; //DO NOTHING
		}
		else{
			lcdSerialClear();
			lcdSerialWriteString("Bad Response.");
			SysCtlDelay(1000000);
			dht11init();
		}

		TimerLoadSet(TIMER0_BASE, TIMER_A, SysCtlClockGet()/16666);	//Wait 40us
		TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		TimerEnable(TIMER0_BASE, TIMER_A);


		process++;
		step = 0;
		break;
	};
}

void dht11dataReading(){

		switch(step){
		case DATAREADINGINITSTEP:

			initTimeControlModule();
			initTimer1Module();

			step = 0;
			break;

		case DATAREADINGSTEP:
			step = 0;
			break;
		}

}

void dht11ClockSetup(){
	SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
}

void readDataBit(){
	if(dataBitReadStatus < 40){
	switch(bitTimingStatus){

		case 0:
			HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_CLEAR) |= 0x10;
			bitEntryTime = count1uS;
			endTimeControlModule();
			bitTimingStatus = 1;
			break;
		case 1:
			HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_CLEAR) |= 0x10;
			bitExitTime = count1uS;

			bitTimeElapsed();
			initTimeControlModule();

			bitTimingStatus = 0;
			count1uS = 0;
			break;

	}
	}
	else{
		convertTimetoBits();
		decodeData();
		IntDisable(INT_GPIOA);
		//IntMasterDisable();
		IntDisable(INT_TIMER0A);
		TimerDisable(TIMER0_BASE, TIMER_A);
		TimerIntDisable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
		IntDisable(INT_TIMER3A);
		TimerDisable(TIMER3_BASE, TIMER_A);
		TimerIntDisable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
		dhtSetInactive();
	}
}

void bitTimeElapsed(){

	bitsReceivedTiming[dataBitReadStatus] = bitExitTime - bitEntryTime;

	dataBitReadStatus++;

}

void convertTimetoBits(){
	int i = 0;
	for(i = 0;i < 40;i++){

		if(bitsReceivedTiming[i] > 0 && bitsReceivedTiming[i] <= 24){ //quantities on uS
				dataBitsReceived[i] = 0;

			}
		if(bitsReceivedTiming[i] > 24){
				dataBitsReceived[i] = 1;

			}
	}

}


void initTimeControlModule(){

	HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_BOTH_EDGES) &= 0xEF; //Delegate to Event Register 408
	HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_MASK) |= 0x10; //Interrupt mask 410
	HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_EVENT)|= 0x10; //Event Register 40C

	IntEnable(INT_GPIOA);

}


void endTimeControlModule(){

	//HWREG(GPIO_PORTB | 0x408)|= 0x02; //Delegate to Event Register
	//HWREG(GPIO_PORTB | 0x410)|= 0x02; //Interrupt mask
	HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_EVENT) &= 0xEF; //Event Register
	HWREG(GPIO_PORTA | GPIO_OFFSET_INTERRUPT_CLEAR) |= 0x10;
	IntEnable(INT_GPIOA);
}

void initTimer1Module(){

	TimerIntEnable(TIMER3_BASE, TIMER_TIMA_TIMEOUT);

	TimerEnable(TIMER3_BASE, TIMER_A);

}

void dht11count1uS(){
	TimerIntClear(TIMER3_BASE, TIMER_TIMA_TIMEOUT);
	count1uS++;
}

float dhtPow(int base, int exponent){
	int j = 1;
	int power = 1;

	for(j = 0;j < exponent; j++){
		power = power * base;
	}
	return power;
}

void decodeData(){
	sum = 0;
	int i = 0;
	humidity = 0;
	decimalHumidity = 0;
	temp = 0;
	decimalTemp = 0;
	sumDataCheck = 0;

	for(i = 0; i < 8;i++){
		humidity += dataBitsReceived[7 - i] * dhtPow(2,i);
	}
	for(i = 8; i < 16;i++){
		decimalHumidity += dataBitsReceived[15 - i + 8] * dhtPow(2,i - 8);
	}
	for(i = 16; i < 24;i++){
		temp += dataBitsReceived[23 - i + 16] * dhtPow(2,i - 16);
	}
	for(i = 24; i < 32;i++){
		decimalTemp += dataBitsReceived[31 - i + 24] * dhtPow(2,i - 24);
	}
	for(i = 32; i < 40;i++){
		sumDataCheck += dataBitsReceived[39 - i + 32] * dhtPow(2,i - 32);
	}

	sum = temp + decimalTemp + humidity + decimalHumidity;

}

