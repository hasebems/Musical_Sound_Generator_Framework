//
//  vas_voice_context.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __ToneGenerator__vas_voice_context__
#define __ToneGenerator__vas_voice_context__

#include <iostream>
#include "msgf_voice_context.h"
#include "msgf_filter.h"
#include "msgf_amplitude.h"
#include "msgf_oscillator.h"

//---------------------------------------------------------
struct VasVoiceParameter {

	int		amp[msgf::VP_AMPLITUDE_MAX];
	int		osc[msgf::VP_OSCILLATOR_MAX];
	int		flt[msgf::VP_FILTER_MAX];
};
//---------------------------------------------------------
class VasVoiceContext : public msgf::VoiceContext {
	
public:
	int		getParameter( int tid, int pid )
	{
		int	value = 0;
		switch (tid){
			case msgf::VP_AMPLITUDE_ID: value = tVasVoiceParameter.amp[pid]; break;
			case msgf::VP_FILTER_ID:	value = tVasVoiceParameter.flt[pid]; break;
			case msgf::VP_OSCILLATOR_ID:value = tVasVoiceParameter.osc[pid]; break;
			default: break;
		}
		return value;
	}

private:
	static const VasVoiceParameter	tVasVoiceParameter;

};
#endif /* defined(__ToneGenerator__vas_voice_context__) */
