# SoftUART_TX

a simple software UART transmitter with delays (blocking) to send small messages for debugging and testing purposes.
Output is redirected to *stdout*, so *printf()* can be used for formatted output.
This code is part of the *BLEthingy*-project (uses the AVR ATtiny3216), but can easily be ported to another platform by providing the following:
* *UART_TX_LOW()* - set TX-pin low
* *UART_TX_HIGH()* - set TX-pin high
* *UART_TX_SETOUTPUT()* - configure TX-pin as output
* *_delay_us(x)* - delay for x µs
