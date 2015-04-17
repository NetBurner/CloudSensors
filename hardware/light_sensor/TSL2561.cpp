
/*
 * TSL2561.cpp
 *
 *  Created on: Mar 31, 2014
 *      Author: forreststanley
 */

#include "TSL2561.h"

TSL2561::TSL2561() {
	_initialized = false;

}

TSL2561::~TSL2561() {
}

void TSL2561::init() {
	I2CInit();

	_addr = 0x39;
	_integration = TSL2561_INTEGRATIONTIME_13MS;
	_gain = TSL2561_GAIN_16X;

	I2CInit();
	int x = read8(0x0A);
	if (x & 0x0A) {
		iprintf("Found TSL2561\r\n");
	} else {
		iprintf("Device not found\r\n");
		//return false;
	}
	_initialized = true;
	// Set default integration time and gain
	setTiming(_integration);
	setGain(_gain);
	// Note: by default, the device is in power down mode on bootup
	disable();

	//return true;
}

void TSL2561::enable(void) {
	if (!_initialized)
		init();

	// Enable the device by setting the control bit to 0x03
	write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL,
			TSL2561_CONTROL_POWERON);
}

void TSL2561::disable(void) {
	if (!_initialized)
		init();

	// Disable the device by setting the control bit to 0x03
	write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_CONTROL,
			TSL2561_CONTROL_POWEROFF);
}

void TSL2561::setGain(tsl2561Gain_t gain) {
	if (!_initialized)
		init();

	enable();
	_gain = gain;
	write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_TIMING, _integration | _gain);
	disable();
}

void TSL2561::setTiming(tsl2561IntegrationTime_t integration) {
	if (!_initialized)
		init();

	enable();
	_integration = integration;
	write8(TSL2561_COMMAND_BIT | TSL2561_REGISTER_TIMING, _integration | _gain);
	disable();
}

int TSL2561::getSensorAverage(uint seconds) {
	int samples = 0;
	int avg = 0;
	uint timeNow = Secs;
	while ((Secs-timeNow) < seconds) {
		uint16_t x = getLuminosity(TSL2561_VISIBLE);
	    int a = x >> 8;
	    int b = x & 0xFF;
	    int c = b << 8;
	    c += a;
	    avg += c;
		samples++;
		OSTimeDly(1);
	}
	return (avg / samples);
}

uint32_t TSL2561::getFullLuminosity(void) {

	HiResTimer *timer = HiResTimer::getHiResTimer();
	if (!_initialized)
		init();

	// Enable the device by setting the control bit to 0x03
	enable();

	// Wait x ms for ADC to complete
	switch (_integration) {
	case TSL2561_INTEGRATIONTIME_13MS:
		//delay(14);
		timer->delay(0.014);
		break;
	case TSL2561_INTEGRATIONTIME_101MS:
		//delay(102);
		timer->delay(0.102);
		break;
	default:
		//delay(403);
		timer->delay(0.403);
		break;
	}

	uint32_t x;
	x = read16(	//TSL2561_REGISTER_CHAN1_LOW );
			TSL2561_COMMAND_BIT | TSL2561_WORD_BIT
					| TSL2561_REGISTER_CHAN1_LOW);
	x <<= 16;
	x |= read16( //TSL2561_REGISTER_CHAN1_LOW );
			TSL2561_COMMAND_BIT | TSL2561_WORD_BIT
					| TSL2561_REGISTER_CHAN0_LOW);

	disable();

	timer->releaseTimer();
	return x;
}
uint16_t TSL2561::getLuminosity(uint8_t channel) {

	uint32_t x = getFullLuminosity();

	if (channel == 0) {
		// Reads two byte value from channel 0 (visible + infrared)
		return (x & 0xFFFF);
	} else if (channel == 1) {
		// Reads two byte value from channel 1 (infrared)
		return (x >> 16);
	} else if (channel == 2) {
		// Reads all and subtracts out just the visible!
		return ((x & 0xFFFF) - (x >> 16));
	}

	// unknown channel!
	return 0;
}

uint16_t TSL2561::read16(uint8_t reg) {
	uint16_t rx = 0;
	I2CSendBuf(_addr, &reg, 1);
	I2CReadBuf(_addr, (BYTE*) &rx, 2);
	return rx;
}

uint8_t TSL2561::read8(uint8_t reg) {

	uint8_t rx = 0;
	I2CSendBuf(_addr, &reg, 1);
	I2CReadBuf(_addr, &rx, 1);

	return rx;
}

void TSL2561::write8(uint8_t reg, uint8_t value) {

	I2CSendBuf(_addr, &reg, 1);
	I2CSendBuf(_addr, &value, 1);
}
