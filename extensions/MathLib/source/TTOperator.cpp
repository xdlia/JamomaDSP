/* 
 * Jamoma DSP Operator Object
 * Copyright © 2008, Timothy Place
 * 
 * License: This code is licensed under the terms of the GNU LGPL
 * http://www.gnu.org/licenses/lgpl.html 
 */

#include "TTOperator.h"

#define thisTTClass			TTOperator
#define thisTTClassName		"operator"
#define thisTTClassTags		"audio, processor, math"


TT_AUDIO_CONSTRUCTOR
{
	addAttributeWithSetter(Operator, kTypeSymbol);
	addAttributeWithSetter(Operand, kTypeFloat64);
	
	// Set Defaults...
	setAttributeValue(TT("Operator"),	TT("+"));
	setAttributeValue(TT("Operand"),	1.0);
}


TTOperator::~TTOperator()
{
	;
}


TTErr TTOperator::setOperator(const TTValue& newValue)
{
	mOperator = newValue;
	if (mOperator == TT("+")) {
		setCalculateMethod(calculateAdd);
		setProcessMethod(processAdd);
	}
	else if (mOperator == TT("-")) {
		setCalculateMethod(calculateSubtract);
		setProcessMethod(processSubtract);
	}
	else if (mOperator == TT("*")) {
		setCalculateMethod(calculateMultiply);
		setProcessMethod(processMultiply);
	}
	else if (mOperator == TT("/")) {
		setCalculateMethod(calculateDivide);
		setProcessMethod(processDivide);
	}
	else if (mOperator == TT("%")) {
		setCalculateMethod(calculateModulo);
		setProcessMethod(processModulo);
	}
	else
		return kTTErrInvalidValue;
	
	return kTTErrNone;
}


TTErr TTOperator::setOperand(const TTValue& newValue)
{	
	TTInt64		i;
	TTUInt32	count;
	
	mOperand = newValue;
	mOperandIsPowerOfTwo = false;
	
	i = (TTInt64)mOperand;
	if (i - mOperand)
		mOperandIsInteger = false;
	else
		mOperandIsInteger = true;
	
	if (mOperandIsInteger) {
		for (count = 0; i != 0; count++)
			i &= i-1;
		if (count == 1)
			mOperandIsPowerOfTwo = true;		
	}
	return kTTErrNone;
}


TTErr TTOperator::calculateAdd(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data)
{
	y = x + mOperand;
	return kTTErrNone;
}

TTErr TTOperator::calculateSubtract(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data)
{
	y = x - mOperand;
	return kTTErrNone;
}

TTErr TTOperator::calculateMultiply(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data)
{
	y = x * mOperand;
	return kTTErrNone;
}

TTErr TTOperator::calculateDivide(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data)
{
	y = x / mOperand;
	return kTTErrNone;
}

TTErr TTOperator::calculateModulo(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt data)
{
	TTUInt64 operander = (TTUInt64)mOperand;
	
	y = fmod(x, operander);	
	return kTTErrNone;
}



TTErr TTOperator::processAdd(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	if (inputs->numAudioSignals != 1)
		return processAddSignal(inputs, outputs);
	else {
		TT_WRAP_CALCULATE_METHOD(calculateAdd);
	}
}

TTErr TTOperator::processAddSignal(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TTAudioSignal&		in1 = inputs->getSignal(0);
	TTAudioSignal&		in2 = inputs->getSignal(1);
	TTAudioSignal&		out = outputs->getSignal(0);
	TTUInt16			vs;
	TTSampleValuePtr	in1Sample;
	TTSampleValuePtr	in2Sample;
	TTSampleValuePtr	outSample;
	TTUInt16			numChannels;
	TTUInt16			channel;
	
	if (in2.getNumChannelsAsInt() == 1) {				// If the operand signal is one only channel, then we apply that to all channels of in1
		numChannels = in2.getNumChannelsAsInt();
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[0];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ + *in2Sample++;
		}		
	}
	else {										// Otherwise we apply channel 1 to channel 1, channel 2 to channel 2, etc.
		numChannels = TTAudioSignal::getMinChannelCount(in1, out);
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[channel];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ + *in2Sample++;
		}
	}
	return kTTErrNone;
}



TTErr TTOperator::processSubtract(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	if (inputs->numAudioSignals != 1)
		return processSubtractSignal(inputs, outputs);
	else {
		TT_WRAP_CALCULATE_METHOD(calculateSubtract);
	}
}

TTErr TTOperator::processSubtractSignal(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TTAudioSignal&		in1 = inputs->getSignal(0);
	TTAudioSignal&		in2 = inputs->getSignal(1);
	TTAudioSignal&		out = outputs->getSignal(0);
	TTUInt16			vs;
	TTSampleValuePtr	in1Sample;
	TTSampleValuePtr	in2Sample;
	TTSampleValuePtr	outSample;
	TTUInt16			numChannels;
	TTUInt16			channel;
	
	if (in2.getNumChannelsAsInt() == 1) {				// If the operand signal is one only channel, then we apply that to all channels of in1
		numChannels = in2.getNumChannelsAsInt();
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[0];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ - *in2Sample++;
		}		
	}
	else {										// Otherwise we apply channel 1 to channel 1, channel 2 to channel 2, etc.
		numChannels = TTAudioSignal::getMinChannelCount(in1, out);
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[channel];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ - *in2Sample++;
		}
	}
	return kTTErrNone;
}



