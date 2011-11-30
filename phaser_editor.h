/*
 *  phaser_editor.h
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 11/18/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#ifndef __phaser__editor__
#define __phaser__editor__


// include VSTGUI
#ifndef __vstgui__
#include "vstgui.sf/vstgui/vstgui.h"
#endif

//-----------------------------------------------------------------------------
class PhaserEditor : public AEffGUIEditor, public CControlListener
{
public:
	PhaserEditor (AudioEffect* effect);
	virtual ~PhaserEditor ();
	
public:
	virtual bool open (void* ptr);
	virtual void close ();
	virtual void idle();
	virtual void setParameter (VstInt32 index, float value);
	virtual void valueChanged (CDrawContext* context, CControl* control);
	
private:
	// Controls
	CHorizontalSlider* rateFader;
	CHorizontalSlider* feedbackFader;
	CHorizontalSlider* depthFader;
	CHorizontalSlider* numStagesFader;
	
	CParamDisplay* rateDisplay;
	CParamDisplay* feedbackDisplay;
	CParamDisplay* depthDisplay;
	CParamDisplay* numStagesDisplay;
	
	// Bitmap
	CBitmap* hBackground;
};

#endif
