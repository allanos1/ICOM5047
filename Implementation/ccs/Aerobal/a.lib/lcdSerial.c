/*
 * lcd.c
 *
 *  Created on: March 1, 2014
 *      Author: Anthony
 */

#include "lcdSerial.h"

//////////////////////////////////////////
// API Layer 0

/*
 * Writes a Value through the UART of the
 * microprocessor to the Serial LCD Backpack
 * from Jaycon Systems
 */
void lcdSerialWrite(uint32_t value){
	uartWriteCharSync(_lcdSerialSetupUART,value);
}
/*
 * Issues a command by first writing the command
 * sequence to the LCD and then writing the command.
 */
void lcdSerialSendCommand(uint32_t command){
	lcdSerialWrite(LCDSERIAL_COMMAND);
	lcdSerialWrite(command);
	SysCtlDelay(2000);
}

/*
 * Issues a hardware configuration command by first writing
 * the hardware configuration sequence and then writing the
 * hardware command.
 */
void lcdSerialSendConfig(uint32_t config){
	lcdSerialWrite(LCDSERIAL_CONFIG);
	lcdSerialWrite(config);
}

/*
 * Issues a 'clear screen' command. Clears the
 * screen from all characters.
 */
void lcdSerialClear(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CLEAR);
}

/*
 * Issues a 'Cursor Right' command. Moves the cursor one
 * character to the right.
 */
void lcdSerialCursorRight(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_RIGHT);
}

/*
 * Issues a 'Cursor Left' Command. Moves the cursor one
 * character to the left.
 */
void lcdSerialCursorLeft(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_LEFT);
}

/*
 * Issues a 'Display Right' command. Moves all characters
 * one block to the right.
 */
void lcdSerialDisplayRight(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_DISPLAY_RIGHT);
}

/*
 * Issues a 'Display Left' command. Moves all characters
 * one block to the left.
 */
void lcdSerialDisplayLeft(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_DISPLAY_LEFT);
}

/*
 * Issues a 'Display On' command. Turns on the display.
 */
void lcdSerialDisplayOn(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_DISPLAY_ON);
}

/*
 * Issues a 'Display Off' command. Turns off the display.
 */
void lcdSerialDisplayOff(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_DISPLAY_OFF);
}

/*
 * Issues a 'Cursor Blink' command. Makes the cursor blink.
 */
void lcdSerialCursorBlink(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_BLINK);
}

/*
 * Issues a 'Position Cursor' command. Moves the cursor
 * to line 1.
 */
void lcdSerialCursorLine1(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_POSITION_BASE
			+ LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_1);
}

/*
 * Issues a 'Position Cursor' command. Moves the cursor
 * to line 2.
 */
void lcdSerialCursorLine2(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_POSITION_BASE
			+ LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_2);
}

/*
 * Issues a 'Position Cursor' command. Moves the cursor
 * to line 3.
 */
void lcdSerialCursorLine3(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_POSITION_BASE
			+LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_3);
}

/*
 * Issues a 'Position Cursor' command. Moves the cursor
 * to line 4.
 */
void lcdSerialCursorLine4(){
	lcdSerialSendCommand(LCDSERIAL_COMMAND_CURSOR_POSITION_BASE
			+ LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_4);
}

/*
 * Issues a 'Serial Config' command. Configures the baud rate
 * of the LCD to be 9600. Must be issued within one second of
 * power up.
 */
void lcdSerialSetBaud9600(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_BAUD);
	lcdSerialWrite(LCDSERIAL_CONFIG_BAUD_9600);
}

/*
 * Issues a 'Serial Config' command. Configures the baud rate
 * of the LCD to be 2400. Must be issued within one second of
 * power up.
 */
void lcdSerialSetBaud2400(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_BAUD);
	lcdSerialWrite(LCDSERIAL_CONFIG_BAUD_2400);
}

/*
 * Issues a 'Serial Config' command. Configures the baud rate
 * of the LCD to be 4800. Must be issued within one second of
 * power up.
 */
void lcdSerialSetBaud4800(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_BAUD);
	lcdSerialWrite(LCDSERIAL_CONFIG_BAUD_4800);
}

/*
 * Issues a 'Serial Config' command. Configures the baud rate
 * of the LCD to be 19200. Must be issued within one second of
 * power up.
 */
void lcdSerialSetBaud19200(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_BAUD);
	lcdSerialWrite(LCDSERIAL_CONFIG_BAUD_19200);
}

/*
 * Issues a 'Serial Config' command. Configures the baud rate
 * of the LCD to be 38400. Must be issued within one second of
 * power up.
 */
