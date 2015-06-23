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
		grains[i].init(input, size, 40 * (44100.f / 1000.f));	
	}
	previous_start_time = 0;
	delay = 48000; // 1 second delay
	periodic = true;
	overlap = 2; // from 1.01 to 2
	playback_rate = 1.0;
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

void PGrainStream::set_periodic(int on){
	periodic = on;	
}

int PGrainStream::get_periodic(){
	return periodic;
}

void PGrainStream::set_overlap(float amount){
	overlap = amount;	
}

float PGrainStream::get_overlap(){
	return overlap;	
}

void PGrainStream::set_rate(float rate){
	playback_rate = rate;
}

float PGrainStream::get_rate(){
	return playback_rate;
}

stereo_sample * PGrainStream::synthesize(int write_ptr){ // uses write_ptr to enforce no overlap
	output_sample.left = output_sample.right = 0.f;
	for(int g = 0; g < max_grains; g++){
		// check for inactive grains, and if inactive, allocate them for the future	
		if(!grains[g].is_active()){ // if grain is inactive, set the params and set it active
			if(periodic){
				next_start_time = n + (duration * overlap); // add an overlap param?  
			} else {
				next_start_time = delay + n + (rand() % (delay * 4)); /// look at this later too...... placeholder
			}
			sample_offset = next_start_time - n; // account for write_ptr travel before grain becomes active
			next_start_sample = ((write_ptr + delay + sample_offset) + (rand() % delay)) % buffer_size; 
			next_location = (float)(rand() % 100) / 100.f;
			grains[g].generate_parameters(duration, next_start_sample, next_start_time, next_location, playback_rate);
			grains[g].activate();
			previous_start_time = next_start_time;
		} else { // grain is active, so call synthesize
			sample_buffer = grains[g].synthesize(n);
			/* clip and saturate if needed */
			output_sample.left += sample_buffer.left;
			if(output_sample.left < -1.0){
				output_sample.left = -1.0;
			}
			if(output_sample.left > 1.0){
				output_sample.left = 1.0;
			}
			output_sample.left = atan(output_sample.left);
			output_sample.right += sample_buffer.right;
			if(output_sample.right < -1.0){
				output_sample.right = -1.0;
			}
			if(output_sample.right > 1.0){
				output_sample.right = 1.0;
			}
			output_sample.right = atan(output_sample.right);
		}
	}
	n++; // increment time
	return &output_sample;
}