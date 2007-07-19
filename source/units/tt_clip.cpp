#include "tt_clip.h"


// OBJECT LIFE					
TT_INLINE 
tt_clip::tt_clip(void)										// Constructor		
{
	set_attr(k_clip_high, 1.0);
	set_attr(k_clip_low, -1.0);
}

TT_INLINE 
tt_clip::~tt_clip(void)										// Destructor
{
	;
}

// use the generic c++ copy constructor
// tt_clip operator = (const tt_add &tt_clip_source)


// ATTRIBUTES
TT_INLINE 
tt_err tt_clip::set_attr(tt_selector sel, const tt_value &a)	// Set Attributes
{
	tt_float32 val = a;
	
	switch (sel){
		case k_clip:
			clip_high_value = decibels_to_amplitude(val);
			clip_low_value = clip_high_value * -1.0;
			break;
		case k_clip_high:
			clip_high_value = val;
			break;
		case k_clip_low:
			clip_low_value = val;
			break;
		default:
			return TT_ERR_ATTR_INVALID;
	}
	return TT_ERR_NONE;
}

TT_INLINE 
tt_err tt_clip::get_attr(tt_selector sel, tt_value &a)				// Get Attributes
{
	switch (sel){
		case k_clip:		// fall through
		case k_clip_high:
			a = clip_high_value;
			break;
		case k_clip_low:
			a = clip_low_value;
			break;
		default:
			return TT_ERR_ATTR_INVALID;
	}
	return TT_ERR_NONE;
}
		

// DSP LOOP
TT_INLINE void tt_clip::dsp_vector_calc(tt_audio_signal *in, tt_audio_signal *out)
{
	temp_vs = in->vectorsize;		
	while(temp_vs--)
		*out->vector++ = clip(*in->vector++, clip_low_value, clip_high_value);
	in->reset(); out->reset();
}

// DSP LOOP
TT_INLINE void tt_clip::dsp_vector_calc(tt_audio_signal *in1, tt_audio_signal *in2, tt_audio_signal *out1, tt_audio_signal *out2)
{
	temp_vs = in1->vectorsize;		
	while(temp_vs--){
		*out1->vector++ = clip(*in1->vector++, clip_low_value, clip_high_value);
		*out2->vector++ = clip(*in2->vector++, clip_low_value, clip_high_value);
	}
	in1->reset(); in2->reset(); out1->reset(); out2->reset();
}