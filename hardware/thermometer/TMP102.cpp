#include "TMP102.h"

TMP102::TMP102() {

}

void TMP102::init() {
    I2CInit();

	_addr = 0x48;

}

int TMP102::getSensorAverage(uint seconds) {
	int samples = 0;
	int avg = 0;
	uint timeNow = Secs;
	while ((Secs-timeNow) < seconds) {
		avg += read8(0x00);
		samples++;
		OSTimeDly(1);
	}
	int c = (avg / samples);
	int f = 1.8 * c + 32;
	return f;
}


uint8_t TMP102::read8(uint8_t reg) {

	uint8_t rx = 0;
	I2CSendBuf(_addr, &reg, 1);
	I2CReadBuf(_addr, &rx, 1);
	return rx;
}

uint8_t TMP102::write8(uint8_t reg, uint8_t value) {

	int status = I2CSendBuf(_addr, &reg, 1);
	I2CSendBuf(_addr, &value, 1);
	return status;
}

TMP102::~TMP102() {

}

