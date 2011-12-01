/*
 *  PGrain.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */


#include "PWindow.h"

#ifndef _PGRAIN_
#define _PGRAIN_

class PGrain{
public:
	PGrain(float * buf, int dur, float offset); // expects duration in samps
	~PGrain();
	float * getGrain();
private:	
	PWindow * window_function;
	float * buffer;
	int duration;
	float sample_offset;
};

#endif