#include "RN4871.h"

// the UUID (unique identifier) of the BLE beacon
char UUID[32] = "123456789ABCDEF123456789ABCDEF12";

// setup wakeup and reset pins, reset RN4871
void RN4871_init(){
	init_UART();
	PORTC.DIRSET = PIN0_bm; // PC0 - BLEwakeup as output
	PORTC.OUTCLR = PIN0_bm; // BLE uart active
	/* reset RN4871 */
	PORTC.OUTSET = PIN1_bm; // BLE RESET HIGH
	PORTC.DIRSET = PIN1_bm; // pin PC1 (BLE RESET) as output
	PORTC.OUTCLR = PIN1_bm;
	_delay_ms(10);
	PORTC.OUTSET = PIN1_bm;
	_delay_ms(100); // wait for RN4871 to boot up and be ready	
}

// wait for a response from the RN4871, e.g. ">" after entering command mode or "K" (from "AOK") after a successfull command
void RN4871_waitfor(char endchar){
	char str[10];
	char* str_ptr = str;
	*str_ptr = UART_get();
	while( *str_ptr != endchar ){
		++str_ptr;
		*str_ptr = UART_get();
	}
	++str_ptr;
	*str_ptr = '\0';
	//dbg_print_str(str); //print the response for debugging
}

// setup RN4871 as non-connectible beacon with an advertisement interval of adv_interval*0.625ms, so e.g. 640 for 1s interval
void RN4871_setup_beacon(uint16_t adv_interval){
	printf("$$$"); // enter command mode
	RN4871_waitfor('>'); // get response, should be: "CMD>"
	_delay_ms(1);
	printf("SS,10\r"); // set service to non-connectible beacon
	RN4871_waitfor('K'); // get response, should be: "AOK"
	_delay_ms(1);
	printf("STB,%.4X\r", adv_interval); // set advertisement interval to adv_interval*0.625ms, so e.g. 640 for 1s interval
	RN4871_waitfor('K'); // get response, should be: "AOK"
	_delay_ms(1);
	printf("SO,1\r"); // enable low power mode
	RN4871_waitfor('K'); // get response, should be: "AOK"
}
// set advertisement content as iBeacon and with given major, minor and txPower value
void iBeacon_advertise(uint16_t minor, uint16_t major, uint8_t TxPower){
	printf("IB,Z\r"); // clear advertisement content
	RN4871_waitfor('K'); // wait for "AOK" response
	printf("IB,FF,"); // set custom manufacturer (FF) advertisement content
	printf("4C000215"); // iBeacon preamble
	printf(UUID); // device UUID
	printf("%.4X", minor); // minor (2 bytes, i.e. uint16_t)
	printf("%.4X", major); // major (2 bytes, i.e. uint16_t)
	printf("%.2X", TxPower); // TxPower (1 byte, i.e. uint8_t) calibration value for distance calculation
	putchar('\r'); // end "IB,FF,..." command with carriage return
	RN4871_waitfor('K'); // wait for "AOK" response
}
// set wakeup pin high to put RN4871 to sleep (RF still works while sleeping, but UART doesn't)
void RN4871_sleep(){
	PORTC.OUTSET = PIN0_bm; // put RN4871 in sleep mode
}
// wakeup RN4871 from sleep and wait 5ms until UART is ready
void RN4871_wakeup(){
	PORTC.OUTCLR = PIN0_bm; // wake up RN4871
	_delay_ms(5); // RN4871 needs 5ms to wake up until UART is responsive
}