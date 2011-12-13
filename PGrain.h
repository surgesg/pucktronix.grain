/*
 *  PGrain.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#ifndef _PGRAIN_
#define _PGRAIN_

enum {
	HAMMING = 0,	
	TRIANGLE,
	HANN,
	FLAT_TOP,
	COSINE
};

class PGrain{
public:
	PGrain();
	PGrain(float * buf, int buffer_size); // expects duration in samps
	~PGrain();
	void init(float * buf, int buffer_size);
	float synthesize(int n);	
	void activate();
	bool is_active();
	void generate_parameters(int duration, int start_sample, int start_time);
	int get_start_time();
	void set_window(int shape);
private:	
	int buffer_size;
	int current_sample;
	bool active;
	float * window_function;
	float window_increment;
	float window_index;
	float * buffer;
	int duration;
	int n; // number of samples calculated of current grain
	int start_time;
	float sample_offset;
	float out_sample;
};

#endif