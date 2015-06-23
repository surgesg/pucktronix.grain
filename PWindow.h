/*
 *  PWindow.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#ifndef _PWINDOW_
#define _PWINDOW_

enum{
	HAMMING = 0,
};

class PWindow{
public:
	PWindow(int shape, int duration_in_samps);
	~PWindow();
	float next_samp();
private:
	float * window_function;
	int n;
};

#endif