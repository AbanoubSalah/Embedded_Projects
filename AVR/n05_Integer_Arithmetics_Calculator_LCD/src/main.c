/**************************************************************************
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: 8-bit unsigned integer calculator
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: Using LCD & keypad to perform integer calculation
 * 
 * Hardware Description: LCD in 4-bit mode connected to portA
 *                       KeyPad connected on portB
 *                      
 * Created Oct 25, 2021
 *************************************************************************/

/**************************************************************************
 * Includes and defines *************************************************************************/

#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>
#include "lcd.h"
#include "calculator.h"

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
	uint8_t num_map[] = { 7, 8, 9, 0, 4, 5, 6, 0, 1, 2, 3, 0, 0, 0 };
	uint8_t operation;
	uint8_t pressed_key, idx = 0;
	uint8_t num_fg = 1, output[COL_PER_ROW] = {'\0'};
	uint8_t num1 = 0, num2 = 0;
	/* Result is 16-bit */
	uint16_t res;

	/* Fill output string with nul character */
	for(idx = 0; idx < COL_PER_ROW; idx++)
	{
		output[idx] = '\0';
	}
	idx = 0;

	/* Peripherals initiations */
	initMain();
	LCD_Init();
	
	/* Welcome message */
	LCD_Write("CALCULATOR V0.1");
	_delay_ms(200);
	LCD_Clear();
	LCD_Command(0x0F);
	
	while(1)
	{
		pressed_key = get_keypad();
		if(pressed_key == 0xFF){ continue; }
		if((idx == (COL_PER_ROW - 1)) && (pressed_key != ON_OFF)){ continue; }

		/* Switch over pressed key */
		switch(pressed_key)
		{
			case ON_OFF:
				/* Clear LCD and reset different variables */
				for(idx = 0; idx < COL_PER_ROW; idx++)
				{
					output[idx] = '\0';
				}

				idx = 0;
				num_fg = 1;
				num1 = 0;
				num2 = 0;
				operation = ' ';
				res = 0;
				
				LCD_Clear();
				break;
			case EQUAL:
				/* Perform different arithmetic operations */
				if((operation == '/') && (num2 != 0)){ res = num1 / num2; }
				else if(operation == 'x'){ res = num1 * num2; }
				else if(operation == '+'){ res = num1 + num2; }
				else if((operation == '-') & (num1 > num2)){ res = num1 - num2; }
				else { idx = COL_PER_ROW - 1; break; }
				
				output[idx++] = ' ';
				output[idx++] = '=';
				output[idx++] = ' ';

				/* Get current output string index */
				uint8_t tmpIdx = idx;

				do
				{
					pressed_key = (res % 10);
					res /= 10;
					output[idx++] = pressed_key + '0';
					LCD_Write(&output[idx - 1]);
				}while(res != 0);
				
				/* reverse result number since, it was reverse inserted */
				while(idx > tmpIdx)
				{
					pressed_key = output[tmpIdx];
					output[tmpIdx++] = output[--idx];
					output[idx] = pressed_key;
				}
				
				idx = COL_PER_ROW - 1;
				break;
			case DIVIDE:
				/* set operation as division and lower first number flag */
				if(num_fg == 0){ break; }
				operation = '/';
				output[idx++] = '/';
				num_fg = 0;
				break;
			case MULTIPLY:
				/* set operation as multiplication and lower first number flag */
				if(num_fg == 0){ break; }
				operation = 'x';
				output[idx++] = 'x';
				num_fg = 0;
				break;
			case PLUS:
				/* set operation as addition and lower first number flag */
				if(num_fg == 0){ break; }
				operation = '+';
				output[idx++] = '+';
				num_fg = 0;
				break;
			case MINUS:
				/* set operation as subtraction and lower first number flag */
				if(num_fg == 0){ break; }
				operation = '-';
				output[idx++] = '-';
				num_fg = 0;
				break;
			default:
				/* Numbers handling */
				if((num2 >= 26) || ((num2 == 25) && (num_map[pressed_key] > 5)))
				{
					/* Handle number exceeding 255 (8-bits) */
					break;
				}
				else if(num_fg == 0)
				{
					/* Increase second number */
					num2 = (num2*10) + num_map[pressed_key];
				}
				else if((num1 >= 26) || ((num1 == 25) && (num_map[pressed_key] > 5)))
				{
					/* Handle number exceeding 255 (8-bits) */
					break;
				}
				else
				{
					/* Increase first number */
					num1 = (num1*10) + num_map[pressed_key];
				}
				
				/* Handle zero inserted muliple times */
				if(((idx == 1) && (num_fg == 1) && (num1 >= 0) && (num1 < 10) && (output[idx - 1] == '0')) || ((num_fg == 0) && (num2 >= 0) && (num2 < 10) && (output[idx - 1] == '0')))
				{
					--idx;
				}
				else
				{
				}
				output[idx++] = num_map[pressed_key] + '0';
				break;
		}
		
		/* Move to the beggining of LCD and write output */
		LCD_XY(0, 0);
		LCD_Write(output);
	}
	return 0;
}
