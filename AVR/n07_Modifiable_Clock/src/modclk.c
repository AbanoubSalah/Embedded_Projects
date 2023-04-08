/**************************************************************************
 * File: modclk.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: modclk source file
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *
 * Program Description: modclk source file provides program functions
 *                      
 * Created Oct 25, 2021
 *************************************************************************/

 
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "lcd.h"
#include "modclk.h"

/**************************************************************************
 * Definitions
 *************************************************************************/

static volatile time_s curTime = {0, 0, 0};
static const time_s maxTime = {24, 60, 60};
static uint8_t curPartPosition;
static volatile uint8_t isCurrentlyModified = 0;

static uint8_t correctTime(void);
static void updateClockPart(uint8_t);
static void blinkLCD(void);

void initMain(void)
{
	uint8_t idx;
	
	/* Write initial time on LCD */
	LCD_Write("  :  :  ");
	for(idx = 0; idx < CLOCK_PARTS_COUNT; idx++)
	{
		updateClockPart(idx);
	}
	
	/* Setup Timer1 in CTC mode	*/
	/* Set the pre-scalar as 1024 */
	TCCR1B |= ((1 << WGM12) | (1 << CS10) | (1 << CS12));
	/* initialize counter */
    TCNT1 = 0;
	/* One second interrupt */
	OCR1A = ONE_SECOND_OCR;
	//OCR1A = 0xF;
	/* Enable o/p compare interrupt */
	TIMSK1 |= (1 << OCIE1A);
	
	/* Enable global interrupts */
    sei();
	
	return ;
}

void updateClock(void)
{	
	uint8_t tens, ones;
	int8_t curIdx = CLOCK_CHARACTER_PER_PART * CLOCK_PARTS_COUNT;
	int8_t idx = CLOCK_PARTS_COUNT - 1;
	uint8_t result = correctTime();
	
	/* Check changed part and update LCD accordingly */
	while(result != 0)
	{
		curIdx -= CLOCK_CHARACTER_PER_PART;
		
		if((result & 1) == 1)
		{
			updateClockPart(idx);
		}
		else
		{
		}
		
		result >>= 1;
		--idx;
	}

	return ;
}

void timeModify()
{
	curPartPosition = 0;
	isCurrentlyModified = 1;
	
	cli();
	
	LCD_XY(curPartPosition * CLOCK_CHARACTER_PER_PART, LCD_FIRST_ROW_INDEX);
	
	/* Fifty ms interrupt for blinking LCD */
	OCR1A = 0x030D;
	TCNT1 = 0;
	
	sei();
	
	/* while SET button is not pressed CLOCK_PARTS_COUNT times */
	/* Disabling interrupts after every button pressed to avoid blinking */
	/* interrupt from using LCD during number alteration */
	/* or moving to the next clock part  */
	while(curPartPosition < CLOCK_PARTS_COUNT)
	{
		/* increment button pressed */
		if((SW_PIN & (1 << SEL_INC)) != 0)
		{
			_delay_ms(DEBOUNCE_TIME);
			if((SW_PIN & (1 << SEL_INC)) != 0)
			{
				cli();
				
				/* Increment current part and check number validity */
				curTime.array[curPartPosition]++;
				curTime.array[curPartPosition] %= maxTime.array[curPartPosition];
				updateClockPart(curPartPosition);
				
				sei();
			}
		}
	
		/* decrement button pressed */
		if((SW_PIN & (1 << SEL_DEC)) != 0)
		{
			_delay_ms(DEBOUNCE_TIME);
			if((SW_PIN & (1 << SEL_DEC)) != 0)
			{
				if(curTime.array[curPartPosition] == 0)
				{
					/* Part number is zero case */
					continue;
				}
				else
				{
					/* Decrement current part and check number validity */
					cli();
					curTime.array[curPartPosition]--;
					updateClockPart(curPartPosition);
					sei();
				}
			}
		}
		
		/* SET button pressed */
		if((SW_PIN & (1 << SEL_SET)) != 0)
		{
			_delay_ms(DEBOUNCE_TIME);
			if((SW_PIN & (1 << SEL_SET)) != 0)
			{
				cli();
				
				/* Move to the next clock part */
				curPartPosition += 1;
				updateClockPart(curPartPosition - 1);
				LCD_XY(curPartPosition * CLOCK_CHARACTER_PER_PART, LCD_FIRST_ROW_INDEX);
				
				sei();
			}
		}
	}
	
	/* Modification is finished reseting registers to their */
	/* original values and returning to main */
	cli();
	
	TCNT1 = 0;
	/* One second interrupt */
	OCR1A = ONE_SECOND_OCR;
	isCurrentlyModified = 0;
	
	sei();
	
	return ;
}

