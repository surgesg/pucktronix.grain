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
#include <math.h>
#define F_PI 3.14159f

PGrain::PGrain(){
		
}

PGrain::PGrain(float * buf, int _buffer_size){
/* constructor, called once when program is run */
	buffer = buf;
	window_function = new float[1024];
	// make cosine window
	for(int i = 0; i < 1024; i++)
		window_function[i] = 0.5 * (1 - cos((2 * F_PI * i) / (1024 - 1)));
	current_sample = 0;
	buffer_size = _buffer_size;
}

PGrain::~PGrain(){
	delete []buffer;
	delete window_function;
}

void PGrain::init(float * buf, int _buffer_size){
	/* constructor, called once when program is run */
	buffer = buf;
	window_function = new float[1024];
	// make cosine window
	for(int i = 0; i < 1024; i++)
		window_function[i] = 0.5 * (1 - cos((2 * F_PI * i) / (1024 - 1)));
	current_sample = 0;
	buffer_size = _buffer_size;
}

void PGrain::generate_parameters(int _duration, int _start_sample, int _start_time){
/* details for a given grain, called each time the grain is reused */
	duration = _duration;
	sample_offset = current_sample = _start_sample;
	window_increment = 1024 / duration;
	window_index = 0;
	n = 0;
	start_time = _start_time;
}
	
bool PGrain::is_active(){
/* used to check if synthesize() should be called */
	return active;
}

void PGrain::activate(){
	active = true;		
}
	
int PGrain::get_start_time(){
	return start_time;	
}

float PGrain::synthesize(int time){
	/* handles time and sample calculations */
	if(time >= start_time){ // if grain is set to become active
		out_sample = buffer[current_sample] * window_function[window_index];
		if(n > duration){ // check that we're under duration of the given grain
			active = false; // this is going to be checked externally
		}
		n++;
		current_sample++;
		/* wrap sample index to buffer */
		if(current_sample >= buffer_size){
			current_sample = 0;
		}
		window_index += window_increment;
		return out_sample;
	}
	return 0;
}

