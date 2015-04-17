/*
 * Pin Setup for Sound Detector using MOD5441X on a MOD-DEV-70
 *
 * Jumper Wires Required: 5
 *
 * J2[50] (VCC)		->	VCC
 * J2[49] (GND)		->	GND
 * J2[06] (A2D0)	-> 	ENVELOPE
 * J2[01] (GND)		->	J2[14] (A2D GND Reference)
 * J2[02] (VCC)		->	J2[05] (A2D VCC Reference)
 *
 */

#ifndef SOUNDDETECTOR_H_
#define SOUNDDETECTOR_H_


#include <sim.h>
#include <stdio.h>
#include <HiResTimer.h>
#include <utils.h>



class Sound_Detector {
public:
	Sound_Detector();
	virtual ~Sound_Detector();
	void init();
	int getSensorAverage(uint seconds);
private:
	void clear();
};

#endif
