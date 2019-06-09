/*	SPI master on AVR ATtiny3216
 *	uses pins PA1 to PA4 for MOSI, MISO, SCK, CS in this order
 *	by Marcel Meyer Garcia
 *	see LICENSE.txt
*/

#include <avr/io.h>
#include "SPI.h"

int main(void){
	/* SETUP SPI */
	struct SPI_config ADXL345_SPIcfg;
	ADXL345_SPIcfg.mode = SPI_MODE_3_gc; 
	ADXL345_SPIcfg.prescaler = SPI_PRESC_DIV64_gc;
	ADXL345_SPIcfg.useDoubleSpeed = true;
	ADXL345_SPIcfg.dataOrder = SPI_MSB_FIRST;
	SPI_init(&ADXL345_SPIcfg);
	
	/* TRANSMIT & RECEIVE ONE BYTE */
	SPI_CS_low();
	uint8_t response = SPI_transmit(0x02);
	SPI_CS_high();

}

