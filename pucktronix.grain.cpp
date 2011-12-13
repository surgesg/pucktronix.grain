//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : phaser.cpp
// Created by   : Greg Surges
// Description  : Mono phaser plugin
//
// � 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "pucktronix.grain.h"
#include "PGrainStream.h"
#include <math.h>
#include <stdlib.h>
#include "PGrain.h"

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new PGranulator (audioMaster);
}

//-------------------------------------------------------------------------------------------------------
PGranulator::PGranulator (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	
{
	setNumInputs (1);		// mono in
	setNumOutputs (1);		// mono out
	setUniqueID ('ptgr');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	SR = getSampleRate();
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	duration = 40 * (SR / 1000.f);
	delay_time = 40 * (SR / 1000.f);
	random_amount = 1;
	wet_dry = 0.5;
	
	buffer_size_samps = SR * 4.f;
	internal_buffer = new float[buffer_size_samps]; // input delay line buffer
	working_buffer = new float[buffer_size_samps]; // working delay line for output
	read_ptr = 0;
	write_ptr = 0;
	output_ptr = 0;
	buffer_full = false;
	numStreams = 1;
	grain_stream = new PGrainStream(internal_buffer, buffer_size_samps, 50);
	grain_stream->set_parameters(duration, delay_time);
	time = 0;
	window = HANN;
	
//	editor = new PGranulatorEditor(this);
}

//-------------------------------------------------------------------------------------------------------
PGranulator::~PGranulator ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------

void PGranulator::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void PGranulator::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void PGranulator::setParameter (VstInt32 index, float value)
{
	switch (index){
		case kDuration:
			duration = value * 200.f;
			grain_stream->set_parameters(duration * (SR / 1000.f), delay_time * (SR / 1000.f));
			break;
		case kDelayTime:
			delay_time = 1 + value * 2000.f;
			grain_stream->set_parameters(duration * (SR / 1000.f), delay_time * (SR / 1000.f));
			break;
		case kWetDry:
			wet_dry = value;
			break;
		case kPeriodic:
			grain_stream->set_periodic((int)value);
			break;
		case kOverlap:
			grain_stream->set_overlap(1.01f + value);
			break;
		case kWindow:
			window = (int)(value * 4);
			switch(window){
				case 0:
					grain_stream->set_window(HAMMING);
					break;
				case 1:
					grain_stream->set_window(TRIANGLE);
					break;
				case 2:
					grain_stream->set_window(HANN);
					break;
				case 3:
					grain_stream->set_window(FLAT_TOP);
					break;
				case 4:
					grain_stream->set_window(COSINE);
					break;
			}		
			break;
			
	}
	//((AEffGUIEditor*)editor)->setParameter (index, value);
}
//-----------------------------------------------------------------------------------------
float PGranulator::getParameter (VstInt32 index)
{
	switch (index){
		case kDuration:
			return duration / 200.f;	
			break;
		case kDelayTime:
			return delay_time / 2000.f;
			break;
		case kWetDry:
			return wet_dry;
			break;
		case kPeriodic:
			return grain_stream->get_periodic();
			break;
		case kOverlap:
			return grain_stream->get_overlap() - 1.01;
			break;
		case kWindow:
			return window / 4.f;
			break;
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::getParameterName (VstInt32 index, char* label)
{
	switch (index){
		case kDuration:
			vst_strncpy (label, "Duration", kVstMaxParamStrLen);
			break;
		case kDelayTime:
			vst_strncpy (label, "Delay Time", kVstMaxParamStrLen);
			break;
		case kWetDry:
			vst_strncpy (label, "Wet/Dry Mix", kVstMaxParamStrLen);
			break;
		case kPeriodic:
			vst_strncpy (label, "Periodic", kVstMaxParamStrLen);
			break;	
		case kOverlap:
			vst_strncpy (label, "Overlap", kVstMaxParamStrLen);
			break;	
		case kWindow:
			vst_strncpy (label, "Window", kVstMaxParamStrLen);
			break;
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index){
		case kDuration:
			float2string(duration, text, kVstMaxParamStrLen);	
			break;
		case kDelayTime:
			float2string(delay_time, text, kVstMaxParamStrLen);
			break;
		case kWetDry:
			float2string(wet_dry * 100, text, kVstMaxParamStrLen);	
			break;
		case kPeriodic:
			int2string(grain_stream->get_periodic(), text, kVstMaxParamStrLen);
			break;
		case kOverlap:
			float2string((grain_stream->get_overlap() - 1.01) * 100.f, text, kVstMaxParamStrLen);	
			break;
		case kWindow:
			switch(window){
				case HAMMING:
					strcpy(text, "HAMMING");
					break;
				case TRIANGLE:
					strcpy(text, "TRIANGLE");
					break;
				case HANN:
					strcpy(text, "HANN");
					break;	
				case FLAT_TOP:
					strcpy(text, "FLAT_TOP");
					break;
				case COSINE:
					strcpy(text, "COSINE");
					break;	
			}
			break;
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::getParameterLabel (VstInt32 index, char* label)
{
	switch (index){
		case kDuration:
			vst_strncpy (label, "ms", kVstMaxParamStrLen);
			break;
		case kDelayTime:
			vst_strncpy (label, "ms", kVstMaxParamStrLen);
			break;
		case kWetDry:
			vst_strncpy (label, "%", kVstMaxParamStrLen);
			break;
		case kPeriodic:
			vst_strncpy (label, "On/Off", kVstMaxParamStrLen);
			break;
		case kOverlap:
			vst_strncpy (label, "%", kVstMaxParamStrLen);
			break;
		case kWindow:
			vst_strncpy (label, "window", kVstMaxParamStrLen);
			break;
	}
}

//------------------------------------------------------------------------
bool PGranulator::getEffectName (char* name)
{
	vst_strncpy (name, "pucktronix.granulator", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool PGranulator::getProductString (char* text)
{
	vst_strncpy (text, "ptgr", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool PGranulator::getVendorString (char* text)
{
	vst_strncpy (text, "pucktronix", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 PGranulator::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void PGranulator::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    float* in2  =  inputs[1];

    float* out1 = outputs[0];
    float* out2 = outputs[1];
	SR = getSampleRate();

	// need two buffers, need two grain_streams to maintain stereo (if that matters... can the input be summed?)
	
	for(int i = 0; i < sampleFrames; i++) // buffer fill loop
    {
		internal_buffer[write_ptr] = in1[i];
		write_ptr++;
		if(write_ptr >= buffer_size_samps / 2){ // can this be removed?
			buffer_full = true;
		}
		if(write_ptr >= buffer_size_samps){ // wrap
			write_ptr = 0;
		}
	}
	if(buffer_full){ // somehow constrain sample writing to areas whch won't be heard in the next sampleFrame... probably not possible 
		for(int i = 0; i < sampleFrames; i++){ 	// copy output buffer to output
			sample_buffer = grain_stream->synthesize(write_ptr);;
			(*out1++) = (sample_buffer->left * (1.f - wet_dry)) + (wet_dry * in1[i]); // move the delay/duration conversions out of process loop 
			(*out2++) = (sample_buffer->right * (1.f - wet_dry)) + (wet_dry * in2[i]); // move the delay/duration conversions out of process loop 
			time++;
		}	
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    double* out1 = outputs[0];


    while (--sampleFrames >= 0)
    {
		//(*out1++) = Update(*in1++);
    }
}
