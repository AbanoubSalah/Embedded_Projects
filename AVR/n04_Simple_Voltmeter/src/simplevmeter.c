/**************************************************************************
 * File: simplevmeter.c
 * Author: Abanoub Salah
 * AtMega169p 16F1717: w/X OSC @ 16MHz, 5v
 * Program: ADC driver
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *           
 *                  
 * Program Description: This program source will provide utility functions
 * 						for main
 *
 * Created on Oct 25, 2021
 *************************************************************************/
 
#include <avr/io.h>
#include <stdint.h>
#include "simplevmeter.h"

/**************************************************************************
 * Globals definitions *************************************************************************/

void initMain()
{
	ADC_DIDR |= (1 << ADC_DIDR_SEL);
	ADMUX |= (1 << REFS0);
	
	/* For 16MHz 128 prescaler puts ADC  within accuracy range */
	/* which is 50-200 KHz*/
	ADCSRA |= ((1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));
	
	return ;
}

uint16_t ReadADC(uint8_t ch)
{
	uint8_t k;
	uint16_t adcValue = 0;
	
	ADMUX &= ~(0x1F);
	ADMUX |= (ch);

	/* Neglect first reading after changing channel */
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	adcValue = ADCL;
	adcValue = ADCH;
	
	/* Take next one */
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	adcValue = 0;
	adcValue = ADCL;
	adcValue |= (ADCH << 8);
	
	return adcValue;
}
