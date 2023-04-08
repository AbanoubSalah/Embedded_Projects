/**************************************************************************
 * File: lcd.h
 * Author: Abanoub Salah
 * AtMega169p: w/X OSC @ 16MHz, 5v
 * Program: Header file to interface LCD
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *
 * Program Description: This header will allow interface of LCD
 *
 * Created on Oct 25, 2021
 *************************************************************************/


#ifndef LCD_HEADER_H
#define LCD_HEADER_H

/**************************************************************************
 * Includes and defines *************************************************************************/

#include <avr/io.h>

#define LCD_PIN PINA
#define LCD_PORT PORTA
#define LCD_DDR DDRA
#define LCD_RS PA0
#define LCD_EN PA1
#define LCD_D4 PA4
#define LCD_D5 PA5
#define LCD_D6 PA6
#define LCD_D7 PA7

#define UNIPMASK 0xF0
#define LNIPMASK 0x0F

/* Function declarations */
void LCD_Init(void);
void LCD_Command(uint8_t cmd);
void LCD_XY(uint8_t x, uint8_t y);
void LCD_Write(uint8_t *str);
void LCD_Clear(void);

#endif
