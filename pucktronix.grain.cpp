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
	setUniqueID ('phas');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	SR = getSampleRate();
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
	
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
		case kRate:
			//Rate(value);
			break;
		case kFeedBack:
			//Feedback(value);
			break;
		case kDepth:
			//Depth(value);
			break;
		case kNumStages:
			//NumStages((int)(value * 256));
			break;
	}
	//((AEffGUIEditor*)editor)->setParameter (index, value);
}
//-----------------------------------------------------------------------------------------
float PGranulator::getParameter (VstInt32 index)
{
	switch (index){
		case kRate:
			//return _rate;	
		case kFeedBack:
			//return _fb;
		case kDepth:
			//return _depth;
		case kNumStages:
			//return num_stages / 256.f;
			break;
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::getParameterName (VstInt32 index, char* label)
{
	switch (index){
		case kRate:
			vst_strncpy (label, "Rate", kVstMaxParamStrLen);
			break;
		case kFeedBack:
			vst_strncpy (label, "Feedback", kVstMaxParamStrLen);
			break;
		case kDepth:
			vst_strncpy (label, "Depth", kVstMaxParamStrLen);
			break;
		case kNumStages:
			vst_strncpy (label, "Stages", kVstMaxParamStrLen);
			break;
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index){
		case kRate:
			//float2string(_rate, text, kVstMaxParamStrLen);	
			break;
		case kFeedBack:
			//float2string(_fb, text, kVstMaxParamStrLen);
			break;
		case kDepth:
			//float2string(_depth, text, kVstMaxParamStrLen);
			break;
		case kNumStages:
			//int2string(num_stages, text, kVstMaxParamStrLen);
			break;
	}
}

//-----------------------------------------------------------------------------------------
void PGranulator::getParameterLabel (VstInt32 index, char* label)
{
	switch (index){
		case kRate:
			vst_strncpy (label, "Hz", kVstMaxParamStrLen);
			break;
		case kFeedBack:
			vst_strncpy (label, "%", kVstMaxParamStrLen);
			break;
		case kDepth:
			vst_strncpy (label, "%", kVstMaxParamStrLen);
			break;
		case kNumStages:
			vst_strncpy (label, "Stages", kVstMaxParamStrLen);
			break;
	}
}

//------------------------------------------------------------------------
bool PGranulator::getEffectName (char* name)
{
	vst_strncpy (name, "pucktronix.phaser", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool PGranulator::getProductString (char* text)
{
	vst_strncpy (text, "phas", kVstMaxProductStrLen);
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

    while (--sampleFrames >= 0)
    {
		//(*out1++) = Update(*in1++);
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
