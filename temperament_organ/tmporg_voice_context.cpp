//
//  tmporg_voice_context.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "tmporg_voice_context.h"

const TmporgVoiceParameter TmporgVoiceContext::tVoiceParameter[MAX_TMPORG_VOICE_NUMBER] = {
	
	{	//	0
		{
			30,		//VP_VOLUME,				//	0 - 100
			0,		//VP_AEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
			30,		//VP_AEG_RELEASE_TIME,		//	0 - 1000 (*10ms)
			0		//VP_LFO_AMD,				//	0 - 100 %
		},
		{
			3		//VP_WAVEFORM,				//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse
		},
		{
			0,		//VP_FILTER_TYPE,			//	0:LPF,
			2000,	//VP_FILTER_CUTOFF,			//	[Hz]
			1,		//VP_FILTER_RESO,			//	1 - 100 ?
			
			0,		//VP_FEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
			0,		//VP_FEG_ATTACK_LEVEL,		//	-100 - 0 - +100 (1/4 - 4[times*Hz])
			0,		//VP_FEG_RELEASE_TIME,		//	0 - 1000 (*10ms)
			0		//VP_FEG_RELEASE_LEVEL,		//	-100 - 0 - +100 (1/4 - 4[times*Hz])
		}
	}

};