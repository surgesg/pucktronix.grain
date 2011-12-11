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

PGrainStream::PGrainStream(float * input, int size, int max_grains){
	buffer_size = size;
	input_buffer = input;
	n = 0;
	grains = new PGrain[max_grains];
	for(int i = 0; i < max_grains; i++){
		grains[i].init(input, size);	
	}
}

PGrainStream::~PGrainStream(){
	delete [] input_buffer;
}

float PGrainStream::synthesize(){
	float output_sample = 0;
	for(int g = 0; g < max_grains; g++){
		// check for inactive grains, and if inactive, allocate them for the future	
		if(!grains[g].is_active()){ // if grain is inactive, set the params
			
		} else { // grain is active, so call synthesize
			output_sample += grains[g].synthesize(n);
		}
	}
	n++; // increment time
}