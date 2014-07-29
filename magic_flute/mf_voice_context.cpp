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
			5,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
			5,		//VP_AEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
			
			0,		//VP_LFO_AMD,			//	0 - 100 %
			1000,	//VP_DOWN_DCNT,			//	0 - inf (*22.67 =[us])	1000
			800,	//VP_MINLVL_DCNT,		//	0 - inf	(*22.67 =[us])	800
			3000,	//VP_UP_DCNT,			//	0 - inf	(*22.67 =[us])	3000
			100		//VP_MINLVL,			//	0 - 10000 (/100 =[%])	1000
		},
		{
			10,		//VP_TUNING,			//	-100 - 100(cent)
			0,		//VP_TRANSPOSE,			//	-24 - 24(seminote)

			0,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			50,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			50,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)

			1000,	//VP_WAITING_DCNT,		//	0 - inf (*22.67 =[us])	1000
			800,	//VP_FAST_MOVE_DCNT,	//	0 - inf	(*22.67 =[us])	800
			2		//VP_PORTAMENTO_DIFF,	//	-24 - 24(semitone)
		},
	}
};