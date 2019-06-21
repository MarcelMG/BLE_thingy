/*	UART on ATtiny3216
 *	by Marcel Meyer Garcia
 *	see LICENSE.txt
*/

#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdio.h>

#define UART_BAUD 115200
#ifndef F_CPU
	#define F_CPU 3333333
#endif
// baud rate calculation according to Microchips's Application Note TB3216 "Getting Started with USART"
#define UART_BAUD_VAL ((float)(F_CPU * 64 / (16 * (float)UART_BAUD)) + 0.5)

FILE UART_FSTREAM;

int UART_putc(char c, FILE *stream);
void init_UART(void);
void UART_print(char c);
void UART_print_str(char *str);
char UART_get(void);

#endif
