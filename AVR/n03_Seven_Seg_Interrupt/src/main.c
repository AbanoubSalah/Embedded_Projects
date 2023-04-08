/**************************************************************************
 * File: main.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: 7-Segment with interrupt
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *
 * Program Description: Using two multiplexed 7-segments to count
 *						from 0 to 99
 * 
 * Hardware Description: 2 7-segments connected to port A pin-0..6
 *                       2-pins as enable lines connected to B0 and B1
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

FUSES = 
{
    .low = (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3),
    .high = HFUSE_DEFAULT,
    .extended = EFUSE_DEFAULT,
};

#define DIGIT_PORT					PORTA
#define DIGIT_EN_PORT				PORTB
#define DIGIT1_PORT_VALUE			(1 << 0)
#define DIGIT2_PORT_VALUE			(1 << 1)
#define SECOND_TIME_DELAY			1000

uint8_t Seg_Pattern[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
uint8_t dig_fg = 0;
uint8_t msd = 0, lsd = 0;

/*******************************************************************************
 * Function: Main
 *
 * Returns: Nothing
 *
 * Description: Program entry point
 ******************************************************************************/

int main()
{
	uint8_t cnt = 0;
	
	/*  */
	DDRA = 0xFF;
	DDRB = 0x03;
	
	/* Set-up Timer1 in CTC mode */
	/* with prescaler of 1024 */
	TCCR1B |= (1<<WGM12 | 1<<CS10) | (1<<CS12);
	
	/* Initialize counter */
    TCNT1 = 0;
	
	/* 5 ms interrupt */
	OCR1A = 0x004E;
	TIMSK1 |= (1 << OCIE1A);
	
	/* Enable global interrupts */
    sei();
	
	while(1)
	{		
		/* Get ones and tens digits */
		msd = (cnt / 10);
		lsd = (cnt % 10);
		
		/* Delay around a second */
		_delay_ms(SECOND_TIME_DELAY);
		
		cnt++;
		
		/* Count until 99 */
		if(100 == cnt)
		{
			cnt = 0;
		}
		else
		{
		}
	}
	return 0;
}

ISR(TIMER1_COMPA_vect)
{
	/* Alternating between the two 7-segments */
	if(dig_fg == 0)
	{
		DIGIT_EN_PORT = DIGIT1_PORT_VALUE;
		DIGIT_PORT = Seg_Pattern[msd];
	}else{
		DIGIT_EN_PORT = DIGIT2_PORT_VALUE;
		DIGIT_PORT = Seg_Pattern[lsd];
	}
	
	dig_fg ^= 1;
}
