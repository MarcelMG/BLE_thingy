# BLEthingy application

Features:
* BLE Beacon advertisement (non-connectible advertising)
* motion detection with accelerometer
* pushbutton
* temperature measurement
* battery voltage measurement

The advertisement interval is set to 10s, i.e. every 10s an advertisement package is sent via BLE.
The microcontroller is woken up by the RTC every 32s to measure the temperature and the battery voltage and to send those values.
The accelerometer detects activity (motion) and can then wake up the microcontroller to send a message.
The BLE RF-module is also in sleep mode unless woken up by the microcontroller.

The average current consumption is approx. 190µA with the above settings (intervals). During BLE transmission and when the LED is on, the maximum current is about 14mA. The current was measured with the oscilloscope across a 10 Ohm shunt resistor.
This amounts to a battery life of roughly 1 month with the CR2032 coin cell battery.
