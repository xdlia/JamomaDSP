/* 
 * TTMatrix Object
 * Copyright © 2010, Timothy Place
 * 
 * License: This code is licensed under the terms of the GNU LGPL
 * http://www.gnu.org/licenses/lgpl.html 
 */

#include "TTMatrix.h"
#ifdef TT_PLATFORM_WIN
#include <algorithm>
#endif

#define thisTTClass			TTMatrix
#define thisTTClassName		"matrix"
#define thisTTClassTags		"audio, matrix"


TT_AUDIO_CONSTRUCTOR,
	mNumInputs(0),
	mNumOutputs(0)
{
	addAttributeWithSetter(NumInputs, kTypeUInt16);
	addAttributeWithSetter(NumOutputs, kTypeUInt16);
	
	addMessageWithArgument(setGain);
	addMessageWithArgument(setLinearGain);
	addMessageWithArgument(setMidiGain);
	//registerMessageWithArgument(updateMaxNumChannels);
	addMessage(clear);	

	setProcessMethod(processAudio);
}


TTMatrix::~TTMatrix()
{
	;
}


// conceptually:
//	columns == inputs
//	rows == outputs


TTErr TTMatrix::setNumInputs(const TTValue& newValue)
{
	TTUInt16 numInputs = newValue;
	
	if (numInputs != mNumInputs) {
		mNumInputs = numInputs;
		mGainMatrix.resize(mNumInputs);
		for_each(mGainMatrix.begin(), mGainMatrix.end(), bind2nd(mem_fun_ref(&TTSampleMatrix::value_type::resize), mNumOutputs));
	}
	return kTTErrNone;
}


TTErr TTMatrix::setNumOutputs(const TTValue& newValue)
{
	TTUInt16 numOutputs = newValue;
	
	if (numOutputs != mNumOutputs) {
		mNumOutputs = numOutputs;
		for_each(mGainMatrix.begin(), mGainMatrix.end(), bind2nd(mem_fun_ref(&TTSampleMatrix::value_type::resize), mNumOutputs));
	}
	return kTTErrNone;
}


TTErr TTMatrix::clear()
{
	for (TTSampleMatrixIter column = mGainMatrix.begin(); column != mGainMatrix.end(); column++)
		column->assign(mNumOutputs, 0.0);
	return kTTErrNone;
}


TTErr TTMatrix::setGain(const TTValue& newValue)
{
	TTUInt16	x;
	TTUInt16	y;
	TTFloat64	gainValue;
	
	if (newValue.getSize() != 3)
		return kTTErrWrongNumValues;
	
	newValue.get(0, x);
	newValue.get(1, y);
	newValue.get(2, gainValue);
	mGainMatrix[x][y] = dbToLinear(gainValue);
	return kTTErrNone;
}


TTErr TTMatrix::setLinearGain(const TTValue& newValue)
{
	TTUInt16	x;
	TTUInt16	y;
	TTFloat64	gainValue;
	
	if (newValue.getSize() != 3)
		return kTTErrWrongNumValues;
	
	newValue.get(0, x);
	newValue.get(1, y);
	newValue.get(2, gainValue);
	mGainMatrix[x][y] = gainValue;
	return kTTErrNone;
}


TTErr TTMatrix::setMidiGain(const TTValue& newValue)
{
	TTUInt16	x;
	TTUInt16	y;
	TTFloat64	gainValue;
	
	if (newValue.getSize() != 3)
		return kTTErrWrongNumValues;
	
	newValue.get(0, x);
	newValue.get(1, y);
	newValue.get(2, gainValue);
	mGainMatrix[x][y] = midiToLinearGain(gainValue);
	return kTTErrNone;
}


// Here we are mixing channels within a signal rather than between multiple signals
TTErr TTMatrix::processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	
	TTAudioSignal&		in = inputs->getSignal(0);
	TTAudioSignal&		out = outputs->getSignal(0);
	TTUInt16			vs = in.getVectorSize();
	TTSampleValuePtr	inSample;
	TTSampleValuePtr	outSample;
	TTUInt16			numInputChannels = in.getNumChannels();
	TTUInt16			numOutputChannels = out.getNumChannels();
	TTUInt16			outChannel;
	TTUInt16			inChannel;

	if (numInputChannels != mNumInputs) {
		setNumInputs(numInputChannels);
	}
	if (numOutputChannels != mNumOutputs) {
		TTValue v = mNumOutputs;

		out.setmaxNumChannels(v);
		out.setnumChannels(v);
		numOutputChannels = mNumOutputs;
	}
	
	out.clear();
	
	// TODO: this multiply-nested for-loop has got to be horrendously slow, there should be a much faster way to do this?
	for (int i=0; i<vs; i++) {
		for (outChannel=0; outChannel<numOutputChannels; outChannel++) {
			for (inChannel=0; inChannel<numInputChannels; inChannel++) {
				inSample = in.sampleVectors[inChannel];
				outSample = out.sampleVectors[outChannel];
				outSample[i] += inSample[i] * mGainMatrix[inChannel][outChannel];
			}
		}
	}		
	return kTTErrNone;
}
