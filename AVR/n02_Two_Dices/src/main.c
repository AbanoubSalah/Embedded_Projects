/*
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: Dices
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: Using a button to roll two 6-sided dices using a *						total of 9 pins
 * 
 * Hardware Description: 1-Button connected to port B pin-0
 *                       2-LEDs connected to port A using 8-pins
 *
 *                      
 * Created Oct 25, 2021
 *************************************************************************/


/**************************************************************************
 *Includes and defines
 *************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>

#define SW_PIN				PINB
#define SW_NUM				PIN0
#define DICE_PORT			PORTA
#define DICE_DDR			DDRA
#define NUM_ROLL			3
#define MAX_DOTS			6
#define DEBOUNCE_TIME		40
#define USER_WAIT_TIME		100

FUSES = 
{
    .low = (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3),
    .high = HFUSE_DEFAULT,
    .extended = EFUSE_DEFAULT,
};

void numberReroll(uint8_t *);
uint8_t checkValidity(uint8_t num);

/**************************************************************************
 * Function: Main
 *
 * Returns: Nothing
 *
 * Description: Program entry point
 *************************************************************************/
int main()
{
	/* Random starting number other than zero for the algorithm */
	uint8_t curRoll = 0xFE;
	
	/* Dices LEDs pattern */
	uint8_t pattern[MAX_DOTS + 1] = {0x00, 0x08, 0x01, 0x09, 0x06, 0x0E, 0x07};
	
	/* Set portA as o/p */
	DICE_DDR = 0xFF;
	DICE_PORT = 0x00;
	
	while(1)
	{
		if((SW_PIN & (1 << SW_NUM)))
		{
			/* Software switch press debouncing */
			_delay_ms(DEBOUNCE_TIME);			
			if((SW_PIN & (1 << SW_NUM)))
			{
				/* Simulate rolling by animating LEDs NUM_ROLL times */
				for(uint8_t i = 0; i < NUM_ROLL; i++)
				{
					numberReroll(&curRoll);
					while(0 == checkValidity(curRoll))
					{
						numberReroll(&curRoll);
					}
					
					/* Put pattern on the port */
					DICE_PORT = ((pattern[curRoll % 10]) | (pattern[curRoll / 10] << 4));
					_delay_ms(USER_WAIT_TIME);
				}
				
				numberReroll(&curRoll);
				while(0 == checkValidity(curRoll))
				{
					numberReroll(&curRoll);
				}
				
				DICE_PORT = ((pattern[curRoll % 10]) | (pattern[curRoll / 10] << 4));
			}
		}
	}
	
	return 0;
}

/**************************************************************************
 * Function: numberReroll
 *
 * Returns: Nothing
 *
 * Description: Generate a random number using LFSR algorithm
 *************************************************************************/
void numberReroll(uint8_t *curNum)
{
	/* Generate a random number using LFSR algorithm */
	uint8_t bit;
	
	bit = (*curNum ^ (*curNum >> 3) ^ (*curNum >> 4) ^ (*curNum >> 5) ^ (*curNum >> 5)) & 1;
	*curNum = (*curNum >> 1) | (bit << 7);
	
	return ;
}

/**************************************************************************
 * Function: checkValidity
 *
 * Returns: Nothing
 *
 * Description: Checks if the ones or tens out of range of 1..6 inclusive
 *				returns 1 if valid 0 otherwise
 *************************************************************************/
uint8_t checkValidity(uint8_t num)
{
	uint8_t isValid = 1;
	uint8_t ones = (num % 10);
	uint8_t tens = (num / 10);
	
	if((MAX_DOTS < ones) || (MAX_DOTS < tens) || (0 == ones) || (0 == tens))
	{
		isValid = 0;
	}
	
	return isValid;
}
