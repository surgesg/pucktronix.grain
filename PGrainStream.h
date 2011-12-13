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
	float synthesize(int write_ptr);
	void set_parameters(int duration, int d_time);
	void set_window(int shape);
private:
	int n, delay, sample_offset;
	int duration;
	int next_start_time, next_start_sample, previous_start_time;
	int max_grains;
	PGrain * grains;
	int buffer_size;
	float * input_buffer;
};

#endif