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
		_velocity(127), _note(60), _vc(0) {}

	//	Accessor
	void	setVelocity( Uint8 vel ){ _velocity = vel;}
	void	setNote( Uint8 nt ){ _note = nt; }
	void	setVoiceContext( VoiceContext* vctx ){ _vc = vctx; }

	Uint8	getVelocity( void ){ return _velocity; }
	Uint8	getNote( void ){ return _note; }
	VoiceContext*	getVoiceContext( void ){ return _vc; }
	
private:
	Uint8			_velocity;
	Uint8			_note;
	VoiceContext*	_vc;
	
};
}
#endif /* defined(__msgf_event_info__) */