TTErr TTOperator::processMultiply(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	if (inputs->numAudioSignals != 1)
		return processMultiplySignal(inputs, outputs);
	else {
		TT_WRAP_CALCULATE_METHOD(calculateMultiply);
	}
}

TTErr TTOperator::processMultiplySignal(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TTAudioSignal&		in1 = inputs->getSignal(0);
	TTAudioSignal&		in2 = inputs->getSignal(1);
	TTAudioSignal&		out = outputs->getSignal(0);
	TTUInt16			vs;
	TTSampleValuePtr	in1Sample;
	TTSampleValuePtr	in2Sample;
	TTSampleValuePtr	outSample;
	TTUInt16			numChannels;
	TTUInt16			channel;
	
	if (in2.getNumChannelsAsInt() == 1) {				// If the operand signal is one only channel, then we apply that to all channels of in1
		numChannels = in2.getNumChannelsAsInt();
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[0];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ * *in2Sample++;
		}		
	}
	else {										// Otherwise we apply channel 1 to channel 1, channel 2 to channel 2, etc.
		numChannels = TTAudioSignal::getMinChannelCount(in1, out);
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[channel];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ * *in2Sample++;
		}
	}
	return kTTErrNone;
}



TTErr TTOperator::processDivide(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	if (inputs->numAudioSignals != 1)
		return processDivideSignal(inputs, outputs);
	else {
		TT_WRAP_CALCULATE_METHOD(calculateDivide);
	}
}


TTErr TTOperator::processDivideSignal(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TTAudioSignal&		in1 = inputs->getSignal(0);
	TTAudioSignal&		in2 = inputs->getSignal(1);
	TTAudioSignal&		out = outputs->getSignal(0);
	TTUInt16			vs;
	TTSampleValuePtr	in1Sample;
	TTSampleValuePtr	in2Sample;
	TTSampleValuePtr	outSample;
	TTUInt16			numChannels;
	TTUInt16			channel;
	
	if (in2.getNumChannelsAsInt() == 1) {				// If the operand signal is one only channel, then we apply that to all channels of in1
		numChannels = in2.getNumChannelsAsInt();
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[0];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ / *in2Sample++;
		}		
	}
	else {										// Otherwise we apply channel 1 to channel 1, channel 2 to channel 2, etc.
		numChannels = TTAudioSignal::getMinChannelCount(in1, out);
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[channel];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			while (vs--)
				*outSample++ = *in1Sample++ / *in2Sample++;
		}
	}
	return kTTErrNone;
}




TTErr TTOperator::processModulo(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	if (inputs->numAudioSignals != 1)
		return processModuloSignal(inputs, outputs);
	else {
		TT_WRAP_CALCULATE_METHOD(calculateModulo);
	}
}

TTErr TTOperator::processModuloSignal(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs)
{
	TTAudioSignal&		in1 = inputs->getSignal(0);
	TTAudioSignal&		in2 = inputs->getSignal(1);
	TTAudioSignal&		out = outputs->getSignal(0);
	TTUInt16			vs;
	TTSampleValuePtr	in1Sample;
	TTSampleValuePtr	in2Sample;
	TTSampleValuePtr	outSample;
	TTUInt16			numChannels;
	TTUInt16			channel;
	TTUInt64			temp;
	TTUInt64			intOperand;
	
	if (in2.getNumChannelsAsInt() == 1) {				// If the operand signal is one only channel, then we apply that to all channels of in1
		numChannels = in2.getNumChannelsAsInt();
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[0];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			
			if (mOperandIsPowerOfTwo) {
				while (vs--) {
					temp = TTUInt64(*in1Sample++);
					intOperand = TTUInt64(*in2Sample++);
					*outSample++ = TTSampleValue(temp & intOperand);
				}
			}
			else {
				while (vs--)
					*outSample++ = fmod(*in1Sample++, *in2Sample++);		
			}
		}		
	}
	else {										// Otherwise we apply channel 1 to channel 1, channel 2 to channel 2, etc.
		numChannels = TTAudioSignal::getMinChannelCount(in1, out);
		for (channel=0; channel<numChannels; channel++) {
			in1Sample = in1.mSampleVectors[channel];
			in2Sample = in2.mSampleVectors[channel];
			outSample = out.mSampleVectors[channel];
			vs = in1.getVectorSizeAsInt();
			
			if (mOperandIsPowerOfTwo) {
				while (vs--) {
					temp = TTUInt64(*in1Sample++);
					intOperand = TTUInt64(*in2Sample++);
					*outSample++ = TTSampleValue(temp & intOperand);
				}
			}
			else {
				while (vs--)
					*outSample++ = fmod(*in1Sample++, *in2Sample++);		
			}
		}
	}
	return kTTErrNone;
}
