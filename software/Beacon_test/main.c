/* iBeacon example */

#include <avr/io.h>
#define F_CPU 3333333
#include <util/delay.h>
#include <string.h>
#include "uart.h"
#include <stdio.h>
int USART0_printChar(char c, FILE *stream){
	USART0_sendChar(c);
	return 0;
}
FILE USART_stream = FDEV_SETUP_STREAM(USART0_printChar, NULL, _FDEV_SETUP_WRITE);

// use pin PB1 for UART TX
#define UART_TX_LOW() PORTB.OUTCLR=PIN1_bm
#define UART_TX_HIGH() PORTB.OUTSET=PIN1_bm
#define BAUD 9600
#define ERROR_COMP 0.00 // for clock freq. deviation error compensation, e.g. 0.05 = 5%, just try until it works :)
#define BAUD_DELAY_US 1000000UL*(1+ERROR_COMP)/BAUD
void dbg_print_char(char c){
	c = ~c;
	UART_TX_LOW();            // start bit
	for( uint8_t i = 10; i; i-- ){        // 10 bits
		_delay_us(BAUD_DELAY_US);            // bit duration
		if( c & 1 ){
			UART_TX_LOW();        // data bit 0
			}else{
			UART_TX_HIGH();           // data bit 1 or stop bit
		}
		c >>= 1;
	}
}
void dbg_print_str(char* str){
	while(*str) dbg_print_char(*str++);
}

void RN4871_waitfor(char endchar){
	// get response, should be: "AOK"
	char str[10];
	char* str_ptr = str;
	*str_ptr = USART0_readChar();
	while( *str_ptr != endchar ){
		++str_ptr;
		*str_ptr = USART0_readChar();
	}
	++str_ptr;
	*str_ptr = '\0';
	dbg_print_str(str);	
}

char UUID[32] = "123456789ABCDEF123456789ABCDEF12";

void iBeacon_advertise(uint16_t minor, uint16_t major, uint8_t TxPower){
	printf("IB,Z\r");
	RN4871_waitfor('K');
	printf("IB,FF,");
	printf("4C000215");
	printf(UUID);
	printf("%.4X", minor);
	printf("%.4X", major);
	printf("%.2X", TxPower);
	putchar('\r');
	RN4871_waitfor('K');
}

int main(void)
{
	PORTB.DIRSET = PIN1_bm;	// TX-pin PB1 as output for debugging
	PORTC.DIRSET = PIN2_bm;	// pin PC2 (LED) as output
	PORTC.OUTCLR = PIN2_bm; // LED off

	PORTC.DIRSET = PIN0_bm; // PC0 - BLEwakeup as output
	PORTC.OUTCLR = PIN0_bm; // BLE uart active
	
	USART0_init(); // init UART to talk to RN4871
	stdout = &USART_stream; // redirect stdout to UART
	
	/* reset RN4871 */
	PORTC.OUTSET = PIN1_bm; // BLE RESET HIGH
	PORTC.DIRSET = PIN1_bm; // pin PC1 (BLE RESET) as output
	PORTC.OUTCLR = PIN1_bm;
	_delay_ms(10);
	PORTC.OUTSET = PIN1_bm;
	
	_delay_ms(100); // wait for RN4871 to boot up and be ready
	dbg_print_str("start\n");
	
	printf("$$$"); // enter command mode
	RN4871_waitfor('>'); // get response, should be: "CMD>"
	_delay_ms(1);
	printf("SS,10\r"); // set service to non-connectable beacon
	RN4871_waitfor('K'); // get response, should be: "AOK"
	_delay_ms(1);
	printf("STB,0640\r"); // set advertisement interval to 640*0.625ms = 1s
	RN4871_waitfor('K'); // get response, should be: "AOK"
	_delay_ms(1);
	printf("SO,1\r"); // enable low power mode
	RN4871_waitfor('K'); // get response, should be: "AOK"
	
	PORTC.OUTSET = PIN0_bm; // put RN4871 in sleep mode
	
	while (1){	
		for(uint16_t i=0; i<65536; ++i){
			PORTC.OUTCLR = PIN0_bm; // wake up RN4871
			_delay_ms(5); // RN4871 needs 5ms to wake up until UART is responsive
			iBeacon_advertise(i, 0xFFFF-i, 100); // advertise as iBeacon
			PORTC.OUTSET = PIN0_bm; // put RN4871 in sleep mode
			PORTC.OUTTGL = PIN2_bm; // LED toggle
			_delay_ms(1000);
			
		}
	}
	return 0;
}