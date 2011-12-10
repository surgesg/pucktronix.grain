/*
 *  PGrainStream.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

class PGrainStream{
public:
	PGrainStream(int buffer_size, float input[], int offset, int dur);
	~PGrainStream();
	int offset_in_samps;
	int duration_in_samps;
	void make_stream();
	float get_stream(int index);
private:
	int buffer_size;
	int stream_index;
	float * stream_buffer; // collects grains end to end - as long as current output buffer
	float * input_buffer;
};