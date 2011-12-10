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

PGrainStream::PGrainStream(int _buffer_size, float input[], int offset, int dur){
	buffer_size = _buffer_size;
	stream_buffer = new float[_buffer_size];
	offset_in_samps = offset;
	duration_in_samps = dur;
	stream_index = 0;
	input_buffer = input;
}

PGrainStream::~PGrainStream(){
	delete [] stream_buffer;
}

void PGrainStream::make_stream(){ // somewhere in here, as duration decreases, things slow down
	while(stream_index + duration_in_samps + offset_in_samps < buffer_size){ // check that the next grain won't cross buffer barrier
		float * grain_samples;
		int grains_index = 0;
		float * samples_for_granulation;
		samples_for_granulation = new float[duration_in_samps];
		
		/* copy samples for next grain */
		for(int i = 0; i < duration_in_samps; i++){
			samples_for_granulation[i] = input_buffer[i + stream_index];
		}
		
		/* make next grain */
		PGrain * current_grain;
		current_grain = new PGrain(samples_for_granulation, duration_in_samps);
		grain_samples = current_grain->getGrains();

		/* copy samples from grain to output buffer */
		stream_index += offset_in_samps;
		for(int s = stream_index; s < stream_index + duration_in_samps; s++){
			stream_buffer[s] = grain_samples[grains_index];
			grains_index++;
		}
		
		
		/* prep for next iteration */
		stream_index += duration_in_samps;
		delete [] grain_samples;
		delete current_grain;
		delete [] samples_for_granulation;
	}
}

float PGrainStream::get_stream(int index){
	return stream_buffer[index];
}