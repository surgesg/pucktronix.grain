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
	delay = 24000; // half second delay
}

PGrainStream::~PGrainStream(){
	delete [] input_buffer;
}

float PGrainStream::synthesize(int write_ptr){ // uses write_ptr to enforce no overlap
	float output_sample = 0;
	for(int g = 0; g < max_grains; g++){
		// check for inactive grains, and if inactive, allocate them for the future	
		if(!grains[g].is_active()){ // if grain is inactive, set the params and set it active
			next_start_sample = ((write_ptr + delay) + (rand() % (buffer_size /  2))) % buffer_size; 
			next_start_time = delay + n + (rand() % delay); /// look at this later too...... placeholder
			grains[g].generate_parameters(800, next_start_sample, next_start_time);
			grains[g].activate();
			previous_start_time = next_start_time;
		} else { // grain is active, so call synthesize
			output_sample += grains[g].synthesize(n);
		}
	}
	n++; // increment time
	return output_sample;
}