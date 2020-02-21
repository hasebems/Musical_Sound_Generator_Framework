//
//  msgf_voice_context.h
//
//	Musical Sound Generator Framework
//
//  Created by Hasebe Masahiko on 2012/10/07.
//  Copyright (c) 2012 Hasebe Masahiko. All rights reserved.
//

#ifndef __msgf_voice_context__
#define __msgf_voice_context__

#include <iostream>
#include "msgf_type.h"

namespace msgf {

	//---------------------------------------------------------
	class VoiceContext {

	public:
		virtual ~VoiceContext( void ){}
		
		virtual int		getParameter( int tid, int pid ) = 0;
	};
}
#endif /* defined(__msgf_voice_context__) */
