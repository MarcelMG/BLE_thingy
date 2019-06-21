/*	UART on ATtiny3216
 *	by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include "uart.h"

FILE UART_FSTREAM = FDEV_SETUP_STREAM(UART_putc, NULL, _FDEV_SETUP_WRITE);

int UART_putc(char c, FILE *stream){
	UART_print(c);
	return 0;
}

// init USART0 with default config (async, 8 data-bit, 1 stop-bit, no parity-bit)
void init_UART(){
	PORTB.DIRSET = PIN2_bm;	// TX pin PB2 as output
	PORTB.DIRCLR = PIN3_bm;	// RX pin PB3 as input
	USART0.BAUD = (uint16_t)UART_BAUD_VAL;	// set baud rate (calculated by macro)
	USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;	// enable RX and TX
	stdout = &UART_FSTREAM; // redirect stdout to UART to use printf()
}
// transmit one character (polling)
void UART_print(char c){
	while( !(USART0.STATUS & USART_DREIF_bm) );	// wait until data transmit buffer is empty, i.e. ready to send new data
	USART0.TXDATAL = c;
}
// transmit a C-string
void UART_print_str(char *str){
	while(*str){	//	until '\0' terminator is reached
		UART_print(*str);
		++str;
	}
}
// receive a character (polling)
char UART_get(void){
	while ( !(USART0.STATUS & USART_RXCIF_bm) );
	return USART0.RXDATAL;
}