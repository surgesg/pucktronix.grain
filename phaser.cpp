//-------------------------------------------------------------------------------------------------------
// VST Plug-Ins SDK
// Version 2.4		$Date: 2006/11/13 09:08:27 $
//
// Category     : VST 2.x SDK Samples
// Filename     : phaser.cpp
// Created by   : Steinberg Media Technologies
// Description  : Stereo plugin which applies Gain [-oo, 0dB]
//
// � 2006, Steinberg Media Technologies, All Rights Reserved
//-------------------------------------------------------------------------------------------------------

#include "phaser.h"
#include <math.h>

//-------------------------------------------------------------------------------------------------------
AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
	return new Phaser (audioMaster);
}

AllpassDelay::AllpassDelay(){
	_a1 = 0.f;
	_zm1 = 0.f;
}

void AllpassDelay::Delay(float delay){ // sample delay time
	_a1 = (1.f - delay) / (1.f + delay);
}

float AllpassDelay::Update(float inSamp){
	float y = inSamp * -_a1 + _zm1;
	_zm1 = y * _a1 + inSamp;
	
	return y;
}


//-------------------------------------------------------------------------------------------------------
Phaser::Phaser (audioMasterCallback audioMaster)
: AudioEffectX (audioMaster, 1, kNumParams)	// 1 program, 1 parameter only
{
	setNumInputs (1);		// stereo in
	setNumOutputs (1);		// stereo out
	setUniqueID ('phas');	// identify
	canProcessReplacing ();	// supports replacing output
	canDoubleReplacing ();	// supports double precision processing

	_fb = 0.7;
	_lfoPhase = 0.f;
	_depth = 1.0;
	_zm1 = 0.f;
	
	num_stages = 12;
	
	Range(440.f, 1600.f);
	Rate(0.5);
	_rate = 0.5;
	
	vst_strncpy (programName, "Default", kVstMaxProgNameLen);	// default program name
}

//-------------------------------------------------------------------------------------------------------
Phaser::~Phaser ()
{
	// nothing to do here
}

//-------------------------------------------------------------------------------------------------------
void Phaser::Range(float fMin, float fMax){
	_dmin = fMin / (SR / 2.f);	
	_dmax = fMax / (SR / 2.f);
}

void Phaser::Rate(float rate){
	_lfoInc = 2.f * F_PI * (rate / SR);	
	_rate = rate;
}

void Phaser::Feedback(float fb){
	_fb = fb;	
}

void Phaser::Depth(float depth){
	_depth = depth;
}

float Phaser::Update(float inSamp){
	
	// update phaser sweep lfo
	float d = _dmin + (_dmax - _dmin) * ((sin(_lfoPhase) + 1.f) / 2.f);
	_lfoPhase += _lfoInc;
	if(_lfoPhase >= F_PI * 2.f)
		_lfoPhase -= F_PI * 2.f;
	
	//update filter coeffs
	for(int i = 0; i < num_stages; i++)
		_alps[i].Delay(d);
	
	// calculate output - converted this to a loop for cleaner code
	// unwrap later for speed?
	float y = inSamp + _zm1 * _fb;
	for(int i = 0; i < num_stages; i++){
		y = _alps[i].Update(y);
	}
	
	if(y > 1.0)
		y = 1.0;
	if(y < -1.0)
		y = -1.0;
	
	_zm1 = tanh(y); // some saturation right here?

	return inSamp + y * _depth;
}

void Phaser::NumStages(int num){
	if (num != num_stages){
		num_stages = num;
	}
}

void Phaser::setProgramName (char* name)
{
	vst_strncpy (programName, name, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Phaser::getProgramName (char* name)
{
	vst_strncpy (name, programName, kVstMaxProgNameLen);
}

//-----------------------------------------------------------------------------------------
void Phaser::setParameter (VstInt32 index, float value)
{
	switch (index){
		case kRate:
			Rate(value);	
			break;
		case kFeedBack:
			Feedback(value);
			break;
		case kDepth:
			Depth(value);
			break;
		case kNumStages:
			NumStages((int)(value * 128));
			break;
	}
}
//-----------------------------------------------------------------------------------------
float Phaser::getParameter (VstInt32 index)
{
	switch (index){
		case kRate:
			return _rate;	
		case kFeedBack:
			return _fb;
		case kDepth:
			return _depth;
		case kNumStages:
			return num_stages / 128.f;
			break;
	}
}

//-----------------------------------------------------------------------------------------
void Phaser::getParameterName (VstInt32 index, char* label)
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
void Phaser::getParameterDisplay (VstInt32 index, char* text)
{
	switch (index){
		case kRate:
			float2string(_rate, text, kVstMaxParamStrLen);	
			break;
		case kFeedBack:
			float2string(_fb, text, kVstMaxParamStrLen);
			break;
		case kDepth:
			float2string(_depth, text, kVstMaxParamStrLen);
			break;
		case kNumStages:
			int2string(num_stages, text, kVstMaxParamStrLen);
			break;
	}
}

//-----------------------------------------------------------------------------------------
void Phaser::getParameterLabel (VstInt32 index, char* label)
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
bool Phaser::getEffectName (char* name)
{
	vst_strncpy (name, "pucktronix.phaser", kVstMaxEffectNameLen);
	return true;
}

//------------------------------------------------------------------------
bool Phaser::getProductString (char* text)
{
	vst_strncpy (text, "phas", kVstMaxProductStrLen);
	return true;
}

//------------------------------------------------------------------------
bool Phaser::getVendorString (char* text)
{
	vst_strncpy (text, "pucktronix", kVstMaxVendorStrLen);
	return true;
}

//-----------------------------------------------------------------------------------------
VstInt32 Phaser::getVendorVersion ()
{ 
	return 1000; 
}

//-----------------------------------------------------------------------------------------
void Phaser::processReplacing (float** inputs, float** outputs, VstInt32 sampleFrames)
{
    float* in1  =  inputs[0];
    //float* in2  =  inputs[1];
    float* out1 = outputs[0];
    //float* out2 = outputs[1];

    while (--sampleFrames >= 0)
    {
     //   (*out1++) = (*in1++) * fGain;
     //   (*out2++) = (*in2++) * fGain;
		(*out1++) = Update(*in1++);
    }
}

//-----------------------------------------------------------------------------------------
void Phaser::processDoubleReplacing (double** inputs, double** outputs, VstInt32 sampleFrames)
{
    double* in1  =  inputs[0];
    //double* in2  =  inputs[1];
    double* out1 = outputs[0];
    //double* out2 = outputs[1];
	//	double dGain = fGain;

    while (--sampleFrames >= 0)
    {
		//   (*out1++) = (*in1++) * fGain;
		//   (*out2++) = (*in2++) * fGain;
		(*out1++) = Update(*in1++);
    }
}