void lcdSerialSetBaud38400(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_BAUD);
	lcdSerialWrite(LCDSERIAL_CONFIG_BAUD_38400);
}

/*
 * Issues a 'Splash Configuration' command. Toggles the welcome
 * screen set by default on the Jaycon Systems Serial Backpack.
 */
void lcdSerialToggleSplash(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_SPLASHTOGGLE);
}

/*
 * Issues a 'Contrast Configuation' command. Sets the contrast
 * level of the LCD to the level supplied. Have the option
 * of 0x00 until 0xFF.
 */
void lcdSerialSetContrast(uint32_t contrast){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_CONTRAST);
	lcdSerialWrite(contrast);
}

/*
 * Issues a 'Backlight Brightness Configuration' command. Sets
 * the brightness of the screen to the level supplied. Have the
 * option of 0x00 until 0xFF.
 */
void lcdSerialSetBrightness(uint32_t brightness){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_BRIGHTNESS);
	lcdSerialWrite(brightness);
}

/*
 * Issues a 'Number of Rows Configuration' command. Sets the
 * number of rows the LCD will be using for 1 row.
 */
void lcdSerialSetNumberOfRows1(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_ROWNUMBER);
	lcdSerialWrite(LCDSERIAL_CONFIG_ROWNUMBER_1);
}

/*
 * Issues a 'Number of Rows Configuration' command. Sets the
 * number of rows the LCD will be using for 2 rows.
 */
void lcdSerialSetNumberOfRows2(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_ROWNUMBER);
	lcdSerialWrite(LCDSERIAL_CONFIG_ROWNUMBER_2);
}

/*
 * Issues a 'Number of Rows Configuration' command. Sets the
 * number of rows the LCD will be using for 4 rows.
 */
void lcdSerialSetNumberOfRows4(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_ROWNUMBER);
	lcdSerialWrite(LCDSERIAL_CONFIG_ROWNUMBER_4);
}

/*
 * Issues a 'Number of Columns Configuration' command. Sets the
 * number of columns the LCD will be using for 16 columns.
 */
void lcdSerialSetNumberOfColumns16(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_COLNUMBER);
	lcdSerialWrite(LCDSERIAL_CONFIG_COLNUMBER_16);
}

/*
 * Issues a 'Number of Columns Configuration' command. Sets the
 * number of columns the LCD will be using for 20 columns.
 */
void lcdSerialSetNumberOfColumns20(){
	lcdSerialSendConfig(LCDSERIAL_CONFIG_COLNUMBER);
	lcdSerialWrite(LCDSERIAL_CONFIG_COLNUMBER_20);
}

////////////////////////////////////////////
// API Layer 1

/*
 * Initialize the LCD for use with a given UART
 * port. The method writes 'Setup Complete' to the
 * LCD.
 *
 * UART0 : RX: A0  TX: A1
 * UART1 : RX: RX: C4  TX: C5
 * UART2 : RX: RX: D6  TX: D7
 * UART3 : RX: C6  TX: C7
 * UART4 : RX: C4  TX: C5
 * UART5 : RX: E4  TX: E5
 * UART6 : RX: D4  TX: D5
 * UART7 : RX: E0  TX: E1
 */
void lcdSerialInit(uint32_t uart){
	uartMasterEnableNoInterrupt(uart,UART_BAUD_9600);
	_lcdSerialSetupUART = uart;
	lcdSerialClear();
	lcdSerialWriteString("Setup Complete.");
}

/*
 * Writes a letter to the LCD and the current
 * cursor.
 */
void lcdSerialWriteLetter(char letter){
	lcdSerialWrite(letter);
}

/*
 * Writes a string to the LCD by limiting the
 * number of strings to 20 or the end of the
 * string.
 */
void lcdSerialWriteString(char* string){
	int i = 0;
	for(i=0;string[i]!='\0' && i < 20;i++){
		//TODO:
		//Overline protection: maybe not needed.
		lcdSerialWrite((char)string[i]);
	}
}


/* Non-API Function.
 * Writes a single digit to the LCD and the current
 * cursor.
 *
 * Must ensure that number is not greater than 48+9
 *
 */
void lcdSerialWriteSingleDigit(char number){
	lcdSerialWrite(number+48);
}


/* Non-API function.
 * Substitutes the math.pow function.
 *
 */
int lcdSerialMathPower(int number, int exponent){
	int result = 1 ;
	int i = 0;
	while (i < exponent) {
		result *= number;
		i++;
	}
	return result;
}

/* LCD Function to write the digits of a number.
 * Does not work on decimals, they must be multiplied
 * and then written.
 * Works to a seven. digit precision.
 */
