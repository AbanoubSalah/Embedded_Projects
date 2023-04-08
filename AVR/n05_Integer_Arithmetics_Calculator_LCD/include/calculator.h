/**************************************************************************
 * File: calculator.h
 * Author: Abanoub Salah
 * AtMega169p: w/X OSC @ 16MHz, 5v
 * Program: Header file of calculator
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *           
 *                  
 * Program Description: This header will provide interface for calculator
 *
 * Created on Oct 25, 2021
 */

#define KP_PORT PORTB
#define KP_DDR DDRB
#define KP_PIN PINB
#define UNIB_MASK (0xF0)
#define LNIB_MASK (0x0F)
#define MINUS (11)
#define PLUS (15)
#define MULTIPLY (7)
#define DIVIDE (3)
#define EQUAL (14)
#define ON_OFF (12)
#define COL_PER_ROW (16)

/**************************************************************************
 * Function: void initMain(void)
 *
 * Returns: Nothing
 *
 * Description: Contains initializations for main
 * 
 * Usage: initMain()
 *************************************************************************/
void initMain(void);

/**************************************************************************
 * Function: uint8_t get_keypad(void)
 *
 * Returns: Pressed key
 *
 * Description: Reads current pressed key on the keypad
 * 
 * Usage: get_keypad()
 *************************************************************************/
uint8_t get_keypad(void);
