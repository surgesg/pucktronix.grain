/*
 *  phaser_editor.cpp
 *  vst 2.4 examples
 *
 *  Created by Greg Surges on 11/18/11.
 *  Copyright 2011 Pucktronix. All rights reserved.
 *
 */

#include "phaser_editor.h"
#include "phaser.h"
#include <stdio.h>

/* resource ids */
enum {
	//bitmaps
	kBackgroundId = 128,
	kFaderBodyId,
	kFaderHandleId,
	
	// positions
	kFaderX = 41,
	kFaderY = 155,
	
	kFaderInc = 30,
	
	kDisplayX = 10,
	kDisplayY = 183,
	kDisplayXWidth = 30,
	kDisplayHeight = 14
};

// prototype string convert float -> percent
void percentStringConvert (float value, char* string);
void percentStringConvert (float value, char* string)
{
	sprintf (string, "%d%%", (int)(100 * value + 0.5f));
}

PhaserEditor::PhaserEditor(AudioEffect *effect) : AEffGUIEditor (effect)
{
	rateFader = 0;
	feedbackFader = 0;
	depthFader = 0;
	numStagesFader = 0;

	rateDisplay = 0;
	feedbackDisplay = 0;
	depthDisplay = 0;
	numStagesDisplay = 0;

	hBackground = new CBitmap (kBackgroundId);

	// init size of plugin window
	rect.left = 0;
	rect.top = 0;
	rect.right = (short)hBackground->getWidth();
	rect.bottom = (short)hBackground->getHeight();
}

PhaserEditor::~PhaserEditor(){
	if(hBackground)
		hBackground->forget();
	hBackground = 0;
}


