/*
 *  PGrain.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#include "PGrain.h"
#include <math.h>
#define F_PI 3.14159f
#define WINDOW_SIZE 2048

PGrain::PGrain(){
		
}

PGrain::PGrain(float * buf, int _buffer_size){
/* constructor, called once when program is run */
	buffer = buf;
	window_function = new float[WINDOW_SIZE];
	// make default hann window
	for(int i = 0; i < WINDOW_SIZE; i++)
		window_function[i] = 0.5 * (1 - cos((2 * F_PI * i) / (WINDOW_SIZE - 1)));
	current_sample = 0;
	buffer_size = _buffer_size;
}

PGrain::~PGrain(){
	delete []buffer;
	delete window_function;
}

void PGrain::set_window(int shape){
	switch(shape){
		case HAMMING:
			for(int i = 0; i < WINDOW_SIZE; i++)
				window_function[i] = 0.54 - 0.46 * cos((2.f * F_PI * i) / (WINDOW_SIZE - 1.f)); // some noise here
			break;
		case TRIANGLE:
			for(int i = 0; i < WINDOW_SIZE; i++)
				window_function[i] = (2.f / (WINDOW_SIZE - 1)) * (((WINDOW_SIZE - 1) / 2.f) - fabs(i - ((WINDOW_SIZE - 1) / 2.f)));
			break;
		case HANN:
			for(int i = 0; i < WINDOW_SIZE; i++)
				window_function[i] = 0.5 * (1 - cos((2 * F_PI * i) / (WINDOW_SIZE - 1)));
			break;
		case FLAT_TOP: 
			for(int i = 0; i < WINDOW_SIZE; i++)
				window_function[i] = 1.f - 1.93 * cos((2.f * F_PI * i) / (WINDOW_SIZE - 1.f)) 
									 + 1.29 * cos((4.f * F_PI * i) / (WINDOW_SIZE - 1.f))
									 - 0.388 * cos((6.f * F_PI * i) / (WINDOW_SIZE - 1.f))
									 + 0.032 * cos((8.f * F_PI * i) / (WINDOW_SIZE - 1.f));
			break;
		case COSINE:
			for(int i = 0; i < WINDOW_SIZE; i++)
				window_function[i] = sin((F_PI * i) / (WINDOW_SIZE - 1.f));
			break;
	}
}

void PGrain::init(float * buf, int _buffer_size){
	/* constructor, called once when program is run */
	buffer = buf;
	window_function = new float[WINDOW_SIZE];
	// make cosine window
	for(int i = 0; i < WINDOW_SIZE; i++)
		window_function[i] = 0.5 * (1 - cos((2 * F_PI * i) / (WINDOW_SIZE - 1)));
	current_sample = 0;
	buffer_size = _buffer_size;
}

void PGrain::generate_parameters(int _duration, int _start_sample, int _start_time){
/* details for a given grain, called each time the grain is reused */
	duration = _duration;
	sample_offset = current_sample = _start_sample;
	window_increment = (float)(WINDOW_SIZE - 1) / duration;
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
		out_sample = buffer[current_sample] * window_function[(int)window_index % WINDOW_SIZE];
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

