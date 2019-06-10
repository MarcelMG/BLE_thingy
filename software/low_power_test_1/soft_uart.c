/*	Simple Software UART TX for AVR with printf support
 *	uses neither interrupts nor timers (blocking)
 *	if you use interrupts, disable them while transmitting
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include "soft_uart.h"

FILE USART_stream = FDEV_SETUP_STREAM(dbg_print_char, NULL, _FDEV_SETUP_WRITE);

void soft_uart_setup(){
	UART_TX_SETOUTPUT(); // set pin as output
	stdout = &USART_stream;	// redirect stdout so printf() can be used
}

// a simple software UART-TX
int dbg_print_char(char c, FILE* stream){
	UART_TX_LOW(); // START bit
	_delay_us(BAUD_DELAY_US);
	for(uint8_t k = 0; k != 8; ++k){
		if( c & 0x01 ) UART_TX_HIGH(); else UART_TX_LOW();
		c >>= 1;
		_delay_us(BAUD_DELAY_US);
	}
	UART_TX_HIGH(); // STOP bit
	_delay_us(BAUD_DELAY_US);
	return 0;
}