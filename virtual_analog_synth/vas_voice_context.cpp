//
//  vas_voice_context.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "vas_voice_context.h"
using namespace msgf;

//---------------------------------------------------------
//		Table
//---------------------------------------------------------
const VasVoiceParameter VasVoiceContext::tVasVoiceParameter =
{
	{
		50,		//VP_VOLUME,		//	0 - 100
		10,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
		80,		//VP_AEG_DECAY1_LEVEL,	//	0 - 100
		100,	//VP_AEG_DECAY1_TIME,	//	0 - 1000 (10ms)
		70,		//VP_AEG_DECAY2_LEVEL,	//	0 - 100
		200,	//VP_AEG_DECAY2_TIME,	//	0 - 1000 (10ms)
		100,	//VP_AEG_RELEASE_TIME,	//	0 - 1000 (10ms)

		0,		//VP_LFO_AMD,				//	0 - 100 %
	},
	{
		0,		//VP_TUNING,		//	-100 - 100(cent)
		0,		//VP_TRANSPOSE,	//	-24 - 24(seminote)
		1,		//VP_WAVE_TYPE,	//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse
		
		0,		//VP_PEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
		0,		//VP_PEG_ATTACK_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
		0,		//VP_PEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
		0,		//VP_PEG_RELEASE_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])

		40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
		50,		//VP_LFO_DELAY_TIME,		//	0 - ??? (*10ms)
		50,		//VP_LFO_FADEIN_TIME,		//	0 - ??? (*10mc)
		2,		//VP_LFO_PMD,				//	0 - 100 %
	},
	{
		0,		//VP_FILTER_TYPE,		//	0:LPF,
		2000,	//VP_FILTER_CUTOFF,		//	[Hz]
		1,		//VP_FILTER_RESO,		//	1 - 100 ?
		
		100,	//VP_FEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
		70,		//VP_FEG_ATTACK_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
		0,		//VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
		0,		//VP_FEG_RELEASE_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
	}
};