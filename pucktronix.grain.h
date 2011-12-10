//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : phaser.h
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// � 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#ifndef __pgranulator__
#define __pgranulator__

// #define SR 44100.f // update this to get SR from host
#define F_PI 3.14159f
#include "public.sdk/source/vst2.x/audioeffectx.h"
#include "PGrain.h"
//-------------------------------------------------------------------------------------------------------

enum
{
	// Global
	kNumPrograms = 16,
	
	// Parameters Tags
	kDuration = 0,
	kNumStreams,
	kProgRate,
	kRandomAmt,
	kNumParams
};

typedef struct {
	int duration;
	int start_sample_read;
	int start_sample_write;
	
} GrainParams;


//class Phaser;

class PGranulator : public AudioEffectX
{
public:
	PGranulator (audioMasterCallback audioMaster);
	~PGranulator ();

	int SR;
	
	float duration;
	float prog_rate;
	float random_amount;
	int buffer_size_samps;
	float * internal_buffer;
	float * working_buffer;
	int read_ptr, write_ptr, output_ptr;
	bool buffer_full;
	int numStreams;
	int num_grains;
	GrainParams * grain_params;
	long int time;
	// Processing
	virtual void processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames);
	virtual void processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames);

	// Program
	virtual void setProgramName (char* name);
	virtual void getProgramName (char* name);

	// Parameters
	virtual void setParameter (VstInt32 index, float value);
	virtual float getParameter (VstInt32 index);
	virtual void getParameterLabel (VstInt32 index, char* label);
	virtual void getParameterDisplay (VstInt32 index, char* text);
	virtual void getParameterName (VstInt32 index, char* text);

	virtual bool getEffectName (char* name);
	virtual bool getVendorString (char* text);
	virtual bool getProductString (char* text);
	virtual VstInt32 getVendorVersion ();

protected:
	
	char programName[kVstMaxProgNameLen + 1];
	
};

#endif
