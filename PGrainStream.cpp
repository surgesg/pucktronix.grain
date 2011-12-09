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

PGrainStream::PGrainStream(int _buffer_size, float * input){
	buffer_size = _buffer_size;
	stream_buffer = new float[_buffer_size];
	for(int i = 0; i < buffer_size; i++){
		stream_buffer[i] = 0.0;
	}
	offset_in_samps = 100;
	duration_in_samps = 90;
	stream_index = 0;
	input_buffer = input;
}

void PGrainStream::make_stream(){
	while(stream_index + duration_in_samps + offset_in_samps < buffer_size){
		float * grain_samples;
		int grains_index = 0;
		PGrain * current_grain;
		current_grain = new PGrain(input_buffer, duration_in_samps, stream_index);
		stream_index += offset_in_samps;
		grain_samples = current_grain->getGrains();
		for(int s = stream_index; s < stream_index + duration_in_samps; s++){
			stream_buffer[s] = grain_samples[grains_index];
			grains_index++;
		}
		stream_index += duration_in_samps;
		delete grain_samples;
		delete current_grain;
	}
}

float PGrainStream::get_stream(int index){
	return stream_buffer[index];
}