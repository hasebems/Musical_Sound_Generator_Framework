//
//  msgf_event_info.h
//
//	Musical Sound Generator Framework
//
//  Created by Hasebe Masahiko on 2012/10/13.
//  Copyright (c) 2012 Hasebe Masahiko. All rights reserved.
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
		EventInfo( Uint8 nt, Uint8 vel ):
			_velocity(vel), _note(nt) {}

		//	Accessor
		void	setVelocity( Uint8 vel ){ _velocity = vel;}
		void	setNote( Uint8 nt ){ _note = nt; }

		Uint8	getVelocity( void ) const { return _velocity; }
		Uint8	getNote( void ) const { return _note; }

	private:
		Uint8			_velocity;
		Uint8			_note;
	};
}
#endif /* defined(__msgf_event_info__) */
