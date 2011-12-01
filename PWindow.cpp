/*
 *  PWindow.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 12/1/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#include "PWindow.h"
#include "math.h"

#define F_PI 3.14159f

PWindow::PWindow(int shape, int duration_in_samps){
	window_function = new float[duration_in_samps];
	n = 0;
	switch(shape){
		case HAMMING:
			for(int n = 0; n < duration_in_samps; n++){
				window_function[n] = 0.54 - 0.46 * cos((2 * F_PI * n) / (duration_in_samps - 1));
			}
			break;
	}
}

PWindow::~PWindow(){
	delete window_function;
}

float PWindow::next_samp(){
	float samp = window_function[n];
	n++;
	return samp;
}
