/*	This example shows how to put the ADXL345 and the main microcontroller in sleep mode and use the ADXL345's 
 *	activity detection feature to trigger an interrupt which wakes up the microcontroller to light up the LED.
 *	The RN4871 BLE module remains in reset-state(shutdown).	
 *	During sleep, a current consumption of ~115µA was measured with DMM.
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#include "ADXL345.h"
#include "soft_uart.h"
#include <avr/sleep.h>
#include <avr/interrupt.h>

volatile bool activity = false;
volatile uint8_t err = 0;

ISR(PORTA_PORT_vect){
	if(PORTA.INTFLAGS != PIN5_bm){
		err = 1; // error: wrong pin has triggered the interrupt
	}
	if(!ADXL345_checkInterrupt(ADXL345_activityInterrupt)){ // ADXL345_checkInterrupt() has to be called within the ISR to clear the interrupt flag on the ADXL345
		err = 2;// error: wrong event on the ADXL345 has triggered the interrupt
	}
	activity = true; // the activity interrupt has been triggered
	PORTA.INTFLAGS = PIN5_bm; // clear interrupt flag
}

int main(void)
{
	//soft_uart_setup();
	
	if( !ADXL345_init() ){
		printf("init success\n");
	}else{
		printf("init error\n");
		return 1;
	}
	
	PORTC.DIRSET = PIN2_bm;	// pin PC2 (LED) as output
	PORTC.OUTCLR = PIN2_bm; // LED off
	
	PORTA.DIRCLR = PIN5_bm; // config pin PA5 (connected to ADXL345 INT1) as input
	PORTA.PIN5CTRL = PORT_ISC_LEVEL_gc; // enable LEVEL interrupt (triggers when input is low) NOTE: only LEVEL and BOTHEDGES can wake the µC from sleep!
	
	ADXL345_configActivityDetection(5.0, false, ADXL345_actXY); // setup activity detection in dc-coupled mode, with X- and Y-axis and a threshold of 5.0 m/s²
	ADXL345_mapInterrupt_1(ADXL345_activityInterrupt); // map the activity interrupt to the INT1 pin of the ADXL345
	ADXL345_setInterrupt(ADXL345_watermarkInterrupt, false); // disable watermark interrupt, as it is enabled by default
	ADXL345_setInterruptPolarity(ActiveLow); // set interrupt polarity to active low (affects all interrupts)
	ADXL345_setInterrupt(ADXL345_activityInterrupt, true); // enable activity interrupt, it works even within sleep mode
	
	ADXL345_enterSleepMode(ADXL345_sleepMode_8Hz); // enter sleep mode and set the sample rate in sleep mode to 8Hz
	
	PORTC.DIRSET = PIN1_bm; // pin PC1 (BLE RESET) as output
	PORTC.OUTCLR = PIN1_bm; // BLE RESET low, keep RN4871 in reset state for lowest power consumption
	// set unused pins as inputs with pull-up and disable input buffer to reduce power consumption
	PORTA.DIRCLR = PIN6_bm;
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.DIRCLR = PIN0_bm | PIN1_bm | PIN4_bm | PIN5_bm;
	PORTB.PIN0CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN1CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTC.DIRCLR = PIN3_bm;
	PORTC.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;	
	BOD.CTRLA &=~(BOD_SLEEP0_bm | BOD_SLEEP1_bm); // disable brown-out-detector in sleep mode to reduce power consumption
	
	sei(); // globally enable interrupts on the µC
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set sleep mode to power-down
	sleep_enable();	// enable sleep mode
	
	while (1){
		sleep_cpu(); // go to sleep
		if(activity){	// activity has been detected
			/* light the LED for 1s */
			PORTC.OUTSET = PIN2_bm; // LED on
			activity = false;
			_delay_ms(1000);
			PORTC.OUTCLR = PIN2_bm; // LED off
		}
    }
	
}

