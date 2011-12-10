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
	internal_buffer = new float[buffer_size_samps];
	read_ptr = 0;
	write_ptr = 0;
	buffer_full = false;
	
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
			break;
		case kProgRate:
			prog_rate = value * 200.f;
			break;
		case kRandomAmt:
			random_amount = value;
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
		case kProgRate:
			return prog_rate / 200.f;
			break;
		case kRandomAmt:
			return random_amount;
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

	PGrainStream * grain_stream;
	grain_stream = new PGrainStream(buffer_size_samps, internal_buffer, prog_rate, duration * (SR / 1000));
	grain_stream->make_stream();
	
    for(int i = 0; i < sampleFrames; i++) // buffer fill loop
    {
		internal_buffer[write_ptr] = in1[i];
		write_ptr++;
		if(write_ptr >= buffer_size_samps){ // wrap
			write_ptr = 0;
			buffer_full = true;
		}
	}
	if(buffer_full){
		for(int i = 0; i < sampleFrames; i++) // output loop
		{
			(*out1++) = grain_stream->get_stream(read_ptr);
			read_ptr++;
			if(read_ptr >= buffer_size_samps){
				read_ptr = 0;
			}
		}
	}
	
	delete grain_stream;
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
