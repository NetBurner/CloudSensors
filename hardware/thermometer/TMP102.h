/*
 * Pin Setup for TMP102 thermometer using MOD5441X on a MOD-DEV-70
 *
 * Jumper Wires Required: 4
 *
 * J2[50] (VCC)		->	VCC
 * J2[49] (GND)		->	GND
 * J2[39] (I2C0SDA)	->	SDA
 * J2[42] (I2C0SCL)	->	SCL
 *
 */

#include <stdint.h>
#include <basictypes.h>
#include <i2cmaster.h>
#include <ucos.h>
#include <stdio.h>
#include <utils.h>

#ifndef TMP102_H_
#define TMP102_H_

class TMP102 {
public:
	TMP102();
	virtual ~TMP102();
	void init();
	int getSensorAverage(uint seconds);
private:
	int8_t _addr;
	uint8_t write8(uint8_t reg, uint8_t value);
	uint8_t read8(uint8_t reg);

};

#endif /* TMP102_H_ */
