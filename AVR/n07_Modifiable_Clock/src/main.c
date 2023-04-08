/**************************************************************************
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: 7-Segment with interrupt
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *
 * Program Description: LCD clock that can be modified
 * 
 * Hardware Description: LCD with 4-bit mode on portA
 *                       3-pins from portB connected with three push-butons
 *                      
 * Created Oct 25, 2021
 *************************************************************************/

/**************************************************************************
 * Includes and defines
 *************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>
#include "lcd.h"
#include "modclk.h"

FUSES = 
{
    .low = (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3),
    .high = HFUSE_DEFAULT,
    .extended = EFUSE_DEFAULT,
};

/**************************************************************************
 * Function: Main
 *
 * Returns: Nothing
 *
 * Description: Program entry point
 *************************************************************************/

int main()
{
	LCD_Init();
	initMain();

	while(1)
	{
		if((SW_PIN & (1 << SEL_SET)) != 0)
		{
			/* Software switch de-Bounce */
			_delay_ms(DEBOUNCE_TIME);
			if((SW_PIN & (1 << SEL_SET)) != 0)
			{
				timeModify();
			}
			else
			{
			}
		}
		else
		{
		}
	}
	
	return 0;
}
