//
//  vas_voice_context.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __vas_voice_context__
#define __vas_voice_context__

#include <iostream>
#include "msgf_voice_context.h"
#include "msgf_iirfilter.h"
#include "msgf_amp4seg.h"
#include "msgf_osc_synth.h"

//---------------------------------------------------------
struct VasVoiceParameter {

	int		amp[msgf::VP_AMPLITUDE_MAX];
	int		osc[msgf::VP_OSCILLATOR_MAX];
	int		flt[msgf::VP_FILTER_MAX];
};
//---------------------------------------------------------
class VasVoiceContext : public msgf::VoiceContext {
	
public:
	VasVoiceContext( int voiceNumber ):
		_vNum(voiceNumber){}

	int		getParameter( int tid, int pid )
	{
		int	value = 0;
		switch (tid){
			case msgf::VP_AMPLITUDE_ID: value = tVasVoiceParameter[_vNum].amp[pid]; break;
			case msgf::VP_FILTER_ID:	value = tVasVoiceParameter[_vNum].flt[pid]; break;
			case msgf::VP_OSCILLATOR_ID:value = tVasVoiceParameter[_vNum].osc[pid]; break;
			default: break;
		}
		return value;
	}

	static const int MAX_VAS_VOICE_NUMBER = 16;
	
private:
	static const VasVoiceParameter	tVasVoiceParameter[MAX_VAS_VOICE_NUMBER];
	int		_vNum;
	
};
#endif /* defined(__vas_voice_context__) */