void lcdSerialWriteDigits(int number){
	//Aparrently I wrote it to seven digits, but have never needed such
	//range.

	//(number/ (10^n) % 10 -> gives the (n+1)th digit.
	if(number >= 1000000) lcdSerialWriteSingleDigit((number/1000000) % 10) ;
	if(number >= 100000) lcdSerialWriteSingleDigit((number/100000) % 10) ;
	if(number >= 10000) lcdSerialWriteSingleDigit((number/10000) % 10) ;
	if(number >= 1000) lcdSerialWriteSingleDigit((number/1000) % 10) ;
	if(number >= 100) lcdSerialWriteSingleDigit((number/100) % 10) ;
	if(number >= 10) lcdSerialWriteSingleDigit((number/10) % 10) ;
	if(number >= 0) lcdSerialWriteSingleDigit((number) % 10) ;
}

/*
 * Writes a rational number to the LCD with maximum of 7 digits on both
 * the integer and rational parts. Truncates zeros on both sides.
 */
void lcdSerialWriteNumber(double number){
	//Write '-' if negative.
	if(number < 0) {
		lcdSerialWriteLetter(45);
		//Negative written, not needed anymore.
		number *= -1;
	}
	//Use the function to write the integer part.
	lcdSerialWriteDigits((int)number);

	//Get the fractional part and shift it to the integer part.
	//Done by eliminating the fractional part and subtracting
	//that to the original number, leaving only the fractional
	//part in the original number. Then, the number is shifted
	//in this case, six places to the left into the integer part.
	int decimals = (number - (double)((int) number)) * 1000000.0;

	//If there was indeed a fractional part, write it.
	if(decimals != 0){
		//Write the dot '.' (46 happens to be the 'char' value for the '.')
		lcdSerialWriteLetter(46);

		//Eliminate the unnecesary non-significant trailing zeros.
		while(decimals%10 == 0){
			decimals /= 10;
		}
		//Use the function again to write the number.
		lcdSerialWriteDigits(decimals);
	}
}
/*
 * Writes a number to LCD with a bound in the number of digits displayed in the
 * integer part and the rational part. The limit of digits is seven.
 *
 */
void lcdSerialWriteNumberWithBounds(double number, int integerDigits, int rationalDigits){

	integerDigits = (integerDigits < 0) ? 0 : integerDigits;
	rationalDigits = (rationalDigits < 0) ? 0 : rationalDigits;
	integerDigits = (integerDigits > 6) ? 6 : integerDigits;
	rationalDigits = (rationalDigits > 6) ? 6 : rationalDigits;


	if(number < 0) {
		lcdSerialWriteLetter(45);
		number *= -1;
	}

	int cap =lcdSerialMathPower(10,integerDigits-1);
	while(cap > (int) number) {
		lcdSerialWriteSingleDigit(0);
		cap /= 10;
	}
	if(number != 0)
	lcdSerialWriteDigits((int)number);

	//Decimal Part VVVVV

	int decimals = (number - (double)((int) number)) * 1000000.0;
	int i = 0;
	if(decimals != 0 && rationalDigits != 0){
		//0.123456
		//0.1234
		lcdSerialWriteLetter(46);
		while(i < 6-rationalDigits){
			decimals /= 10;
			i++;
		}

		lcdSerialWriteDigits(decimals);
	}
	else{
		if(rationalDigits > 0){
			lcdSerialWriteLetter(46);
			while(i < rationalDigits){
				lcdSerialWriteSingleDigit(0);
				i++;
			}
		}
	}
}

////////////////////////////////////////////
// API Layer 2

/*
 * Clears a line from the LCD.
 */
void lcdSerialClearLine(uint32_t line){
	char* c = "                 ";
	if(line == LCDSERIAL_LINE_1){
		lcdSerialCursorLine1();
	}
	else if(line == LCDSERIAL_LINE_2){
		lcdSerialCursorLine2();
	}
	else if(line == LCDSERIAL_LINE_3){
		lcdSerialCursorLine3();
	}
	else{
		lcdSerialCursorLine4();
		c = "                ";
	}
	lcdSerialWriteString(c);
}

/*
 * Write a string to a particular line of the LCD.
 * The line is cleared before writing.
 */
void lcdSerialWriteStringInLine(uint32_t line, char* string){
	lcdSerialClearLine(line);
	if(line == LCDSERIAL_LINE_1){
		lcdSerialCursorLine1();
	}
	else if(line == LCDSERIAL_LINE_2){
		lcdSerialCursorLine2();
	}
	else if(line == LCDSERIAL_LINE_3){
		lcdSerialCursorLine3();
	}
	else{
		lcdSerialCursorLine4();
	}
	lcdSerialWriteString(string);
}
