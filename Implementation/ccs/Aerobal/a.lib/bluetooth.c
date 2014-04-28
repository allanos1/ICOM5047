/*
 * bluetooth.c
 *
 *  Created on: Feb 23, 2014
 *      Author: Jesus and Anthony
 */

#include "bluetooth.h"

int bluetoothBufferCounter = 0;

///////////////////////////////////
// API Layer 0

/* Initializes the bluetooth UART module to the given
 * uart and baud modes.
 * Test mode hardcoded as of v0.1.
 *
 * BLUETOOTH_UART0 : RX: A0  TX: A1
 * BLUETOOTH_UART1 : RX: C4  TX: C5
 * BLUETOOTH_UART2 : RX: D6  TX: D7
 * BLUETOOTH_UART3 : RX: C6  TX: C7
 * BLUETOOTH_UART4 : RX: C4  TX: C5
 * BLUETOOTH_UART5 : RX: E4  TX: E5
 * BLUETOOTH_UART6 : RX: D4  TX: D5
 * BLUETOOTH_UART7 : RX: E0  TX: E1
 */
void bluetoothInit(uint32_t uart, uint32_t uartBaud){
	bluetoothUART = uart;
	uartMasterEnable(uart,uartBaud);
	//To track number of events for testing.
	bluetoothEventCount = 0;
}

/*****************************
 * Interrupt Handler
 * | extern void bluetoothInterruptHandler(void);
 *****************************
 *
 * Bluetooth Interrupt Handler that empties the FIFO by
 * writing back the same characters to UART.
 *
 */
void bluetoothInterruptHandler(void){
	uartInterruptClear(bluetoothUART);
	gpioSetData(GPIO_PORTF,0x0C,0x0C);
	SysCtlDelay(100000);
	gpioSetData(GPIO_PORTF,0x0C,0x00);
	bluetoothEventCount++;
	while(uartHasAvailable(bluetoothUART)){
		//uartWriteCharSync(bluetoothUART,uartGetBufferCharSync(bluetoothUART));
		bluetoothStateMachine(uartGetBufferChar(bluetoothUART));
    }

}

/*
 * The State Machine that reads commands
 * from the software system of Aerobal. Follows
 * communication protocol v1.0.
 */
int bluetoothStateMachine(char c){
	switch(bluetoothState){
		case BLUETOOTH_STATE_1_B:
			if(c == 'b'){
				bluetoothState = BLUETOOTH_STATE_2_T;
			}
			else{
				bluetoothReset = 1;
			}
			break;
		case BLUETOOTH_STATE_2_T:
			if(c == 't'){
				bluetoothState = BLUETOOTH_STATE_3_C;
			}
			else{
				bluetoothReset = 1;
			}
			break;
		case BLUETOOTH_STATE_3_C:
			if(c == ':'){
				bluetoothState = BLUETOOTH_STATE_4_READ;
				bluetoothBufferCounter = 0;
			}
			else{
				bluetoothReset = 1;
			}
			break;
		case BLUETOOTH_STATE_4_READ:
			if(c == '\r' || c == '\n' || c == '#') {
				// TODO:
				// Testing...
				// Could be dangerous at high speeds.
				// - Emptying buffer might erase valid unprocessed chars.
				// - Buffer is not copied.
				bluetoothEmptyUartBuffer();
				bluetoothState = BLUETOOTH_STATE_1_B;
				bluetoothBuffer[bluetoothBufferCounter] = '\0';
				bluetoothBufferCounter = 0;
				bluetoothEvaluateBuffer(bluetoothBuffer);
			}
			else{
				bluetoothBuffer[bluetoothBufferCounter] = c;
				bluetoothBufferCounter++;
			}
			break;
		default:
			bluetoothReset = 1;
			break;
	}
	if(bluetoothReset){
		bluetoothState = BLUETOOTH_STATE_1_B;
		bluetoothReset = 0;
		return 0;
	}


	return 1;

}
/*
 * Empties the bluetooth buffer by reading all
 * of the characters available until it is empty.
 */
void bluetoothEmptyUartBuffer(){
	while(uartHasAvailable(bluetoothUART)){
		uartGetBufferCharSync(bluetoothUART);
	}
}

/* Sends the predefined terminator string
 * by issuing a carriage return '\r' and
 * a line feed '\n'.
 */
void bluetoothSendTerminator(){
	bluetoothSendString("\r\n");
}

/* Sends the acknowledge command 'ack'
 * followed by a terminator.
 */
void bluetoothSendAck(){
	bluetoothSendString("ack");
	bluetoothSendTerminator();
}


/* Sends the error command 'err'
 * followed by a terminator.
 */
void bluetoothSendErr(){
	bluetoothSendString("err");
	bluetoothSendTerminator();
}

/*
 * Sends a string over the preconfigured
 * bluetooth UART.
 */
void bluetoothSendString(char * s){
	int i = 0;
	for(i = 0; s[i] != '\0'; i++){
		uartWriteCharSync(bluetoothUART,s[i]);
	}
}

/*
 * Verifies if a string is a query or 'getter' by
 * checking the first character and evaluating
 * whether it is a '?' or not.
 */
int bluetoothIsQuery(char *value){
	return value[0] == '?';
}

/*
 * Gets the query by replacing the '?'
 * in the string.
 */
void bluetoothGetQuery(char * value){
	int i = 0;
	for(i = 1; value[i] != '\0'; i++){
		value[i+1] = value[i];
	}
	value[i-1] = '\0';
}

int bluetoothGetNumber(char value){
	return value-48;
}
/*
 * Evaluates the string coming from the
 * bluetooth communication.
 */
