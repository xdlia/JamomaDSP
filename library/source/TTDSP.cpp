/* 
 * TTBlue Library
 * Copyright © 2008, Timothy Place
 * 
 * License: This code is licensed under the terms of the GNU LGPL
 * http://www.gnu.org/licenses/lgpl.html 
 */


#include "TTDSP.h"
#include "TTAudioEngine.h"


static bool TTDSPHasInitialized = false;


void TTDSPRegisterInternalClasses();


/****************************************************************************************************/

void TTDSPInit()
{
	TTFoundationInit();
	
	if(!TTDSPHasInitialized){
		TTDSPHasInitialized = true;
				
		// TODO: someday implement these so that we have project-scoped caches and don't stuff everything into the foundation?
		//TTDSPSymbolCacheInit();
		//TTDSPValueCacheInit();
		
#ifdef TT_DEBUG
		TTLogMessage("JamomaDSP -- Version %s -- Debugging Enabled\n", TTDSP_VERSION_STRING);
#else
		TTLogMessage("JamomaDSP -- Version %s\n", TTDSP_VERSION_STRING);
#endif
		
		TTDSPRegisterInternalClasses();
		TTAudioEngineCreate();
	}
}


// FIXME: this is never called right now!
void TTDSPShutdown()
{
	TTAudioEngineFree();
}


/****************************************************************************************************/

// Core
#include "TTAudioSignal.h"

// Analysis
#include "TTZerocross.h"

// Generators
#include "TTAdsr.h"
#include "TTNoise.h"
#include "TTPhasor.h"
#include "TTRamp.h"
#include "TTWavetable.h"

// Misc
#include "TTBuffer.h"
#include "TTCrossfade.h"
#include "TTOperator.h"
#include "TTMatrixMixer.h"


void TTDSPRegisterInternalClasses()
{
	TTAudioSignal::registerClass();
	TTAudioSignalArray::registerClass();
	TTBuffer::registerClass();
	
	// Analysis
	TTZerocross::registerClass();
		
	// Generators
	TTAdsr::registerClass();
	TTNoise::registerClass();
	TTPhasor::registerClass();
	TTRamp::registerClass();
	TTWavetable::registerClass();
		
	// Misc
	TTCrossfade::registerClass();
	TTOperator::registerClass();
	TTMatrixMixer::registerClass();
}


/****************************************************************************************************/

TTErr TTObjectInstantiate(const TTSymbolPtr className, TTAudioObjectPtr* returnedObjectPtr, TTValue& arguments)
{
	return ttEnvironment->createInstance(className, (TTObjectPtr*)returnedObjectPtr, arguments);
}

TTErr TTObjectInstantiate(const TTSymbolPtr className, TTAudioSignalPtr* returnedObjectPtr, TTValue& arguments)
{
	return ttEnvironment->createInstance(className, (TTObjectPtr*)returnedObjectPtr, arguments);
}


TTErr TTObjectInstantiate(const TTSymbolPtr className, TTAudioObjectPtr* returnedObjectPtr, const TTValue& arguments)
{
	return ttEnvironment->createInstance(className, (TTObjectPtr*)returnedObjectPtr, arguments);
}

TTErr TTObjectInstantiate(const TTSymbolPtr className, TTAudioSignalPtr* returnedObjectPtr, const TTValue& arguments)
{
	return ttEnvironment->createInstance(className, (TTObjectPtr*)returnedObjectPtr, arguments);
}


TTErr TTObjectInstantiate(const TTSymbolPtr className, TTAudioObjectPtr* returnedObjectPtr, const TTUInt16 arguments)
{
	TTValue	v(arguments);
	return ttEnvironment->createInstance(className, (TTObjectPtr*)returnedObjectPtr, v);
}

TTErr TTObjectInstantiate(const TTSymbolPtr className, TTAudioSignalPtr* returnedObjectPtr, const TTUInt16 arguments)
{
	TTValue	v(arguments);
	return ttEnvironment->createInstance(className, (TTObjectPtr*)returnedObjectPtr, v);
}


TTAudioObjectPtr TTObjectReference(TTAudioObjectPtr anObject)
{
	return (TTAudioObjectPtr)ttEnvironment->referenceInstance((TTObjectPtr)anObject);
}

TTAudioSignalPtr TTObjectReference(TTAudioSignalPtr anObject)
{
	return (TTAudioSignalPtr)ttEnvironment->referenceInstance((TTObjectPtr)anObject);
}


TTErr TTObjectRelease(TTAudioObjectPtr* anObject)
{
	if(*anObject)
		return ttEnvironment->releaseInstance((TTObjectPtr*)anObject);
	else
		return kTTErrNone;
}

TTErr TTObjectRelease(TTAudioSignalPtr* anObject)
{
	if(*anObject)
		return ttEnvironment->releaseInstance((TTObjectPtr*)anObject);
	else
		return kTTErrNone;
}