/**************************************************************************
 * Function: uint8_t CorrectTime(void)
 *
 * Returns: Masked bits of the modified clock parts
 *
 * Description: Correct time by checking correct seconds, minutes,
 * and hours numbers
 * 
 * Usage: CorrectTime()
 *************************************************************************/
static uint8_t correctTime(void)
{
	uint8_t tens, ones;
	uint8_t result = 0b001;
	int8_t idx;
	
	/* Check if clock parts passed their maximum values by comparing current to maximum */
	for(idx = (CLOCK_PARTS_COUNT - 1); idx >= 0; idx--)
	{
		if(curTime.array[idx] >= maxTime.array[idx])
		{
			if(idx != 0)
			{
				curTime.array[idx - 1] += (curTime.array[idx] / maxTime.array[idx]);
			}
			else
			{
			}
			
			curTime.array[idx] %= maxTime.array[idx];
			result |= (1 << (CLOCK_PARTS_COUNT - idx - 1));
		}
		else
		{
		}
	}
	
	return result;
}

/**************************************************************************
 * Function: static void updateClockPart(uint8_t partIndex)
 *
 * Returns: Nothing
 *
 * Description: Updates specified LCD Clock Part
 * 
 * Usage: updateClockPart(partIndex)
 *************************************************************************/
static void updateClockPart(uint8_t partIndex)
{
	uint8_t tens, ones;
	uint8_t tmpStr[CLOCK_CHARACTER_PER_PART] = {'0', '0', '\0'};
	uint8_t LCDIndex = CLOCK_CHARACTER_PER_PART * partIndex;
	
	/* Check if index within range */
	if(partIndex < CLOCK_PARTS_COUNT)
	{
		/* Get tens and ones parts of the number */
		tens = (curTime.array[partIndex] / 10);
		ones = (curTime.array[partIndex] % 10);
		tmpStr[0] = (tens + '0');
		tmpStr[1] = (ones + '0');
		
		/* Display number on LCD */
		LCD_XY(LCDIndex, LCD_FIRST_ROW_INDEX);
		LCD_Write(tmpStr);
		LCD_XY(LCDIndex, LCD_FIRST_ROW_INDEX);
	}
	else
	{
	}
	
	return ;
}

/**************************************************************************
 * Function: static void blinkLCD(void)
 *
 * Returns: Nothing
 *
 * Description: Called to blinks LCD periodically
 * 
 * Usage: blinkLCD()
 *************************************************************************/
static void blinkLCD(void)
{
	const uint8_t totalTimeSlices = 5;
	static uint8_t timeSlice = 0;
	
	/* Move cursor to part position */
	LCD_XY((curPartPosition * CLOCK_CHARACTER_PER_PART), LCD_FIRST_ROW_INDEX);
	
	/* Reset timeSlice counter to realize 1:(totalTimeSlices - 1) partion */
	if(timeSlice == totalTimeSlices)
	{
		timeSlice = 0;
	}
	else
	{
		timeSlice++;
	}
	
	/* On/Off time of the number are partioned 1:(totalTimeSlices - 1) */
	if(timeSlice >= totalTimeSlices)
	{
		LCD_Write("  ");
	}
	else
	{
		updateClockPart(curPartPosition);
	}
	
	return ;
}

ISR(TIMER1_COMPA_vect)
{
	/* Two states modifying clock or incrementing seconds */
	/* Controlled by one global variable */
	if(isCurrentlyModified == 1)
	{
		blinkLCD(); 
	}
	else
	{
		(curTime.secs)++;
		updateClock();
	}
}
