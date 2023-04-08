/**************************************************************************
 * File: calculator.c
 * Author: Abanoub Salah
 * AtMega169p: w/X OSC @ 16MHz, 5v
 * Program: Library file for calculator program 
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *
 * Program Description: This program source will provide utility functions
						for main
 *
 * Created on Oct 25, 2021
 *************************************************************************/

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "calculator.h"

/**************************************************************************
 * Globals definitions *************************************************************************/

uint8_t kp_map[] = { '7', '8', '9', '/',
					 '4', '5', '6', 'x',
					 '1', '2', '3', '-',
					 'c', '0', '=', '+'};

void initMain()
{
	/* Lowe nibble as o/p and the other i/p */
	KP_DDR = LNIB_MASK;
	
	/* Put logic high on lower nibble */
	/* make high nibble bits as pull-up */
	KP_PORT = 0xFF;
}

uint8_t get_keypad()
{
	int8_t idx;
	uint8_t key = 0xFF;
	
	for(idx = 0; idx <= 3; idx++)
	{
		/* check for pressed key by putting logic low */
		/* on bit by bit and check for i/p */
		KP_PORT = 0xFF;
		KP_PORT &= ~(1<<idx);
		if((KP_PIN & UNIB_MASK) != UNIB_MASK)
		{
			/* 4*idx */
			key = (idx << 2);
			break;
		}
	}
	
	/* Nothing is pressed */
	if(key == 0xFF){ return key; }
	
	/* Wait for de-bouncing */
	_delay_ms(20);
	
	for(idx = 3; idx >= 0; idx--)
	{
		if((KP_PIN & (1 << (idx + 4))) == 0)
		{
			key += idx;
			break;
		}
	}

	while((KP_PIN & UNIB_MASK) != UNIB_MASK);
	_delay_ms(20);
	
	return key;
}
