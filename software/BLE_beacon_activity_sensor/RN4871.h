/*	RN4871 BLE module, part of BLEthingy project
 *	by Marcel Meyer Garcia
 *	see LICENSE.txt
*/

#ifndef RN4871_H
#define RN4871_H

#include "uart.h"
#define F_CPU 3333333
#include <util/delay.h>

char UUID[32];

// setup wakeup and reset pins, reset RN4871
void RN4871_init();
// wait for a response from the RN4871, e.g. ">" after entering command mode or "K" (from "AOK") after a successfull command
void RN4871_waitfor(char endchar);
// setup RN4871 as non-connectible beacon with an advertisement interval of adv_interval*0.625ms, so e.g. 640 for 1s interval
void RN4871_setup_beacon(uint16_t adv_interval);
// set advertisement content as iBeacon and with given major, minor and txPower value
void iBeacon_advertise(uint16_t minor, uint16_t major, uint8_t TxPower);
// set wakeup pin high to put RN4871 to sleep (RF still works while sleeping, but UART doesn't)
void RN4871_sleep();
// wakeup RN4871 from sleep and wait 5ms until UART is ready
void RN4871_wakeup();


#endif