/**************************************************************************
 * File: simplevmeter.h
 * Author: Abanoub Salah
 * AtMega169p: w/X OSC @ 16MHz, 5v
 * Program: Header file of simplevmeter
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *
 * Program Description: This header will provide the interface of 
 *						simplevmeter
 *
 * Created on Oct 25, 2021
 *************************************************************************/

#include <stdint.h>

#define ADC_PIN					PINF
#define ADC_PORT				PORTF
#define ADC_DDR					DDRF
#define ADC_DIDR				DIDR0
#define ADC_DIDR_SEL			ADC0D
 
/**************************************************************************
 * Function: void initMain(void)
 *
 * Returns: Nothing
 *
 * Description: Contains initializations for main
 * 
 * Usage: initMain() *************************************************************************/
void initMain(void);

/**************************************************************************
 * Function: uint16_t ReadADC(uint8_t)
 *
 * Returns: Sampled value
 *
 * Description: Read ADC channel and returns it's value
 * 
 * Usage: ReadADC(channel); *************************************************************************/
uint16_t ReadADC(uint8_t);
