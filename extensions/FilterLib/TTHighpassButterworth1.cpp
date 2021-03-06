/* 
 * TTBlue 1st order Butterworth Highpass Filter Object
 * Copyright © 2008, Trond Lossius
 * 
 * License: This code is licensed under the terms of the GNU LGPL
 * http://www.gnu.org/licenses/lgpl.html 
 */

#include "TTHighpassButterworth1.h"

#define thisTTClass			TTHighpassButterworth1
#define thisTTClassName		"highpass.butterworth.1"
#define thisTTClassTags		"audio, processor, filter, highpass, butterworth"


TT_AUDIO_CONSTRUCTOR{
	// register attributes
	addAttributeWithSetter(Frequency,	kTypeFloat64);
	addAttributeProperty(Frequency,			range,			TTValue(10.0, sr*0.475));
	addAttributeProperty(Frequency,			rangeChecking,	TT("clip"));

	// register for notifications from the parent class so we can allocate memory as required
	addMessageWithArgument(updateMaxNumChannels);
	// register for notifications from the parent class so we can recalculate coefficients as required
	addMessage(updateSr);
	// make the clear method available to the outside world
	addMessage(Clear);

	// Set Defaults...
	setAttributeValue(TT("MaxNumChannels"),	arguments);			// This attribute is inherited
	setAttributeValue(TT("Frequency"),		1000.0);
	setProcessMethod(processAudio);
	setCalculateMethod(calculateValue);
}


TTHighpassButterworth1::~TTHighpassButterworth1()
{
	;
}


TTErr TTHighpassButterworth1::updateMaxNumChannels(const TTValue& oldMaxNumChannels)
{
	mX1.resize(maxNumChannels);
	mY1.resize(maxNumChannels);	
	Clear();
	return kTTErrNone;
}


TTErr TTHighpassButterworth1::updateSr()
{
	TTValue	v(mFrequency);
	return setFrequency(v);
}


TTErr TTHighpassButterworth1::Clear()
{
	mX1.assign(maxNumChannels, 0.0);
	mY1.assign(maxNumChannels, 0.0);
	return kTTErrNone;
}


TTErr TTHighpassButterworth1::setFrequency(const TTValue& newValue)
{	
	mFrequency = newValue;
	mRadians = kTTTwoPi*mFrequency;
	mK = mRadians/tan(kTTPi*mFrequency/sr);
	calculateCoefficients();
	return kTTErrNone;
}

	
	
void TTHighpassButterworth1::calculateCoefficients()
{
	mA0 = mK/(mK+mRadians); 
	mA1 = -mA0;//-k/(k+wc); 
	mB1 = (mRadians-mK)/(mK+mRadians);
}	


inline TTErr TTHighpassButterworth1::calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel)
{
	//y = TTAntiDenormal(mA0*x + mA1*mX1[channel] - mB1*mY1[channel]);
    //since mA1 = -mA0, we can simplyfiy to
	y = mA0*(x - mX1[channel]) - mB1*mY1[channel];
	TTZeroDenormal(y);
	mX1[channel] = x;
	mY1[channel] = y;
	return kTTErrNone;
}


TTErr TTHighpassButterworth1::processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TT_WRAP_CALCULATE_METHOD(calculateValue);
}


