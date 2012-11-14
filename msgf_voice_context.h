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

	VP_VOLUME,		//	0 - 100
	VP_PAN,			//	-100 - 0 - 100	,0:center

	VP_TUNING,		//	-100 - 100(cent)
	VP_TRANSPOSE,	//	-24 - 24(seminote)

	VP_WAVE_TYPE,	//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse

	VP_AEG_ATTACK_TIME,	//	0 - 1000 (10ms)
	VP_AEG_DECAY1_LEVEL,//	0 - 100
	VP_AEG_DECAY1_TIME,	//	0 - 1000 (10ms)
	VP_AEG_DECAY2_LEVEL,//	0 - 100
	VP_AEG_DECAY2_TIME,	//	0 - 1000 (10ms)
	VP_AEG_RELEASE_TIME,//	0 - 1000 (10ms)
	
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
