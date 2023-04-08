/**************************************************************************
 * File: lcd.c
 * Author: Abanoub Salah
 * AtMega169p: w/X OSC @ 16MHz, 5v
 * Program: LCD driver
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *
 * Program Description: This program will allow interfacing of LCD
 *
 * Created on Oct 25, 2021
 *************************************************************************/

#include <lcd.h>
#ifndef F_CPU
#define F_CPU 16000000UL
#endif
#include <util/delay.h>

void LCD_Init()
{
	/* Make LCD port direction as o/p */
	LCD_DDR = 0xFF;
	LCD_PORT &= ~(1 << LCD_RS);
	LCD_PORT |= ( 1 << LCD_EN);
	
	LCD_Command(0x02);
	
	_delay_ms(10);
	LCD_PORT &= ~(1 << LCD_EN);
	
	LCD_PORT |= (1 << LCD_EN);
	LCD_Command(0x28);
	_delay_ms(10);
	LCD_PORT &= ~(1 << LCD_EN);
	
	LCD_PORT |= (1 << LCD_EN);
	LCD_Command(0x06);
	_delay_ms(10);
	LCD_PORT &= ~(1 << LCD_EN);
	
	LCD_PORT |= (1 << LCD_EN);
	LCD_Command(0x80);
	_delay_ms(10);
	LCD_PORT &= ~(1 << LCD_EN);
	
	LCD_PORT |= (1 << LCD_EN);
	LCD_Command(0x0C);
	_delay_ms(10);
	LCD_PORT &= ~(1 << LCD_EN);

	return ;
}

void LCD_Command(unsigned char cmd)
{
	LCD_PORT = (LCD_PORT & LNIPMASK) | (cmd & UNIPMASK);
    LCD_PORT &= ~(1<<LCD_RS);
    
	LCD_PORT |= (1<<LCD_EN);
    _delay_ms(1);
    LCD_PORT &= ~ (1<<LCD_EN);            
    _delay_ms(1);
	
    LCD_PORT = (LCD_PORT & LNIPMASK) | (cmd << 4);
	LCD_PORT |= (1<<LCD_EN);
    _delay_ms(1);
    LCD_PORT &= ~ (1<<LCD_EN);                                                
	_delay_ms(1);
}

void LCD_XY(uint8_t x, uint8_t y)
{
	if(0 == y)
	{
		LCD_Command(x + 0x80);
	}
	else if(1 == y)
	{
		LCD_Command(x + 0xC0);
	}
	return ;
}

void LCD_Write(unsigned char *str)
{
	for(uint8_t i = 0; str[i] != '\0'; i++)
	{
		LCD_PORT = (LCD_PORT & 0x0F) | (str[i] & 0xF0); 
		LCD_PORT |= (1 << LCD_RS);                                       
		LCD_PORT|= (1 << LCD_EN);
		_delay_us(1);
		LCD_PORT &= ~ (1 << LCD_EN);
		_delay_us(200);
		LCD_PORT = (LCD_PORT & 0x0F) | (str[i] << 4);   
		LCD_PORT |= (1 << LCD_EN);
		_delay_us(1);
		LCD_PORT &= ~ (1 << LCD_EN);
		_delay_ms(2);
	}
}

void LCD_Clear()
{
    LCD_Command(0x01);                            
}
