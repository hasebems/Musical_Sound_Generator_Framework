//
//  tmporg_voice_context.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2015/07/19.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#ifndef __tmporg_voice_context__
#define __tmporg_voice_context__

#include <iostream>
#include "msgf_voice_context.h"
#include "msgf_iirfilter.h"
#include "msgf_amp2seg.h"
#include "msgf_osc_tmpr.h"

//---------------------------------------------------------
struct TmporgVoiceParameter {
	
	int		amp[msgf::VP_AMPLITUDE_MAX];
	int		osc[msgf::VP_OSCILLATOR_MAX];
	int		flt[msgf::VP_FILTER_MAX];
};
//---------------------------------------------------------
class TmporgVoiceContext : public msgf::VoiceContext {
	
public:
	TmporgVoiceContext( int voiceNumber ):
	_vNum(voiceNumber){}
	
	int		getParameter( int tid, int pid )
	{
		int	value = 0;
		int vceNum = _vNum;

		if ( vceNum >= MAX_TMPORG_VOICE_NUMBER ) vceNum = MAX_TMPORG_VOICE_NUMBER-1;
		switch (tid){
			case msgf::VP_AMP_2SEG_ID:	value = tVoiceParameter[vceNum].amp[pid]; break;
			case msgf::VP_FILTER_ID:	value = tVoiceParameter[vceNum].flt[pid]; break;
			case msgf::VP_OSCILLATOR_TMPR_ID:	value = tVoiceParameter[vceNum].osc[pid]; break;
			default: break;
		}
		return value;
	}
	
	static const int MAX_TMPORG_VOICE_NUMBER = 1;
	
private:
	static const TmporgVoiceParameter	tVoiceParameter[MAX_TMPORG_VOICE_NUMBER];
	int		_vNum;
	
};
#endif /* defined(__mf_voice_context__) */
