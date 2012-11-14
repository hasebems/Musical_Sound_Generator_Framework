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
	
	0,		//VP_WAVE_TYPE,	//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse
	
	10,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
	80,		//VP_AEG_DECAY1_LEVEL,//	0 - 100
	10,		//VP_AEG_DECAY1_TIME,	//	0 - 1000 (10ms)
	60,		//VP_AEG_DECAY2_LEVEL,//	0 - 100
	200,	//VP_AEG_DECAY2_TIME,	//	0 - 1000 (10ms)
	200		//VP_AEG_RELEASE_TIME,//	0 - 1000 (10ms)
}
};
