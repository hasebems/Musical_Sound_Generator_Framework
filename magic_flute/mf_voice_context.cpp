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
			1000,	//VP_DOWN_DCNT,			//	0 - inf (*22.67 =[us])	1000
			800,	//VP_MINLVL_DCNT,		//	0 - inf	(*22.67 =[us])	800
			3000,	//VP_UP_DCNT,			//	0 - inf	(*22.67 =[us])	3000
			100		//VP_MINLVL,			//	0 - 10000 (/100 =[%])	1000
		},
		{
			0,		//VP_TUNING,			//	-100 - 100(cent)
			0,		//VP_TRANSPOSE,			//	-24 - 24(seminote)
			4,		//VP_CARRIER_FREQ,		//	0 - 20?
			3,		//VP_CARRIER_LEVEL,		//	0 - 100[%]

			1,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			3,		//VP_PORTAMENTO,		//	0 - ??? (rate: *10ms/100cent, time: *10ms)	3
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			50,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			50,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)
			0,		//VP_LFO_PMD,			//	0 - 100 %

			1000,	//VP_WAITING_DCNT,		//	0 - inf (*22.67 =[us])	1000
			800,	//VP_FAST_MOVE_DCNT,	//	0 - inf	(*22.67 =[us])	800
			1		//VP_PORTAMENTO_DIFF,	//	-24 - 24(semitone)
		},
//		{
//			0,		//VP_FILTER_TYPE,		//	0:LPF,
//			2000,	//VP_FILTER_CUTOFF,		//	[Hz]
//			1,		//VP_FILTER_RESO,		//	1 - 100 ?
//
//			0,		//VP_FEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
//			0,		//VP_FEG_ATTACK_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
//			0,		//VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
//			0,		//VP_FEG_RELEASE_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
//		}
	},
	{	//	1
		{
			50,		//VP_VOLUME,			//	0 - 100
			10,		//VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
			100,	//VP_AEG_RELEASE_TIME,	//	0 - 1000 (10ms)
			
			0,		//VP_LFO_AMD,			//	0 - 100 %
			1000,	//VP_DOWN_DCNT,			//	0 - inf (*22.67 =[us])
			800,	//VP_MINLVL_DCNT,		//	0 - inf	(*22.67 =[us])
			3000,	//VP_UP_DCNT,			//	0 - inf	(*22.67 =[us])
			1000	//VP_MINLVL,			//	0 - 10000 (/100 =[%])
		},
		{
			0,		//VP_TUNING,			//	-100 - 100(cent)
			0,		//VP_TRANSPOSE,			//	-24 - 24(seminote)
			1,		//VP_CARRIER_FREQ,		//	0 - 20?
			10,		//VP_CARRIER_LEVEL,		//	0 - 100[%]

			0,		//VP_PORTAMENTO_MODE,	//	0:rate constant, 1:time constant
			0,		//VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
			0,		//VP_PORTAMENTO,		//	0 - ??? (rate: *10ms/100cent, time: *10ms)
			
			40,		//VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
			50,		//VP_LFO_DELAY_TIME,	//	0 - ??? (*10ms)
			50,		//VP_LFO_FADEIN_TIME,	//	0 - ??? (*10mc)
			2,		//VP_LFO_PMD,			//	0 - 100 %

			1000,	//VP_WAITING_DCNT,		//	0 - inf (*22.67 =[us])
			800,	//VP_FAST_MOVE_DCNT,	//	0 - inf	(*22.67 =[us])
			1		//VP_PORTAMENTO_DIFF,	//	-24 - 24(semitone)
		},
//		{
//			0,		//VP_FILTER_TYPE,		//	0:LPF,
//			8000,	//VP_FILTER_CUTOFF,		//	[Hz]
//			1,		//VP_FILTER_RESO,		//	1 - 100 ?
//
//			100,	//VP_FEG_ATTACK_TIME,	//	0 - 1000 (*10ms)
//			70,		//VP_FEG_ATTACK_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
//			0,		//VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
//			0,		//VP_FEG_RELEASE_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
//		}
	}
};