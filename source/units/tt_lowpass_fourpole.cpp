#include "tt_lowpass_fourpole.h"


// OBJECT LIFE					
TT_INLINE 
tt_lowpass_fourpole::tt_lowpass_fourpole()								// Constructor		
{
	set_attr(k_frequency, 4000.0);	
	set_attr(k_resonance, 4.0);	
	clear();
}

TT_INLINE 
tt_lowpass_fourpole::~tt_lowpass_fourpole()								// Destructor
{
	;
}


// ATTRIBUTES
TT_INLINE 
tt_err tt_lowpass_fourpole::set_attr(tt_selector sel, const tt_value &a)		// Set Attributes
{
	switch (sel){			
		case k_frequency:
			frequency = a;
			c = (frequency * 2.0) / sr;
			f = c * 1.16;
			fb = res * (1.0 - 0.15 * (f * f));
			break;
		case k_resonance:
			resonance = a;
			res = 4.0 * (resonance * 0.1);
			fb = res * (1.0 - 0.15 * (f * f)); 			
			break;
		default:
			return TT_ERR_ATTR_INVALID;
	}
	return TT_ERR_NONE;
}

TT_INLINE 
tt_err tt_lowpass_fourpole::get_attr(tt_selector sel, tt_value &a)				// Get Attributes
{
	switch (sel){
		case k_frequency:
			a = frequency;
			break;
		case k_resonance:
			a = resonance;
			break;
		default:
			return TT_ERR_ATTR_INVALID;
	}
	return TT_ERR_NONE;
}


// DSP LOOP
TT_INLINE void tt_lowpass_fourpole::dsp_vector_calc(tt_audio_signal *input, tt_audio_signal *output)
{
	tt_sample_value temp;
	temp_vs = input->vectorsize;
	
	while(temp_vs--){
		temp = *input->vector++;
		
		temp -= anti_denormal(out[3] * fb); 
		temp *= anti_denormal(0.35013 * ((f * f) * (f * f))); 
		out[0] = anti_denormal(temp + 0.3 * in[0] + (1 - f) * out[0]); 
		in[0] = temp; 
		out[1] = anti_denormal(out[0] + 0.3 * in[1] + (1 - f) * out[1]); 
		in[1] = out[0]; 
		out[2] = anti_denormal(out[1] + 0.3 * in[2] + (1 - f) * out[2]); 
		in[2] = out[1]; 
		out[3] = anti_denormal(out[2] + 0.3 * in[3] + (1 - f) * out[3]); 
		in[3] = out[2]; 
		temp = out[3];
						
		*output->vector++ = temp;
	}
	input->reset(); output->reset();
}


// clear
TT_INLINE void tt_lowpass_fourpole::clear()
{
	short i;
	
	for(i=0; i < num_poles; i++){
		in[i] = 0.0;
		out[i] = 0.0;
	}
}
