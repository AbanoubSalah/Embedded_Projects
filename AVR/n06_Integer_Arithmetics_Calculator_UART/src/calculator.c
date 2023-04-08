/**************************************************************************
 * File: calculator.c
 * Author: Abanoub Salah
 * AtMega169p: OSC @ 16MHz, 5v
 * Program: Source file for calculator program
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: Provides functions to interface with UART
 *                      
 * Created Oct 25, 2021
 *************************************************************************/


#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "calculator.h"

/**************************************************************************
 * Globals definitions
 *************************************************************************/

void initUART(uint16_t ubrr)
{
	/* Set baud rate */
	UBRR0H = (uint8_t)(ubrr>>8);
	UBRR0L = (uint8_t)ubrr;
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	// Set frame format: 8data
	UCSR0C = (1<<UCSZ01|1<<UCSZ00);
}

void UART_tx( uint8_t *data )
{
	/* Wait for empty transmit buffer, Then put data into buffer */
	uint8_t *ptr = data;
	while(*ptr != '\0')
	{
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = *ptr++;
	}
}

uint8_t UART_rx(void)
{
	/* Wait for data to be received */
	/* Then get and return received data from buffer */
	while (!(UCSR0A & (1<<RXC0)));
	return UDR0;
}
