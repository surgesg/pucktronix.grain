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
	float synthesize();
private:
	int n;
	PGrain * grains;
	int buffer_size;
	float * input_buffer;
};

#endif