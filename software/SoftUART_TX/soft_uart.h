#ifndef SOFT_UART_H
#define SOFT_UART_H

/*	Simple Software UART TX for AVR with printf support
 *	uses neither interrupts nor timers (blocking)
 *	if you use interrupts, disable them while transmitting
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include <avr/io.h>
#define F_CPU 3333333
#include <util/delay.h>
#include <stdio.h>

// use pin PB1 for UART TX, change these 3 defines to use another pin
#define UART_TX_LOW() PORTB.OUTCLR=PIN1_bm
#define UART_TX_HIGH() PORTB.OUTSET=PIN1_bm
#define UART_TX_SETOUTPUT() PORTB.DIRSET=PIN1_bm

#define BAUD 9600
#define ERROR_COMP 0.0 // for clock freq. deviation error compensation
#define BAUD_DELAY_US 1000000UL*(1+ERROR_COMP)/BAUD


FILE USART_stream;

void soft_uart_setup(void);
int dbg_print_char(char c, FILE* stream);


#endif