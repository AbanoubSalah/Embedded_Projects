/**************************************************************************
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: 7-Segment with interrupt
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: using UART peripheral to communicate with a simple
 * 						unsigned integer arithmetic calculator
 * 
 * Hardware Description: using 2 pins of rx and tx of the UART
 *                      
 * Created Oct 25, 2021
 *************************************************************************/

/**************************************************************************
 * Includes and defines
 *************************************************************************/

#include <avr/io.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>
#include "calculator.h"

#define BAUD 9600
#define MYUBRR ((F_CPU/16/BAUD)-1)

FUSES = 
{
    .low = (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3),
    .high = HFUSE_DEFAULT,
    .extended = EFUSE_DEFAULT,
};

inline void newline()
{
	/* Send carriage-return */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = 0x0D;
	/* Send line-feed */
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = 0x0A;
}

/**************************************************************************
 * Function: Main
 *
 * Returns: Nothing
 *
 * Description: Program entry point
 *************************************************************************/

int main()
{
	uint8_t operation, output[6];
	uint8_t idx;
	uint8_t num1, num2, cur_char;
	uint16_t res;
	uint8_t prog_msg[] = "Calculator Program";
	uint8_t num1_msg[] = "Enter Number 1: ";
	uint8_t num2_msg[] = "Enter Number 2: ";
	uint8_t operation_msg[] = "Enter Operation: ";
	uint8_t res_msg[] = "Result: ";

	initUART(MYUBRR);
	
	UART_tx(prog_msg);

	while(1)
	{
		num1 = 0;
		num2 = 0;
		newline();
		UART_tx(num1_msg);
		
		/* Get first number */
		while(1)
		{
			cur_char = UART_rx();
			if(cur_char == ENTER){ break; }
			/* Echo-back character */
			UART_tx(&cur_char);
			if(cur_char >= '0' && cur_char <= '9')
			{
				cur_char = cur_char - '0';
			}
			else
			{
				break;
			}
			
			num1 = 10*num1 + cur_char;
		}
		newline();
		
		UART_tx(num2_msg);
		
		/* Get second number */
		while(1)
		{
			cur_char = UART_rx();
			if(cur_char == ENTER){ break; }
			UART_tx(&cur_char);
			if(cur_char >= '0' && cur_char <= '9')
			{
				cur_char = cur_char - '0';
			}
			else
			{
				break;
			}
			
			num2 = 10*num2 + cur_char;
		}
		newline();
		
		/* Get the operation */
		UART_tx(operation_msg);
		while(1)
		{
			cur_char = UART_rx();
			if(cur_char == ENTER)
			{
				break;
			}
			
			UART_tx(&cur_char);
			operation = cur_char;
		}
		
		newline();
		
		/* Perform arithmetic operation wrong message otherwise */
		switch(operation)
		{
			case '+':
				res = num1 + num2;
				break;
			case '-':
				res = num1 - num2;
				break;
			case '*':
				res = num1 * num2;
				break;
			case '/':
				res = num1 / num2;
				break;
			default:
			UART_tx("Wrong operation");
			newline();
		}
		
		idx = 0;
		if(res == 0)
		{
			/* Handle zero result */
			output[idx++] = '0';
			output[idx++] = '\0';
		}
		else
		{
			while(res != 0)
			{
				output[idx++] = ((res % 10) + '0');
				res /= 10;
			}
			
			/* Reverse result string since it was reverse inserted */
			uint8_t tmpIdx = 0;
			uint8_t tmp;
			
			output[idx] = '\0';
			while(idx > tmpIdx)
			{
				tmp = output[--idx];
				output[idx] = output[tmpIdx];
				output[tmpIdx++] = tmp;
			}
		}
		
		/* Send result */
		UART_tx(res_msg);
		UART_tx(output);
		newline();
	}
	return 0;
}
