/**************************************************************************
 * File: Main.c
 * Author: Abanoub Salah
 * AtMega169p 16F1717: w/X OSC @ 16MHz, 5v
 * Program: Modifiable Clock
 * Compiler: avr-gcc 8.4.0
 * Program Version: 1.0
 *                
 *                
 * Program Description: Using three potentiometers to control RGB LED
 * 
 * Hardware Description: 1 RGB LED connected to three pins
 *						 3 potentiometers connected to three ADC channels
 *
 * Created Oct 25, 2021
 */


/**************************************************************************
 *Includes and defines
 *************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/power.h>

#define RED_PIN				PA0
#define GREEN_PIN			PA1
#define BLUE_PIN			PA2
#define LED_PORT			PORTA
#define LED_DDR				DDRA


#define RED_ADC_PIN			PF0
#define GREEN_ADC_PIN		PF1
#define BLUE_ADC_PIN		PF2
#define ADC_DDR				DDRF

#define RED_ADC_CHD			(ADC0D)
#define GREEN_ADC_CHD		(ADC1D)
#define BLUE_ADC_CHD		(ADC2D)
#define RED_ADC_CHM			(MUX0)
#define GREEN_ADC_CHM		(MUX1)
#define BLUE_ADC_CHM		(MUX2)

FUSES = 
{
    .low = (FUSE_CKSEL0 & FUSE_CKSEL1 & FUSE_CKSEL2 & FUSE_CKSEL3),
    .high = HFUSE_DEFAULT,
    .extended = EFUSE_DEFAULT,
};

static uint8_t pwm[3] = {255, 255, 255}, scale = 0;

void initGPIO(void);
void initADC(void);
void initTimer(void);
uint8_t ReadADC(uint8_t);

void main()
{
	initGPIO();
	initADC();
	initTimer();
	
	sei();
	
    while(1)
    {
	  pwm[0] = ReadADC(RED_ADC_CHM);
	  pwm[1] = ReadADC(GREEN_ADC_CHM);
	  pwm[2] = ReadADC(BLUE_ADC_CHM);
    }
}

void initGPIO(void)
{
	LED_DDR |= ((1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN));
	LED_PORT |= ((1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN));
	
	ADC_DDR &= ~((1 << RED_ADC_PIN) | (1 << GREEN_ADC_PIN) | (1 << BLUE_ADC_PIN));
	
	return ;
}

void initTimer(void)
{	
	/* Normal mode and no prescaler */
	TCCR0A |= (1 << CS00);
	
	/* zero-out the time counter */
	TCNT0 = 0x00;
	
	/* Overflow inturrupt enable */
	TIMSK0 |= (1 << TOIE0);
	
	return ;
}

void initADC(void)
{
	/* Activate 3-ADC channels as configured */
	DIDR1 |= ((1 << BLUE_ADC_CHD) | (1 << GREEN_ADC_CHD) | (1 << RED_ADC_CHD));
	/* AVCC with external capacitor at AREF pin */
	/* and left adjusted result */
	ADMUX |= ((1 << REFS0) | (1 << ADLAR));
	/* ADC enable and 128 prescaler for 125 KHz sampling */
	/* @ 16 KHz CPU clock */
	ADCSRA |= ((1 << ADEN) | (1 << ADPS1) | (1 << ADPS1) | (1 << ADPS0));
	
	return ;
}

uint8_t ReadADC(uint8_t ch)
{
	uint8_t k;
	uint16_t adcvalue = 0;
	
	/* clear channel select bits */
	ADMUX = ch;
	ADMUX |= ((1 << REFS0) | (1 << ADLAR));
	/* Neglect first reading after changing channel */
	ADCSRA |= (1 << ADSC);
	while((ADCSRA & (1 << ADSC)) != 0);
	adcvalue = ADCH;
	adcvalue = 0;
	
	/* Summing 8 readings */
	for(k = 0; k < 8; k++)
	{
		ADCSRA |= (1 << ADSC);
		
		while(ADCSRA&(1<<ADSC));
		
		adcvalue += ADCH;
	}
	
	PORTA ^= (1 << PA7);
	
	/* return average of 8 readings */
	return (adcvalue >> 3);
}

ISR(TIMER0_OVF_vect)
{
	/* Value of (scale) decides the no of levels of PWM */
	/* scale has 256 levels (0..255) */
	/* Where 0: completely on, 255: completely off */
	if(scale == 255)
	{
		scale = 0;
		LED_PORT |= ((1 << RED_PIN) | (1 << GREEN_PIN) | (1 << BLUE_PIN));
	}
	
	if(pwm[0] == scale){ LED_PORT &= ~(1 << RED_PIN); }
	else{ }
	if(pwm[1] == scale){ LED_PORT &= ~(1 << GREEN_PIN); }
	else{ }
	if(pwm[2] == scale){ LED_PORT &= ~(1 << BLUE_PIN); }
	else{ }
	
	scale++;
}
