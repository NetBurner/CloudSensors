#include "SoundDetector.h"

Sound_Detector::Sound_Detector() {
	// TODO Auto-generated constructor stub

}

Sound_Detector::~Sound_Detector() {
	// TODO Auto-generated destructor stub
}

void Sound_Detector::init()
{
    sim2.adc.cr1 = 0;      // Set up control register1
    sim2.adc.cr2 = 0;      // Set up control register2
    sim2.adc.zccr = 0;     // Disable ZC
    sim2.adc.lst1 = 0x3210;// Set samples 0-3 to ADC_IN0-3
    sim2.adc.lst2 = 0x7654;// Set samples 4-7 to ADC_IN4-7
    sim2.adc.sdis = 0;     // Enable all samples
    sim2.adc.sr = 0xFFFF;  // Clear Status Register
    for (int i = 0; i < 8; i++) { // Clear result + offset registers
        sim2.adc.rslt[i] = 0;
        sim2.adc.ofs[i] = 0;
    }
    sim2.adc.lsr = 0xFFFF; // Clear limit register
    sim2.adc.zcsr = 0xFFFF;// Clear ZC

    sim2.adc.pwr = 0;      // Everything is turned on
    sim2.adc.cal = 0;      // User internal calibration
    sim2.adc.pwr2 = 0x0005;// Default power conversion
    sim2.adc.div = 0x505;  // Set default
    sim2.adc.asdiv = 0x13; // ASDivisor set
}

void Sound_Detector::clear()
{
   sim2.adc.sr = 0xffff;   // Clear status codes
   sim2.adc.cr1 |= 0x2000; // Starting A2D
}

int Sound_Detector::getSensorAverage(uint seconds) {
	int samples = 0;
	int avg = 0;
	uint timeNow = Secs;
	while ((Secs-timeNow) < seconds) {
		clear();
		int a2d = sim2.adc.rslt[0];
		float volts = ((float) a2d / (32768.0)) * 3.3;
		avg += (volts/2.69) * 200;
		samples++;
		OSTimeDly(1);
	}
	return (avg / samples);
}
