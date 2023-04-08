/*
 * File: util.h
 * Author: Abanoub Salah
 * AtMega169p 16F1717: w/X OSC @ 16MHz, 5v
 * Program: Header file of main functions
 * Compiler: avr-gcc 8.4.0
 * Program Version 1.0
 *           
 *                  
 * Program Description: This program header will provide main functions
 *
 * Created on Oct 15th, 2021
 */


#define ENTER (13)

/**************************************************************************
 * Function: void initUART(uint16_t)
 *
 * Returns: Nothing
 *
 * Description: Initializes the UART
 * 
 * Usage: initUART(ubrr)
 *************************************************************************/
void initUART(ubrr);

/**************************************************************************
 * Function: void UART_tx(uint8_t *)
 *
 * Returns: Nothing
 *
 * Description: Sends a string of characters over the UART
 * 
 * Usage: UART_tx(pointer_to_string)
 *************************************************************************/
void UART_tx(uint8_t *);

/**************************************************************************
 * Function: uint8_t UART_rx(void)
 *
 * Returns: Received data
 *
 * Description: Receive data over the UART
 * 
 * Usage: UART_rx()
 *************************************************************************/
uint8_t UART_rx(void);
