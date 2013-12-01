//
//  mf_voice_context.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "mf_voice_context.h"

const MfVoiceParameter MfVoiceContext::tMfVoiceParameter[MAX_MF_VOICE_NUMBER] = {
	
	{	//	0
		{
			50,		//VP_VOLUME,			//	0 - 100
			5,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
			30,		//VP_AEG_RELEASE_TIME,	//	0 - 1000 (10ms)
			
			0,		//VP_LFO_AMD,			//	0 - 100 %
		},
		{
			0,		//VP_TUNING,			//	-100 - 100(cent)
			0,		//VP_TRANSPOSE,			//	-24 - 24(seminote)
			0,		//VP_WAVE_TYPE,			//	0:sine, 1:tri

			1,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			10,		//VP_PORTAMENTO,		//	0 - ??? (rate: *10ms/100cent, time: *10ms)
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			50,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			50,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)
			0,		//VP_LFO_PMD,			//	0 - 100 %
		},
		{
			0,		//VP_FILTER_TYPE,		//	0:LPF,
			2000,	//VP_FILTER_CUTOFF,		//	[Hz]
			1,		//VP_FILTER_RESO,		//	1 - 100 ?
			
			0,		//VP_FEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
			0,		//VP_FEG_ATTACK_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
			0,		//VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
			0,		//VP_FEG_RELEASE_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
		}
	},
	{	//	1
		{
			50,		//VP_VOLUME,			//	0 - 100
			10,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
			100,	//VP_AEG_RELEASE_TIME,	//	0 - 1000 (10ms)
			
			0,		//VP_LFO_AMD,			//	0 - 100 %
		},
		{
			0,		//VP_TUNING,			//	-100 - 100(cent)
			0,		//VP_TRANSPOSE,			//	-24 - 24(seminote)
			2,		//VP_WAVE_TYPE,			//	0:sine, 1:tri

			0,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			0,		//VP_PORTAMENTO,		//	0 - ??? (rate: *10ms/100cent, time: *10ms)
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			50,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			50,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)
			2,		//VP_LFO_PMD,			//	0 - 100 %
		},
		{
			0,		//VP_FILTER_TYPE,		//	0:LPF,
			8000,	//VP_FILTER_CUTOFF,		//	[Hz]
			1,		//VP_FILTER_RESO,		//	1 - 100 ?
			
			100,	//VP_FEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
			70,		//VP_FEG_ATTACK_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
			0,		//VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
			0,		//VP_FEG_RELEASE_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
		}
	}
};