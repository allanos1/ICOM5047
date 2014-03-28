/*
 * lcd.h
 *
 *  Created on: Sep 29, 2013
 *      Author: Anthony
 */

#ifndef LCD_H_
#define LCD_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "uart.h"

/****************************************************/
/*	Constant Definitions							*/
/****************************************************/
#define LCDSERIAL_INIT_UART0 0x00 //RX: A0  TX: A1
#define LCDSERIAL_INIT_UART1 0x01 //RX: C4  TX: C5
#define LCDSERIAL_INIT_UART2 0x02 //RX: D6  TX: D7
#define LCDSERIAL_INIT_UART3 0x03 //RX: C6  TX: C7
#define LCDSERIAL_INIT_UART4 0x04 //RX: C4  TX: C5
#define LCDSERIAL_INIT_UART5 0x05 //RX: E4  TX: E5
#define LCDSERIAL_INIT_UART6 0x06 //RX: D4  TX: D5
#define LCDSERIAL_INIT_UART7 0x07 //RX: E0  TX: E1

#define LCDSERIAL_COMMAND 0xFE //LCD Command Escape Character.
#define LCDSERIAL_CONFIG 0xFF //LCD Hardware Command Escape Character.

#define LCDSERIAL_COMMAND_CLEAR 0x01 //Clear the LCD screen.
#define LCDSERIAL_COMMAND_CURSOR_RIGHT 0x14 //Move the LCD Cursor one step to the right.
#define LCDSERIAL_COMMAND_CURSOR_LEFT 0x10 //Move the LCD Cursor one step to the left.
#define LCDSERIAL_COMMAND_DISPLAY_RIGHT 0x1C //Move all Characters to the right.
#define LCDSERIAL_COMMAND_DISPLAY_LEFT 0x18 //Move all characters to the left.
#define LCDSERIAL_COMMAND_DISPLAY_ON 0x0C //Turn on the display.
#define LCDSERIAL_COMMAND_DISPLAY_OFF 0x08 //Turn off the display.
#define LCDSERIAL_COMMAND_CURSOR_BLINK 0x0D //Make the cursor blink.
#define LCDSERIAL_COMMAND_CURSOR_POSITION_BASE 0x80 //Base of all cursor positions.

#define LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_1 0x00 //Offset for line 1.
#define LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_2 0x40 //Offset for line 2.
#define LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_3 0x54 //Offset for line 3.
#define LCDSERIAL_COMMAND_CURSOR_POSITION_LINE_4 0x55 //Offset for line 4.

#define LCDSERIAL_CONFIG_BAUD 0x01 //Baud hardware configuration command.
#define LCDSERIAL_CONFIG_BAUD_9600 0x00 //Baud rate 9600.
#define LCDSERIAL_CONFIG_BAUD_2400 0x01 //Baud rate 2400.
#define LCDSERIAL_CONFIG_BAUD_4800 0x02 //Baud rate 4800.
#define LCDSERIAL_CONFIG_BAUD_19200 0x03 //Baud rate 19200.
#define LCDSERIAL_CONFIG_BAUD_38400 0x04 //Baud rate 38400.
#define LCDSERIAL_CONFIG_SPLASHTOGGLE 0x02 //Toggle introduction from Jaycon Systems.
#define LCDSERIAL_CONFIG_CONTRAST 0x03 //Contrast hardware configuration command.
#define LCDSERIAL_CONFIG_BRIGHTNESS 0x04 //Brightness hardware configuration command.
#define LCDSERIAL_CONFIG_ROWNUMBER 0x05 //Number of Rows configuration command.
#define LCDSERIAL_CONFIG_ROWNUMBER_1 0x01 //Number of Rows: one.
#define LCDSERIAL_CONFIG_ROWNUMBER_2 0x02 //Number of Rows: two.
#define LCDSERIAL_CONFIG_ROWNUMBER_4 0x04 //Number of Rows: four.
#define LCDSERIAL_CONFIG_COLNUMBER 0x06 //Number of Columns configuration command.
#define LCDSERIAL_CONFIG_COLNUMBER_16 0x10 //Number of Columns: 16.
#define LCDSERIAL_CONFIG_COLNUMBER_20 0x14 //Number of Columns: 20.

#define LCDSERIAL_LINE_1 1 //Line 1 serial sub-definition.
#define LCDSERIAL_LINE_2 2 //Line 2 serial sub-definition.
#define LCDSERIAL_LINE_3 3 //Line 3 serial sub-definition.
#define LCDSERIAL_LINE_4 4 //Line 4 serial sub-definition.

/****************************************************/
/*	Variable Definitions							*/
/****************************************************/
uint32_t _lcdSerialSetupUART;
/****************************************************/
/*	Function Definitions							*/
/****************************************************/
//////////////////////////////////////////
// API Layer 0

void lcdSerialSendCommand(uint32_t command);
void lcdSerialSendConfig(uint32_t config);
void lcdSerialClear();
void lcdSerialCursorRight();
void lcdSerialCursorLeft();
void lcdSerialDisplayRight();
void lcdSerialDisplayLeft();
void lcdSerialDisplayOn();
void lcdSerialDisplayOff();
void lcdSerialCursorBlink();
void lcdSerialCursorLine1();
void lcdSerialCursorLine2();
void lcdSerialCursorLine3();
void lcdSerialCursorLine4();
void lcdSerialSetBaud9600();
void lcdSerialSetBaud2400();
void lcdSerialSetBaud4800();
void lcdSerialSetBaud19200();
void lcdSerialSetBaud38400();
void lcdSerialToggleSplash();
void lcdSerialSetContrast(uint32_t contrast);
void lcdSerialSetBrightness(uint32_t brightness);
void lcdSerialSetNumberOfRows1();
void lcdSerialSetNumberOfRows2();
void lcdSerialSetNumberOfRows4();
void lcdSerialSetNumberOfColumns16();
void lcdSerialSetNumberOfColumns20();

//////////////////////////////////////////
// API Layer 1
void lcdSerialInit(uint32_t uart);
void lcdSerialWriteLetter(char letter);
void lcdSerialWriteString(char* string);
void lcdSerialWriteNumber(double number);
void lcdSerialWriteNumberWithBounds(double number, int integerDigits, int rationalDigits);


//////////////////////////////////////////
// API Layer 2
void lcdSerialClearLine(uint32_t line);
void lcdSerialWriteStringInLine(uint32_t line, char* string);

#endif /* LCD_H_ */
