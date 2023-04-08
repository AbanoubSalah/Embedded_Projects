/**************************************************************************
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: Cylon eyes LEDs
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: Using LEDs to Demonstrate MCU I/O
 * 
 * Hardware Description: 8 LEDs connected to port A
 *
 *                      
 * Created Oct 25, 2021
 *************************************************************************/

/**************************************************************************
 *Includes and defines *************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>
#include <stdint.h>

#define DELAY_TIME			50
#define LEDS_DDR			DDRA
#define LEDS_PORT			PORTA

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
	/* Configure port as output */
	LEDS_DDR = 0xFF;
	
	LEDS_PORT = 0x01;
	_delay_ms(DELAY_TIME);

	while(1)
	{		
		/* Shifting left */
		while(LEDS_PORT < 0x80)
		{
			LEDS_PORT <<= 1;
			_delay_ms(DELAY_TIME);
		}
		
		/* Shifting right */
		while(LEDS_PORT > 0x01)
		{
			LEDS_PORT >>= 1;
			_delay_ms(DELAY_TIME);	
		}
	}
	
	return 0;
}

