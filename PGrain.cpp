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

float * PGrain::getGrains(){
	grainBuffer = new float[duration];
	int output_sample_index = 0;
	for(int current_sample = sample_offset; current_sample < sample_offset + duration; current_sample++){
		// iterate through samples and multiply with window
		// how to time-stretch window ? some percentage of total duration...
		grainBuffer[output_sample_index] = buffer[current_sample] * window_function->next_samp();
		output_sample_index++;
	}
	// return normalized float * to grainBuffer
	return grainBuffer;
}
