/*	Simple Software UART TX for AVR with printf support
 *	uses neither interrupts nor timers (blocking)
 *	if you use interrupts, disable them while transmitting
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/

#include <avr/io.h>
#include "soft_uart.h"

int main(void){
    soft_uart_setup();
	
    while(1){
		printf("hello world!\n");
		_delay_ms(1000);
    }
	
	
	return 0;
}

