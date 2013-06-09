//
//  msgf_signal_process_core.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/25.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_signal_process_core__
#define __msgf_signal_process_core__

#include <iostream>
#include "msgf_type.h"
#include "msgf_audio_buffer.h"
#include "msgf_voice_context.h"

namespace msgf {
	//---------------------------------------------------------
	class SignalProcessCore {
		
	public:
		SignalProcessCore( void ):
			_dacCounter(0) {}
		virtual ~SignalProcessCore( void ){}

		virtual void	init( void ) = 0;
		virtual void	process( TgAudioBuffer& buf ) = 0;
		
	protected:
		//	Judge Segment change
		virtual	void	checkEvent( void ){}
		
		int		getTotalDacCount( const int time ){return time*(SAMPLING_FREQUENCY/100);}
		
		//	Basic Variables
		long	_dacCounter;		//	should be incremented by buffer size in process
	};
}
#endif
