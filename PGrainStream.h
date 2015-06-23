/*
 *  PGrainStream.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#include "PGrain.h"

#ifndef __PGRAINSTREAM__
#define __PGRAINSTREAM__

class PGrainStream{
public:
	PGrainStream(float * input, int size, int max_grains);
	~PGrainStream();
	stereo_sample * synthesize(int write_ptr);
	void set_parameters(int duration, int d_time);
	void set_window(int shape);
	void set_periodic(int on);
	void set_overlap(float amount);
	void set_rate(float rate);
	float get_rate();
	float get_overlap();
	int get_periodic();
private:
	int n, delay, sample_offset;
	int duration;
	float overlap;
	float playback_rate;
	bool periodic;
	int next_start_time, next_start_sample, previous_start_time;
	float next_location;
	int max_grains;
	PGrain * grains;
	int buffer_size;
	float * input_buffer;
	stereo_sample output_sample;
	stereo_sample sample_buffer;
};

#endif