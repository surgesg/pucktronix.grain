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

#ifndef __phaser__
#define __phaser__

// #define SR 44100.f // update this to get SR from host
#define F_PI 3.14159f
#include "public.sdk/source/vst2.x/audioeffectx.h"

//-------------------------------------------------------------------------------------------------------
/* todo:
	mix an amount of noise in with lfo
	variable # of allpass stages?
*/
enum
{
	// Global
	kNumPrograms = 16,
	
	// Parameters Tags
	kRate = 0,
	kFeedBack,
	kDepth,
	kNumStages,
	kNumParams
};

class Phaser;

class AllpassDelay{
public:
	AllpassDelay();
	virtual void Delay(float delay);
	virtual float Update(float inSamp);
private:
	float _a1, _zm1;
};

class Phaser : public AudioEffectX
{
public:
	Phaser (audioMasterCallback audioMaster);
	~Phaser ();

	void Range(float fMin, float fMax);
	void Rate(float rate);
	void Feedback(float fb);
	void Depth(float depth);
	void NumStages(int num);
	float Update(float inSamp);
	bool ChangeStages;
	int SR;
	
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
	AllpassDelay _alps[256]; // dynamic
	
	float _dmin, _dmax;
	float _fb;
	float _lfoPhase;
	float _lfoInc;
	float _rate;
	float _depth;
	int num_stages;
	
	float _zm1;
	char programName[kVstMaxProgNameLen + 1];
	
};

#endif
