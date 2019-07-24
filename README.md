# BLEthingy
BLEthingy is a small battery-powered device with Bluetooth-Low-Energy connectivity.

The main microcontroller used is an AVR ATtiny3216 from the new tiny-1 series.

BLE connectivity is provided by a RN4871-I RF module.

There is an ADXL345 3-axis accelerometer onboard for motion detection. It sits on a GY-291 breakout-board, which is very cheap and avoids hand-soldering the tiny package of the ADXL345 which is virtually impossible without hot air / reflow oven. The GY-291 has to be slightly modified, see [here](https://github.com/MarcelMG/BLE_thingy/blob/master/hardware/GY-291_mod.md) for further instructions.

A pushbutton and a user LED are also onboard.

On the back of the PCB, a holder for a CR2032 coin cell is mounted.

![front_view](https://github.com/MarcelMG/BLE_thingy/raw/master/hardware/front_view.jpg)
![back_view](https://github.com/MarcelMG/BLE_thingy/raw/master/hardware/back_view.jpg)

An enclosure has been designed which can be 3D-printed.

![enclosure](https://raw.githubusercontent.com/MarcelMG/BLE_thingy/master/enclosure/enclosure.bmp)
![lid](https://raw.githubusercontent.com/MarcelMG/BLE_thingy/master/enclosure/lid.bmp)
![enclosure_3dprint_picture](https://raw.githubusercontent.com/MarcelMG/BLE_thingy/master/enclosure/enclosure_3dprint_picture.bmp)
(c) 2019 Marcel Meyer-Garcia
<a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc/4.0/88x31.png" /></a><br />This work (hardware and software) is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc/4.0/">Creative Commons Attribution-NonCommercial 4.0 International License</a>. 
