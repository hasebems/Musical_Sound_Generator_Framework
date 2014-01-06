//
//  mf_voice_context.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __mf_voice_context__
#define __mf_voice_context__

#include <iostream>
#include "msgf_voice_context.h"
//#include "msgf_iirfilter.h"
#include "msgf_amp_pipe.h"
#include "msgf_osc_pipe.h"

//---------------------------------------------------------
struct MfVoiceParameter {
	
	int		amp[msgf::VP_AMPLITUDE_MAX];
	int		osc[msgf::VP_OSCILLATOR_MAX];
//	int		flt[msgf::VP_FILTER_MAX];
};
//---------------------------------------------------------
class MfVoiceContext : public msgf::VoiceContext {
	
public:
	MfVoiceContext( int voiceNumber ):
	_vNum(voiceNumber){}
	
	int		getParameter( int tid, int pid )
	{
		int	value = 0;
		int vceNum = _vNum;

		if ( vceNum >= MAX_MF_VOICE_NUMBER ) vceNum = MAX_MF_VOICE_NUMBER-1;
		switch (tid){
			case msgf::VP_AMP_PIPE_ID:	value = tMfVoiceParameter[vceNum].amp[pid]; break;
//			case msgf::VP_FILTER_ID:	value = tMfVoiceParameter[vceNum].flt[pid]; break;
			case msgf::VP_OSC_PIPE_ID:	value = tMfVoiceParameter[vceNum].osc[pid]; break;
			default: break;
		}
		return value;
	}
	
	static const int MAX_MF_VOICE_NUMBER = 2;
	
private:
	static const MfVoiceParameter	tMfVoiceParameter[MAX_MF_VOICE_NUMBER];
	int		_vNum;
	
};
#endif /* defined(__mf_voice_context__) */
