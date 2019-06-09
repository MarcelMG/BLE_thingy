/*	SPI master on AVR ATtiny3216
 *	uses pins PA1 to PA4 for MOSI, MISO, SCK, CS in this order
 *	by Marcel Meyer Garcia
 *	see LICENSE.txt
*/

#ifndef SPI_H
#define SPI_H

#include <avr/io.h>
#include <stdbool.h>

typedef enum SPIdataOrder {SPI_MSB_FIRST, SPI_LSB_FIRST} SPIdataOrder;

struct SPI_config{
	SPI_MODE_t mode;
	SPI_PRESC_t prescaler;
	bool useDoubleSpeed;
	SPIdataOrder dataOrder;
};

// set CS pin PA4 low (normally, to select the slave)
inline void SPI_CS_low(void){
	PORTA.OUTCLR = PIN4_bm;
}
// set CS pin PA4 high (normally, to deselect the slave)
inline void SPI_CS_high(void){
	PORTA.OUTSET = PIN4_bm;
}

void SPI_init(struct SPI_config* SPIcfg);
uint8_t SPI_transmit(uint8_t tx_data);

#endif