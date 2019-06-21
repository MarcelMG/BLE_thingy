/*	Driver for the ADXL345 accelerometer using SPI for communication
 *	(c)2019 by Marcel Meyer Garcia
 *	see LICENSE.txt
*/
#ifndef ADXL345_H
#define ADXL345_H

#include <stdbool.h>
#include <util/delay_basic.h>
#include <math.h>
#include "SPI.h"

typedef enum {	ADXL345_sleepMode_8Hz,
				ADXL345_sleepMode_4Hz, 
				ADXL345_sleepMode_2Hz,
				ADXL345_sleepMode_1Hz	}ADXL345_sleepModeFrequency;
typedef enum {	ADXL345_overrunInterrupt,	// 0
				ADXL345_watermarkInterrupt, // 1
				ADXL345_freeFallInterrupt,	// 2
				ADXL345_inactivityInterrupt,// 3
				ADXL345_activityInterrupt,	// 4
				ADXL345_doubleTapInterrupt,	// 5
				ADXL345_singleTapInterrupt,	// 6
				ADXL345_dataReadyInterrupt	/* 7 */	}ADXL345_interrupt;
typedef enum {	ActiveHigh,
			    ActiveLow }ADXL345_interruptPolarity;
typedef enum {	ADXL345_range_2g, // value is: 0
				ADXL345_range_4g, // 1
				ADXL345_range_8g, // 2
				ADXL345_range_16g /* 3 */	}ADXL345_range;
typedef enum {	ADXL345_bypassFIFO, // value is: 0
				ADXL345_normalFIFO, // 1
				ADXL345_streamFIFO, // 2
				ADXL345_triggerFIFO /* 3 */	}ADXL345_FIFOmode;				
typedef enum {	ADXL345_actX = (1<<6),
				ADXL345_actY = (1<<5),
				ADXL345_actZ = (1<<4),
				ADXL345_actXY = (1<<6)|(1<<5),
				ADXL345_actXZ = (1<<6)|(1<<4),
				ADXL345_actYZ = (1<<5)|(1<<4),
				ADXL345_actXYZ = (1<<6)|(1<<5)|(1<<4)	}ADXL_345_Activity_Axis;
typedef enum {	ADXL345_inactX = (1<<2),
				ADXL345_inactY = (1<<1),
				ADXL345_inactZ = (1<<0),
				ADXL345_inactXY = (1<<2)|(1<<1),
				ADXL345_inactXZ = (1<<2)|(1<<0),
				ADXL345_inactYZ = (1<<1)|(1<<0),
				ADXL345_inactXYZ = (1<<2)|(1<<1)|(1<<0)	}ADXL_345_Inactivity_Axis;
			
/* DON'T CHANGE THESE 2 VARIABLES (consider them as 'private') */
ADXL345_range ADXL345_currentRange;
bool ADXL345_currentFullRes; 
				
bool ADXL345_init(void);
uint8_t ADXL345_readRegister(uint8_t regAddr);
bool ADXL345_readRegisterBit(uint8_t regAddr, uint8_t bitPos);
void ADXL345_writeRegister(uint8_t regAddr, uint8_t regVal);
void ADXL345_writeRegisterBit(uint8_t regAddr, uint8_t bitPos, bool bitVal);
void ADXL345_enterStandbyMode(void);
void ADXL345_enterMeasurementMode(void);
void ADXL345_enterSleepMode(ADXL345_sleepModeFrequency freq);
void ADXL345_wakeupFromSleepMode(void);
void ADXL345_linkActivityInactivity(bool link);
void ADXL345_setAutoSleep(bool autosleep);
void ADXL345_setInterrupt(ADXL345_interrupt interrupt, bool interruptVal);
bool ADXL345_checkInterrupt(ADXL345_interrupt interrupt);
void ADXL345_mapInterrupt_1(ADXL345_interrupt interrupt);
void ADXL345_mapInterrupt_2(ADXL345_interrupt interrupt);
void ADXL345_setInterruptPolarity(ADXL345_interruptPolarity pol);
void ADXL345_setFullResolution(bool fullRes);
void ADXL345_setRange(ADXL345_range range);
void ADXL_configFIFO(ADXL345_FIFOmode mode);
uint8_t ADXL345_getFIFOSamplesAvailable(void);
void ADXL345_getRawData(int16_t* x, int16_t* y, int16_t* z);
void ADXL345_getAccelerationSI(float* x_f, float* y_f, float* z_f);
void ADXL345_getTiltAnglesDegrees(float* roll, float* pitch);
void ADXL345_configActivityDetection(float threshold, bool ac, ADXL_345_Activity_Axis axis);
void ADXL345_configInactivityDetection(float threshold, bool ac, ADXL_345_Inactivity_Axis axis, uint8_t inact_time);

#endif