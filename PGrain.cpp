/*
 *  PGrain.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#include "PGrain.h"
#include "PWindow.h"

PGrain::PGrain(float * buf, int dur, float offset){
	buffer = buf;
	duration = dur;
	sample_offset = offset;
	window_function = new PWindow(HAMMING, dur);
}

PGrain::~PGrain(){
	delete buffer;
	delete window_function;
}

float * PGrain::getGrain(){
	float grainBuffer[duration];
	for(int current_sample = sample_offset; current_sample < sample_offset + duration; current_sample++){
		// iterate through samples and multiply with window
		// how to time-stretch window ? some percentage of total duration...
	}
	// return normalized float * to grainBuffer
}
