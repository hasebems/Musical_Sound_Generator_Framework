//
//  msgf_eg.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/02/02.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_eg__
#define __msgf_eg__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
	//---------------------------------------------------------
	typedef enum {
		
		EG_NOT_YET,
		EG_ATTACK,
		EG_DECAY1,
		EG_DECAY2,
		EG_KEY_ON_STEADY,
		EG_RELEASE,
		EG_KEY_OFF_STEADY,
		EG_STATE_MAX
		
	} EG_STATE;
	//---------------------------------------------------------
	class CallBack {
	public:
		virtual double	getEgLvl( EG_STATE prm ) = 0;
		virtual int		getEgTime( EG_STATE prm ) = 0;
	};
	//---------------------------------------------------------
	class Eg {
	public:
		virtual ~Eg(){}
		
		//	Judge Segment change
		virtual void	periodicOnceEveryProcess( void ) = 0;
		virtual void	periodicOnceEveryDac( long dacCnt ) = 0;
		virtual double	calcEgLevel( void ) = 0;					//	return: -1 - 0 - +1
	};
}
#endif /* defined(__ToneGenerator__msgf_eg__) */
