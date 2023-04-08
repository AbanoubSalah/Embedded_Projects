/**************************************************************************
 * File: util.h
 * Author: Abanoub Salah
 * AtMega169p 16F1717: w/X OSC @ 16MHz, 5v
 * Program: Header file of modclk functions
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *
 * Program Description: This program header interface for modclk functions
 *
 * Created on Oct 25, 2021
 *************************************************************************/
 
#define SW_PIN								PINB
#define SW_PORT								PORTB
#define SW_DDR								DDRB
#define SEL_SET								PB0
#define SEL_INC								PB1
#define SEL_DEC								PB2

#define ONE_SECOND_OCR						0x3D09
#define CLOCK_PARTS_COUNT					3
#define CLOCK_CHARACTER_PER_PART			3
#define DEBOUNCE_TIME						40
#define LCD_FIRST_ROW_INDEX					0
 
/**************************************************************************
 * Structure definition
 *
 * Description: Time structure definition
 *************************************************************************/
 
typedef union time_s
{
	struct
	{
		uint8_t hrs;
		uint8_t mins;
		uint8_t secs;
	};
	uint8_t array[3];
} time_s;
 
/**************************************************************************
 * Function: void initMain(void)
 *
 * Returns: Nothing
 *
 * Description: Contains initializations for main
 * 
 * Usage: initMain()
 *************************************************************************/
void initMain(void);

/**************************************************************************
 * Function: void updateClock(void)
 *
 * Returns: Nothing
 *
 * Description: Updates Clock Every Second
 * 
 * Usage: updateClock()
 *************************************************************************/
void updateClock(void);

/**************************************************************************
 * Function: void timeModify(void)
 *
 * Returns: Nothing
 *
 * Description: Modify time by user interaction with buttons
 * 
 * Usage: timeModify()
 *************************************************************************/
void timeModify(void);
 