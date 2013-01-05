//
//  msgf_voice_context.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/07.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_voice_context__
#define __msgf_voice_context__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
//---------------------------------------------------------
typedef enum {

	//	All type are Integer

	VP_VOLUME,				//	0 - 100
	VP_PAN,					//	-100 - 0 - 100	,0:center

	VP_TUNING,				//	-100 - 100(cent)
	VP_TRANSPOSE,			//	-24 - 24(seminote)

	VP_WAVEFORM,			//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse

	VP_PEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
	VP_PEG_ATTACK_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
	VP_PEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
	VP_PEG_RELEASE_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
	
	VP_AEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
	VP_AEG_DECAY1_LEVEL,	//	0 - 100
	VP_AEG_DECAY1_TIME,		//	0 - 1000 (*10ms)
	VP_AEG_DECAY2_LEVEL,	//	0 - 100
	VP_AEG_DECAY2_TIME,		//	0 - 1000 (*10ms)
	VP_AEG_RELEASE_TIME,	//	0 - 1000 (*10ms)

	VP_FILTER_TYPE,			//	0:LPF,
	VP_FILTER_CUTOFF,		//	[Hz]
	VP_FILTER_RESO,			//	1 - 100 ?

	VP_FEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
	VP_FEG_ATTACK_LEVEL,	//	-32 - 0 - +32 (1/2 - 2[times*Hz])
	VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
	VP_FEG_RELEASE_LEVEL,	//	-32 - 0 - +32 (1/2 - 2[times*Hz])
		
	VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
	VP_LFO_DELAY_TIME,		//	0 - ??? (*10ms)
	VP_LFO_FADEIN_TIME,		//	0 - ??? (*10mc)
	VP_LFO_PMD,				//	0 - 100 % 
	VP_LFO_AMD,				//	0 - 100 %
	VP_LFO_CMD,				//	0 - 100 %
	
	VP_MAX_ID

} VoiceParameterId;

//---------------------------------------------------------
class VoiceContext {
	
public:	
	int		getVoiceId( Uint8 msb, Uint8 lsb, Uint8 pcNum ){ return 0;}
	int		getParameter( VoiceParameterId pid ){ return tVoiceParameter[0][pid];}

private:
	static const int	tVoiceParameter[1][VP_MAX_ID];

};
}
#endif /* defined(__msgf_voice_context__) */