void bluetoothEvaluateBuffer(char *buffer){
	char command[5] ;
	char value[20] ;
	char out[20];

	stringSplit(buffer,'=',command,value);

	if(stringEquals("ps",command)){
		if(bluetoothIsQuery(value)){
			int pNumber = bluetoothGetNumber(value[1]);
			if(value[2]!='\0'){
				pNumber*=10;
				pNumber+bluetoothGetNumber(value[2]);
			}
			//GET PS ok!
			bluetoothSendAck();
			//TODO: Set Tunnel Pressure Sensor.
			stringFTOA(ABSSGetMPSAIndexPressure(pNumber),out);
			bluetoothSendString(out);
			bluetoothSendTerminator();
			bluetoothSendAck();
		}
		else{
			bluetoothSendErr();
		}
	}
	else if(stringEquals("hm",command)){
		if(bluetoothIsQuery(value)){
			//GET HM ok!
			bluetoothSendAck();
			stringFTOA(ABSSGetDHTHumidity(),out);
			//stringFTOA(143.987/(float)bluetoothEventCount,out);
			bluetoothSendString(out);
			bluetoothSendTerminator();
			bluetoothSendAck();
		}
		else{
			bluetoothSendErr();
		}
	}
	else if(stringEquals("tm",command)){
		if(bluetoothIsQuery(value)){
			//GET TM ok!
			bluetoothSendAck();
			stringFTOA(ABSSGetBMPTemperature(),out);
			bluetoothSendString(out);
			bluetoothSendTerminator();
			bluetoothSendAck();
		}
		else{
			bluetoothSendErr();
		}

	}
	else if(stringEquals("sp",command)){
			if(bluetoothIsQuery(value)){
				//GET WD ok!
				bluetoothSendAck();
				stringFTOA(ABSSGetAnemometerSpeed(),out);
				bluetoothSendString(out);
				bluetoothSendTerminator();
				bluetoothSendAck();
			}
			else{
				bluetoothSendErr();
			}
		}
	else if(stringEquals("wd",command)){
		if(bluetoothIsQuery(value)){
			//GET WD ok!
			bluetoothSendAck();
			stringFTOA(ABSSGetWindVaneDirection(),out);
			bluetoothSendString(out);
			bluetoothSendTerminator();
			bluetoothSendAck();
		}
		else{
			bluetoothSendErr();
		}
	}
	else if(stringEquals("lc",command)){
		if(bluetoothIsQuery(value)){
			if(stringEquals(value,"?front")){
				bluetoothSendAck();
				stringFTOA(ABSSGetLoadCellDragFront(),out);
				bluetoothSendString(out);
			}
			else if(stringEquals(value,"?back")){
				bluetoothSendAck();
				stringFTOA(ABSSGetLoadCellDragBack(),out);
				bluetoothSendString(out);
			}
			else if(stringEquals(value,"?up")){
				bluetoothSendAck();
				stringFTOA(ABSSGetLoadCellLiftUp(),out);
				bluetoothSendString(out);
			}
			else if(stringEquals(value,"?down")){
				bluetoothSendAck();
				stringFTOA(ABSSGetLoadCellLiftDown(),out);
				bluetoothSendString(out);
			}
			else if(stringEquals(value,"?left")){
				bluetoothSendAck();
				stringFTOA(ABSSGetLoadCellSideLeft(),out);
				bluetoothSendString(out);
			}
			else if(stringEquals(value,"?right")){
				bluetoothSendAck();
				stringFTOA(ABSSGetLoadCellSideRight(),out);
				bluetoothSendString(out);
			}
			else{
				bluetoothSendErr();
				return;
			}
			bluetoothSendTerminator();
			bluetoothSendAck();
		}
		else{
			bluetoothSendErr();
		}
	}
	else if(stringEquals("pss",command)){
		if(bluetoothIsQuery(value)){
			//GET PSS ok!
			bluetoothSendAck();
			bluetoothSendErr();
		}
		else{
			bluetoothSendErr();
		}
	}
	else if(stringEquals("fan",command)){
		//SET FAN ok!
		if(stringEquals(value,"on") || stringEquals(value,"off")){
			bluetoothSendString("fan:");
			bluetoothSendString(value);
			bluetoothSendTerminator();
			bluetoothSendAck();
		}
		else{
			bluetoothSendErr();
		}
	}
	else if(stringEquals("ln1",command)){
		//Write to LCD Line 1 ok!
		bluetoothSendString("line1=");
		bluetoothSendString(value);
		bluetoothSendTerminator();
		bluetoothSendAck();
	}
	else if(stringEquals("ln2",command)){
		//Write to LCD Line 2
		bluetoothSendString("line2=");
		bluetoothSendString(value);
		bluetoothSendTerminator();
		bluetoothSendAck();
	}
	else if(stringEquals("ln3",command)){
		//Write to LCD Line 3
		bluetoothSendString("line3=");
		bluetoothSendString(value);
		bluetoothSendTerminator();
		bluetoothSendAck();
	}
	else if(stringEquals("ln4",command)){
		//Write to LCD Line 4 ok
		bluetoothSendString("line4=");
		bluetoothSendString(value);
		bluetoothSendTerminator();
		bluetoothSendAck();
	}
	else if(stringEquals("ws",command)){
		//Set WS ok
		bluetoothSendString("ws:");
		bluetoothSendString(value);
		bluetoothSendTerminator();
		bluetoothSendAck();
	}
	else if(stringEquals("ack",command)){
		//Not Sure Yet.
		bluetoothSendAck();
	}
	else if(stringEquals("err",command)){
		//Not Sure Yet.
		bluetoothSendErr();
	}
	else{
		bluetoothSendErr();
	}
}


