/*	BLEthingy -- Accelerometer Activity Detection + Temperature & Voltage Measurement BLE Beacon */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "RN4871.h"
#include "ADXL345.h"

// interrupt variables
volatile bool activity = false;
volatile bool rtc_wakeup = false;
volatile bool pushbutton_pressed = false;
volatile uint8_t err = 0;

// pushbutton interrupt handler, wakes CPU from power-down sleep mode
ISR(PORTB_PORT_vect){
	if(PORTB.INTFLAGS != PIN5_bm){
		err = 3; // error: wrong pin has triggered the interrupt
	}else if( !(PORTB.IN & PIN5_bm) ){ // a falling edge has triggered the interrupt (button has been pressed)
			pushbutton_pressed = true;
	} // else a rising edge has triggered the interrupt (releasing the button) and we do nothing

	PORTB.INTFLAGS = PIN5_bm; // clear interrupt flag
}


// ADXL345 interrupt handler, wakes CPU from power-down sleep mode
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

// RTC PIT interrupt handler, wakes CPU from power-down sleep mode. Occurs every 4s
ISR(RTC_PIT_vect){
	rtc_wakeup = true;
	RTC.PITINTFLAGS = RTC_PI_bm;	// clear interrupt flag
}

// measure internal temperature of ATtiny2316 in °C
int16_t measure_internal_temperature(){
	// setup ADC to measure temperature
	VREF.CTRLA = VREF_ADC0REFSEL_1V1_gc;
	ADC0.CTRLC = ADC_PRESC_DIV256_gc | ADC_REFSEL_INTREF_gc | ADC_SAMPCAP_bm;
	ADC0.MUXPOS = ADC_MUXPOS_TEMPSENSE_gc;
	ADC0.CTRLA = ADC_ENABLE_bm;
	ADC0.COMMAND = ADC_STCONV_bm;
	while( !(ADC0.INTFLAGS & ADC_RESRDY_bm) );
	// calculate T in °C
	int8_t sigrow_offset = SIGROW.TEMPSENSE1;
	uint8_t sigrow_gain = SIGROW.TEMPSENSE0;
	int32_t tmp = ADC0.RES - sigrow_offset;
	tmp *= sigrow_gain;
	tmp += 0x80;
	tmp >>= 8;
	tmp -= 273; // °K to °C
	ADC0.CTRLA = 0; // disable ADC
	return ((int16_t) tmp);	
}

// measure VCC voltage internally, result in mV
uint16_t measure_VCC(){
	// setup ADC to measure VCC
	ADC0.MUXPOS = ADC_MUXPOS_INTREF_gc;
	ADC0.CTRLC = ADC_PRESC_DIV256_gc | ADC_REFSEL_VDDREF_gc;
	ADC0.CTRLA = ADC_ENABLE_bm;
	ADC0.COMMAND = ADC_STCONV_bm;
	while( !(ADC0.INTFLAGS & ADC_RESRDY_bm) );
	ADC0.CTRLA = 0; // disable ADC
	return (1126400 / ADC0.RES); // VCC in mV	
}

int main(void)
{
	/* ONBOARD LED SETUP */
	PORTC.DIRSET = PIN2_bm;	// pin PC2 (LED) as output
	PORTC.OUTCLR = PIN2_bm; // LED off
	
	/* ONBOARD PUSHBUTTON SETUP */
	PORTB.DIRCLR = PIN5_bm; // config pin PB5 as input
	PORTB.PIN5CTRL = PORT_PULLUPEN_bm | PORT_ISC_BOTHEDGES_gc; // enable pull-up and interrupt on both edges (NOTE: only LEVEL and BOTHEDGES can wake the µC from sleep, so we can't use falling edge)
	
	/* BLE BEACON SETUP */
	RN4871_init();
	RN4871_setup_beacon(6400); // setup Beacon with 10s advertisement interval
	
	/* ACCELEROMETER SETUP */
	ADXL345_init();
	PORTA.DIRCLR = PIN5_bm; // config pin PA5 (connected to ADXL345 INT1) as input
	PORTA.PIN5CTRL = PORT_ISC_LEVEL_gc; // enable LEVEL interrupt (triggers when input is low) NOTE: only LEVEL and BOTHEDGES can wake the µC from sleep!
	ADXL345_configActivityDetection(5.0, false, ADXL345_actXY); // setup activity detection in dc-coupled mode, with X- and Y-axis and a threshold of 5.0 m/s²
	ADXL345_mapInterrupt_1(ADXL345_activityInterrupt); // map the activity interrupt to the INT1 pin of the ADXL345
	ADXL345_setInterrupt(ADXL345_watermarkInterrupt, false); // disable watermark interrupt, as it is enabled by default
	ADXL345_setInterruptPolarity(ActiveLow); // set interrupt polarity to active low (affects all interrupts)
	ADXL345_setInterrupt(ADXL345_activityInterrupt, true); // enable activity interrupt, it works even within sleep mode
	ADXL345_enterSleepMode(ADXL345_sleepMode_8Hz); // enter sleep mode and set the sample rate in sleep mode to 8Hz
	
	/* REDUCE POWER CONSUMPTION OF GPIOs */
	// set unused pins as inputs with pull-up and disable input buffer to reduce power consumption
	PORTA.DIRCLR = PIN6_bm;
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.DIRCLR = PIN0_bm | PIN1_bm | PIN4_bm;
	PORTB.PIN0CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN1CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTB.PIN4CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	PORTC.DIRCLR = PIN3_bm;
	PORTC.PIN3CTRL = PORT_PULLUPEN_bm | PORT_ISC_INPUT_DISABLE_gc;
	BOD.CTRLA &=~(BOD_SLEEP0_bm | BOD_SLEEP1_bm); // disable brown-out-detector in sleep mode to reduce power consumption
	
	/* RTC SETUP */
	while(RTC.STATUS); // wait until RTC is synchronized
	RTC.CLKSEL = RTC_CLKSEL_INT1K_gc; // select 1024 Hz clock from internal 32kHz oscillator
	RTC.PITINTCTRL = RTC_PI_bm; // enable periodic interrupt
	RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm;	// select interrupt period and enable PIT
		
	/* SLEEP MODE SETUP */	
	sei(); // globally enable interrupts on the µC
	set_sleep_mode(SLEEP_MODE_PWR_DOWN); // set sleep mode to power-down
	sleep_enable();	// enable sleep mode
	
	/* VARIABLES FOR BLE BEACON ADVERTISEMENT PACKET */
	uint16_t major, minor;
	const uint8_t txpwr = 100;
	
	/* MAIN LOOP */
	while (1){
		sleep_cpu(); // put ATtiny3216 in sleep mode (again)
		/* if either the RTC, the pushbutton or the accelerometer interrupt wake up the CPU, we continue here */
		if(pushbutton_pressed){
			major = 0xEEEE;
			minor = 0xEEEE;
			pushbutton_pressed = false;
		}else if(activity){ // accelerometer interrupt has caused wakeup
			major = 0xFFFF;
			minor = 0xFFFF;
			activity = false;
		}else{ // RTC interrupt has caused wakeup
			major = measure_internal_temperature();
			minor = measure_VCC();
			rtc_wakeup = false;
		}
		PORTC.OUTSET = PIN2_bm; // LED on
		RN4871_wakeup();
		PORTC.OUTCLR = PIN2_bm; // LED off
		iBeacon_advertise(minor, major, txpwr); // advertise as Beacon
		RN4871_sleep();
	}
	return 0;
}