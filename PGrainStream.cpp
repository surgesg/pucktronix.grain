/*
 *  PGrainStream.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#include "PGrainStream.h"
#include "PGrain.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>

PGrainStream::PGrainStream(float * input, int size, int m_grains){
	buffer_size = size;
	input_buffer = input;
	n = 0;
	max_grains = m_grains;
	grains = new PGrain[max_grains];
	for(int i = 0; i < max_grains; i++){
		grains[i].init(input, size);	
	}
	previous_start_time = 0;
	delay = 48000; // 1 second delay
}

PGrainStream::~PGrainStream(){
	delete [] input_buffer;
}

void PGrainStream::set_parameters(int _duration, int d_time){
	delay = d_time;
	duration = _duration;
}

void PGrainStream::set_window(int shape){
	for(int i = 0; i < max_grains; i++){
		grains[i].set_window(shape);	
	}
}

float PGrainStream::synthesize(int write_ptr){ // uses write_ptr to enforce no overlap
	float output_sample = 0;
	for(int g = 0; g < max_grains; g++){
		// check for inactive grains, and if inactive, allocate them for the future	
		if(!grains[g].is_active()){ // if grain is inactive, set the params and set it active
			next_start_time = delay + n + (rand() % (delay * 4)); /// look at this later too...... placeholder
			sample_offset = next_start_time - n; // account for write_ptr travel before grain becomes active
			next_start_sample = ((write_ptr + delay + sample_offset) + (rand() % delay)) % buffer_size; 
			grains[g].generate_parameters(duration, next_start_sample, next_start_time);
			grains[g].activate();
			previous_start_time = next_start_time;
		} else { // grain is active, so call synthesize
			output_sample += grains[g].synthesize(n);
		}
	}
	n++; // increment time
	return output_sample;
}