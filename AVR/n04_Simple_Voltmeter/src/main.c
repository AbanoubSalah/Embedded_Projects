/**************************************************************************
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: Simple voltmeter
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: Using ADC peripheral to sample analog signal
 *						and displaying it on the LCD
 * 
 * Hardware Description: Port A to connect to LCD
 *                       1-pin as analog i/p fo the ADC
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
#include <stdlib.h>
#include "lcd.h"
#include "simplevmeter.h"

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
	uint16_t vin, int_pt, frc_pt;
	uint8_t op[5], tmp, idx, cnt;

	initMain();
	LCD_Init();
	
	LCD_Write("Volt [mV] = ");

	while(1)
	{		
		vin = ReadADC(0);
		
		/* getting most significant numbers without losing accuracy */
		/* Reference voltage is 5000 mV so multiplying by (5000/1024) */
		/* Note: multiplying incremently to not saturate the 16-bit */
		/* register each step won't saturate it */
		int_pt = (25 * vin);
		int_pt >>= 1;
		int_pt *= 5;
		int_pt >>= 3;
		int_pt *= 5;
		int_pt >>= 3;
		
		idx = 0;
		if(0 == int_pt)
		{
			/* 0 result case */
			op[idx++] = ('0');
		}
		else
		{
			/* Converting number to an array of ASCII characters */
			while(0 != int_pt)
			{
				tmp = int_pt % 10;
				int_pt /= 10;
				op[idx++] = (tmp + '0');
			}
		}
		
		/* Flipping ASCII numbers array to be displayed correctly */
		cnt = idx;
		for(; idx != cnt/2; idx--)
		{
			tmp = op[idx-1];
			op[idx-1] = op[cnt-idx];
			op[cnt-idx] = tmp;
		}

		op[cnt] = '\0';
		LCD_XY(12, 0);
		LCD_Write("    ");
		LCD_XY(12, 0);
		LCD_Write(op);
		_delay_ms(500);
	}
	
	return 0;
}