bool PhaserEditor::open (void *ptr)
{
	// !!! always call this !!!
	AEffGUIEditor::open (ptr);
	
	//--load some bitmaps
	CBitmap* hFaderBody   = new CBitmap (kFaderBodyId);
	CBitmap* hFaderHandle = new CBitmap (kFaderHandleId);
	
	//--init background frame-----------------------------------------------
	// We use a local CFrame object so that calls to setParameter won't call into objects which may not exist yet. 
	// If all GUI objects are created we assign our class member to this one. See bottom of this method.
	CRect size (0, 0, hBackground->getWidth (), hBackground->getHeight ());
	CFrame* lFrame = new CFrame (size, ptr, this);
	lFrame->setBackground (hBackground);
	
	//--init the faders------------------------------------------------
	int minPos = kFaderX;
	int maxPos = kFaderX + hFaderBody->getWidth () - hFaderHandle->getWidth () - 1;
	CPoint point (0, 0);
	CPoint offset (1, 0);
	
	// Rate
	size (kFaderX, kFaderY,
          kFaderX + hFaderBody->getWidth (), kFaderY + hFaderBody->getHeight ());
	rateFader = new CHorizontalSlider (size, this, kRate, minPos, maxPos, hFaderHandle, hFaderBody, point);
	rateFader->setOffsetHandle (offset);
	rateFader->setValue (effect->getParameter (kRate));
	lFrame->addView (rateFader);
	
	// FeedBack
	size.offset (0, kFaderInc + hFaderBody->getHeight ());
	feedbackFader = new CHorizontalSlider (size, this, kFeedBack, minPos, maxPos, hFaderHandle, hFaderBody, point);
	feedbackFader->setOffsetHandle (offset);
	feedbackFader->setValue (effect->getParameter (kFeedBack));
	lFrame->addView (feedbackFader);
	
	// Depth
	size.offset (0, kFaderInc + hFaderBody->getHeight ());
	depthFader = new CHorizontalSlider (size, this, kDepth, minPos, maxPos, hFaderHandle, hFaderBody, point);
	depthFader->setOffsetHandle (offset);
	depthFader->setValue (effect->getParameter (kDepth));
	lFrame->addView (depthFader);
	
	// Num Stages
	size.offset (0, kFaderInc + hFaderBody->getHeight ());
	numStagesFader = new CHorizontalSlider (size, this, kNumStages, minPos, maxPos, hFaderHandle, hFaderBody, point);
	numStagesFader->setOffsetHandle (offset);
	numStagesFader->setValue (effect->getParameter (kNumStages));
	lFrame->addView (numStagesFader);
	
	//--init the display------------------------------------------------
	// Rate
	size (kDisplayX, kDisplayY,
          kDisplayX + kDisplayXWidth, kDisplayY + kDisplayHeight);
	rateDisplay = new CParamDisplay (size, 0, kCenterText);
	rateDisplay->setFont (kNormalFontSmall);
	rateDisplay->setFontColor (kWhiteCColor);
	rateDisplay->setBackColor (kBlackCColor);
	rateDisplay->setFrameColor (kBlueCColor);
	rateDisplay->setValue (effect->getParameter (kRate));
	lFrame->addView (rateDisplay);
	
	// FeedBack
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	feedbackDisplay = new CParamDisplay (size, 0, kCenterText);
	feedbackDisplay->setFont (kNormalFontSmall);
	feedbackDisplay->setFontColor (kWhiteCColor);
	feedbackDisplay->setBackColor (kBlackCColor);
	feedbackDisplay->setFrameColor (kBlueCColor);
	feedbackDisplay->setValue (effect->getParameter (kFeedBack));
	feedbackDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (feedbackDisplay);
	
	// Depth
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	depthDisplay = new CParamDisplay (size, 0, kCenterText);
	depthDisplay->setFont (kNormalFontSmall);
	depthDisplay->setFontColor (kWhiteCColor);
	depthDisplay->setBackColor (kBlackCColor);
	depthDisplay->setFrameColor (kBlueCColor);
	depthDisplay->setValue (effect->getParameter (kDepth));
	depthDisplay->setStringConvert (percentStringConvert);
	lFrame->addView (depthDisplay);

	// Num Stages
	size.offset (kFaderInc + hFaderBody->getWidth (), 0);
	numStagesDisplay = new CParamDisplay (size, 0, kCenterText);
	numStagesDisplay->setFont (kNormalFontSmall);
	numStagesDisplay->setFontColor (kWhiteCColor);
	numStagesDisplay->setBackColor (kBlackCColor);
	numStagesDisplay->setFrameColor (kBlueCColor);
	numStagesDisplay->setValue (effect->getParameter (kNumStages));
	lFrame->addView (numStagesDisplay);
	
	hFaderBody->forget ();
	hFaderHandle->forget ();
	
	frame = lFrame;
	return true;
}

//-----------------------------------------------------------------------------
void PhaserEditor::close ()
{
	delete frame;
	frame = 0;
}

//-----------------------------------------------------------------------------
void PhaserEditor::setParameter (VstInt32 index, float value)
{
	if (frame == 0)
		return;
	switch (index)
	{
		case kRate:
			if (rateFader)
				rateFader->setValue (effect->getParameter (index));
			if (rateDisplay)
				rateDisplay->setValue (effect->getParameter (index));
			break;
			
		case kFeedBack:
			if (feedbackFader)
				feedbackFader->setValue (effect->getParameter (index));
			if (feedbackDisplay)
				feedbackDisplay->setValue (effect->getParameter (index));
			break;
			
		case kDepth:
			if (depthFader)
				depthFader->setValue (effect->getParameter (index));
			if (depthDisplay)
				depthDisplay->setValue (effect->getParameter (index));
			break;
		case kNumStages:
			if (numStagesFader)
				numStagesFader->setValue (effect->getParameter (index));
			if (numStagesDisplay)
				numStagesDisplay->setValue (effect->getParameter (index));
			break;
	}
}

//-----------------------------------------------------------------------------
void PhaserEditor::valueChanged (CDrawContext* context, CControl* control)
{
	long tag = control->getTag ();
		effect->setParameter (tag, control->getValue ());
		control->setDirty();
}


void PhaserEditor::idle()
{
    AEffGUIEditor::idle();
	
}



