/* 
 * TTBlue 3rd order Butterworth Lowpass Filter Object
 * Copyright © 2008, Trond Lossius
 * 
 * License: This code is licensed under the terms of the GNU LGPL
 * http://www.gnu.org/licenses/lgpl.html 
 */

#ifndef __TT_LOWPASS_BUTTERWORTH_3_H__
#define __TT_LOWPASS_BUTTERWORTH_3_H__

#include "TTDSP.h"


/**	3rd order Butterworth lowpass filter; Butterworth filters have maximum flat frequency response in the pass band.
 *
 * Filter equations from:
 * 
 * Second-order IIR Filters will support cascade implementations
 * By Rusty Allred, Texas Instruments, Dallas
 * July 01, 2003
 * http://www.planetanalog.com/article/printableArticle.jhtml?articleID=12802683
 *
 */
class TTLowpassButterworth3 : public TTAudioObject {
	TTCLASS_SETUP(TTLowpassButterworth3)

	TTFloat64			mFrequency;				///< filter cutoff frequency
	TTFloat64			mK,mKSquared,mKCubic, mRadians, mRadiansSquared, mRadiansCubic;		///< filter coefficients
	TTFloat64			mA0, mA1, mA2, mA3;			///< filter coefficients for input samples
	TTFloat64			mB1, mB2, mB3;				///< filter coefficients for output samples
	TTSampleVector		mX1;						///< Input sample n-1
	TTSampleVector		mX2;						///< Input sample n-2
	TTSampleVector		mX3;						///< Input sample n-3
	TTSampleVector		mY1;						///< Output sample n-1
	TTSampleVector		mY2;						///< Output sample n-2
	TTSampleVector		mY3;						///< Output sample n-3
	
	
	/**	Receives notifications when there are changes to the inherited 
	 maxNumChannels attribute.  This allocates memory for xm1, xm2, ym1, and ym2 
	 so that each channel's previous values are remembered.		*/
	TTErr updateMaxNumChannels(const TTValue& oldMaxNumChannels);
	TTErr updateSr();
	TTErr Clear();
	
	void calculateCoefficients();
	
    /**	Standard single value calculate method as used by DSP objects. */
	inline TTErr calculateValue(const TTFloat64& x, TTFloat64& y, TTPtrSizedInt channel);
	
	/**	Standard audio processing method as used by TTBlue objects. */
	TTErr processAudio(TTAudioSignalArrayPtr inputs, TTAudioSignalArrayPtr outputs);
	
	/**	Setter for the frequency attribute. */
	TTErr setFrequency(const TTValue& value);
	
	/**	This algorithm uses an IIR filter, meaning that it relies on feedback.  If the filter should
	 *	not be producing any signal (such as turning audio off and then back on in a host) or if the
	 *	feedback has become corrupted (such as might happen if a NaN is fed in) then it may be 
	 *	neccesary to clear the filter by calling this method.
	 *	@return Returns a TTErr error code.												*/
	
};



#endif // __TT_LOWPASS_BUTTERWORTH_3_H__
