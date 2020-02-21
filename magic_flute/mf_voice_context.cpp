//
//  mf_voice_context.cpp
//  ToneGenerator
//
//  Created by Hasebe Masahiko on 2013/07/20.
//  Copyright (c) 2013 Hasebe Masahiko. All rights reserved.
//

#include "mf_voice_context.h"

const MfVoiceParameter MfVoiceContext::tMfVoiceParameter[MAX_MF_VOICE_NUMBER] = {
	
	{	//	0
		{
			50,		//VP_VOLUME,			//	0 - 100
			0,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
			30,		//VP_AEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
			
			0,		//VP_LFO_AMD,			//	0 - 100 %
			500,	//VP_DOWN_DCNT,			//	0 - inf (*22.67 =[us])	1000
			500,	//VP_MINLVL_DCNT,		//	0 - inf	(*22.67 =[us])	800
			1000,	//VP_UP_DCNT,			//	0 - inf	(*22.67 =[us])	3000
			100		//VP_MINLVL,			//	0 - 10000 (/100 =[%])	1000
		},
		{
			1,		//VP_OVERTONE_TYPE,		//	0:Sine, 1:LikeRecorder, 2:LikeOboe
			10,		//VP_TUNING,			//	-100 - 100(cent)
			0,		//VP_TRANSPOSE,			//	-24 - 24(seminote)

			0,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			10,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			10,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)

			500,	//VP_WAITING_DCNT,		//	0 - inf (*22.67 =[us])	1000
			1000,	//VP_FAST_MOVE_DCNT,	//	0 - inf	(*22.67 =[us])	800
			2		//VP_PORTAMENTO_DIFF,	//	-24 - 24(semitone)
		},
	},

	{	//	1
		{
			50,		//VP_VOLUME,			//	0 - 100
			0,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
			30,		//VP_AEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
			
			12,		//VP_LFO_AMD,			//	0 - 100 %
			500,	//VP_DOWN_DCNT,			//	0 - inf (*22.67 =[us])	1000
			500,	//VP_MINLVL_DCNT,		//	0 - inf	(*22.67 =[us])	800
			1000,	//VP_UP_DCNT,			//	0 - inf	(*22.67 =[us])	3000
			100		//VP_MINLVL,			//	0 - 10000 (/100 =[%])	1000
		},
		{
			2,		//VP_OVERTONE_TYPE,		//	0:Sine, 1:LikeRecorder, 2:LikeOboe
			10,		//VP_TUNING,			//	-100 - 100(cent)
			-12,	//VP_TRANSPOSE,			//	-24 - 24(seminote)
			
			0,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			10,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			10,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)
			
			500,	//VP_WAITING_DCNT,		//	0 - inf (*22.67 =[us])	1000
			1000,	//VP_FAST_MOVE_DCNT,	//	0 - inf	(*22.67 =[us])	800
			2		//VP_PORTAMENTO_DIFF,	//	-24 - 24(semitone)
		},
	}

};
