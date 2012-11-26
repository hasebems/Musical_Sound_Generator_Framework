//
//  msgf_event_info.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/13.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_event_info__
#define __msgf_event_info__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
//---------------------------------------------------------
class VoiceContext;
class LayerContext;
//---------------------------------------------------------
class EventInfo {
	
public:
	EventInfo( void ):
		_velocity(127), _note(60) {}

	//	Accessor
	void	setVelocity( Uint8 vel ){ _velocity = vel;}
	void	setNote( Uint8 nt ){ _note = nt; }

	Uint8	getVelocity( void ){ return _velocity; }
	Uint8	getNote( void ){ return _note; }

private:
	Uint8			_velocity;
	Uint8			_note;
	
};
}
#endif /* defined(__msgf_event_info__) */
