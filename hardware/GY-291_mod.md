A small breakout board called "GY-291" is used for the ADXL345 accelerometer. It is very cheap and can be found easily. To use it with BLEthingy, it has to be modified as follows:
* desolder the voltage regulator (the small 5-pin device next to the GND label). Since it will be powered by a 3V coin cell, the regulator is not needed and would waste additional current
* desolder all 4 pull-up resistors. Since SPI is used, they are not needed and would waste current. One is located next to the accelerometer in the middle, above the CS label, the other 3 are between the SDO and SDA labels and the coordinate axis label
* solder a small wire between the VCC connector pin and the positive terminal of the yellow tantalum capacitor to bypass the regulator. CAUTION: with tantalum caps, the dark stripe indicates the positive terminal, so it's the opposite of electrolytic caps

The resistors can be easily desoldered using a large tip, as they will stick to the iron's tip. To desolder the regulator, heat the pins on one side and bend it up using a cutter knife while the solder is molten. Then heat the opposite side and remove the component.
I advise to use flux (e.g. from a flux pen) and desoldering wick to clean the pads after desoldering the components. Then clean the flux residues using rubbing alcohol (ethanol).

![mod_pic](https://github.com/MarcelMG/BLE_thingy/raw/master/hardware/gy-291_mod_pic.jpg)
