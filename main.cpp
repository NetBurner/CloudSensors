#include "predef.h"
#include <stdio.h>
#include <init.h>
#include <ucos.h>
#include <ctype.h>
#include <startnet.h>

#include <nb-libxively/xively.h>
#include "hardware/sound_detector/SoundDetector.h"
#include "hardware/thermometer/TMP102.h"
#include "hardware/light_sensor/TSL2561.h"



extern "C" {
void UserMain(void * pd);
}

/*
 * API_KEY should be a long string
 * FEED_ID is a 10 digit number
 */

#define API_KEY     ""
#define FEED_ID     0

const char * AppName="XivelyClassDemo2";

void UserMain(void * pd) {
	init();
	iprintf("Application started\n");

	TMP102 sensor;
	sensor.init();

	xi_context_t *context = xi_create_context(XI_TCP,API_KEY,FEED_ID);
	static xi_feed_t f;
	f.feed_id           = FEED_ID;
	f.datastream_count  = 1; //Number of datastreams to be updated
	xi_datastream_t* d = &f.datastreams[ 0 ];
	d->datapoint_count = 1;
	xi_datapoint_t* p = &d->datapoints[ 0 ];
    while (1) {
		iprintf("Sensor Reading: %d\r\n",sensor.getSensorAverage(2));
    	xi_set_value_i32(p,sensor.getSensorAverage(5));
    	p->timestamp.timestamp = 0;
    	sprintf(d->datastream_id,"sensorX");
//        xi_feed_update(context, &f );
        OSTimeDly(1);
    }
}
