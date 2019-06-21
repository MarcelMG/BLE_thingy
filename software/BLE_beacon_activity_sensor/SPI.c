/*	SPI master on AVR ATtiny3216
 *	by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include "SPI.h"

// initialize the SPI peripheral with a certain configuration stored in the SPI_config struct
void SPI_init(struct SPI_config* SPIcfg){
	SPI_CS_high();
	PORTA.DIRSET = PIN1_bm | PIN3_bm | PIN4_bm; // MOSI ,SCK and CS pins as outputs (PA1, PA3 and PA4)
	PORTA.DIRCLR = PIN2_bm;	// MISO pin (PA2) as input
	SPI0.CTRLB = SPIcfg->mode; // set SPI mode (clock polarity and phase)
	SPI0.CTRLA = SPIcfg->prescaler;
	if(SPIcfg->useDoubleSpeed == true){
		SPI0.CTRLA |= SPI_CLK2X_bm; 
	}
	if(SPIcfg->dataOrder == SPI_LSB_FIRST){
		SPI0.CTRLA |= SPI_DORD_bm; // set LSB first (MSB first is default)
	}
	SPI0.CTRLA |= SPI_ENABLE_bm | SPI_MASTER_bm;
}

// transmit and receive one byte (polling)
uint8_t SPI_transmit(uint8_t tx_data){
	    SPI0.DATA = tx_data;	// put byte to be sent in transmit buffer
	    while (!(SPI0.INTFLAGS & SPI_IF_bm));	// wait until transmit buffer is empty, i.e. transmission is finished
	    return SPI0.DATA;	// return received byte from slave
}
