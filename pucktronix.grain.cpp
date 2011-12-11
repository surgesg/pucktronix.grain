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

	SR = 48000;
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
	duration = 40;
	prog_rate = 40;
	random_amount = 1;
	
	buffer_size_samps = SR * 4.f;
	internal_buffer = new float[buffer_size_samps]; // input delay line buffer
	working_buffer = new float[buffer_size_samps]; // working delay line for output
	read_ptr = 0;
	write_ptr = 0;
	output_ptr = 0;
	buffer_full = false;
	numStreams = 1;
	grain_params = new GrainParams;
	time = 0;
	
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
			duration = value * 2000.f;
			break;
		case kProgRate:
			prog_rate = value * 2000.f;
			break;
		case kRandomAmt:
			random_amount = value;
			break;
		case kNumStreams:
			numStreams = (int)(value * 8);
			break;
	}
	//((AEffGUIEditor*)editor)->setParameter (index, value);
}
//-----------------------------------------------------------------------------------------
float PGranulator::getParameter (VstInt32 index)
{
	switch (index){
		case kDuration:
			return duration / 2000.f;	
			break;
		case kProgRate:
			return prog_rate / 2000.f;
			break;
		case kRandomAmt:
			return random_amount;
			break;
		case kNumStreams:
			return numStreams / 8.f;
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
		case kProgRate:
			vst_strncpy (label, "Rate of Progression", kVstMaxParamStrLen);
			break;
		case kRandomAmt:
			vst_strncpy (label, "Amt of Random", kVstMaxParamStrLen);
			break;
		case kNumStreams:
			vst_strncpy (label, "Number of Streams", kVstMaxParamStrLen);
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
		case kProgRate:
			float2string(prog_rate, text, kVstMaxParamStrLen);
			break;
		case kRandomAmt:
			float2string(random_amount, text, kVstMaxParamStrLen);
			break;
		case kNumStreams:
			float2string(numStreams, text, kVstMaxParamStrLen);	
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
		case kProgRate:
			vst_strncpy (label, "ms", kVstMaxParamStrLen);
			break;
		case kRandomAmt:
			vst_strncpy (label, "%", kVstMaxParamStrLen);
			break;
		case kNumStreams:
			vst_strncpy (label, "streams", kVstMaxParamStrLen);
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
    float* out1 = outputs[0];
	
	for(int i = 0; i < sampleFrames; i++) // buffer fill loop
    {
		internal_buffer[write_ptr] = in1[i];
		write_ptr++;
		if(write_ptr >= buffer_size_samps / 2){ // wrap
			buffer_full = true;
		}
		if(write_ptr >= buffer_size_samps){
			write_ptr = 0;
		}
	}
	if(buffer_full){ // somehow constrain sample writing to areas whch won't be heard in the next sampleFrame... probably not possible 
					// basically need to reserve a section which is no longer being modified
					// shifting samples out, so to speak
		num_grains = 16;
		for(int i = 0; i < num_grains; i++){ // each grain
			grain_params->duration = duration * (48000.f / 1000.f);
			grain_params->start_sample_write = rand() % sampleFrames; // start writing within the output buffer...... 
			grain_params->start_sample_read = (write_ptr + (rand() % buffer_size_samps)) % buffer_size_samps;
			for(int samp = 0; samp < grain_params->duration; samp++){
				working_buffer[(samp + grain_params->start_sample_write) % buffer_size_samps] = internal_buffer[(samp + grain_params->start_sample_read) % buffer_size_samps] *
																								(0.5 * (1 - cos((2 * F_PI * samp) / (grain_params->duration - 1))));
			}
		}
	}
	
	for(int i = 0; i < sampleFrames; i++){ 	// copy output buffer to output
		(*out1++) = working_buffer[i]; 
	}
	memcpy(working_buffer, working_buffer + sampleFrames, sizeof(float) * sampleFrames); // shift out old samples
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
