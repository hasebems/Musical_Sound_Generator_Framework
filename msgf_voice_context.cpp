//
//  msgf_voice_context.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/07.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_voice_context.h"
using namespace msgf;

//---------------------------------------------------------
//		Table
//---------------------------------------------------------
const int VoiceContext::tVoiceParameter[1][VP_MAX_ID] =
{
{
	50,		//VP_VOLUME,		//	0 - 100
	0,		//VP_PAN,			//	-100 - 0 - 100	,0:center
	
	0,		//VP_TUNING,		//	-100 - 100(cent)
	0,		//VP_TRANSPOSE,	//	-24 - 24(seminote)
	
	3,		//VP_WAVE_TYPE,	//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse

	20,		//VP_PEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
	-60,		//VP_PEG_ATTACK_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
	40,		//VP_PEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
	60,		//VP_PEG_RELEASE_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
	
	10,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
	80,		//VP_AEG_DECAY1_LEVEL,	//	0 - 100
	10,		//VP_AEG_DECAY1_TIME,	//	0 - 1000 (10ms)
	60,		//VP_AEG_DECAY2_LEVEL,	//	0 - 100
	200,	//VP_AEG_DECAY2_TIME,	//	0 - 1000 (10ms)
	100,		//VP_AEG_RELEASE_TIME,	//	0 - 1000 (10ms)

	0,		//VP_FILTER_TYPE,		//	0:LPF,
	1000,	//VP_FILTER_CUTOFF,		//	[Hz]
	7,		//VP_FILTER_RESO,		//	1 - 100 ?

	100,		//VP_FEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
	32,		//VP_FEG_ATTACK_LEVEL,	//	-32 - 0 - +32 (1/2 - 2[times*Hz])
	40,		//VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
	0,		//VP_FEG_RELEASE_LEVEL,	//	-32 - 0 - +32 (1/2 - 2[times*Hz])


}
};
