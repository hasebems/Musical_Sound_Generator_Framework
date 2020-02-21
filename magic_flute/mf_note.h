//
//  mf_note.h
//  ToneGenerator
//
//  Created by Hasebe Masahiko on 2013/07/19.
//  Copyright (c) 2013 Hasebe Masahiko. All rights reserved.
//

#ifndef __mf_note__
#define __mf_note__

#include <iostream>
#include "msgf_note.h"
//#include "msgf_iirfilter.h"
#include "msgf_osc_pipe.h"
#include "msgf_amp_pipe.h"
#include "msgf_event_info.h"

//---------------------------------------------------------
class MfNote : public msgf::Note {
	
public:
	MfNote( msgf::Instrument* inst );
	~MfNote( void );
	bool	keyOn( msgf::EventInfo* ei );
	void	keyOff( void );
	//	void	damp( void );

	void	keyOnAlternate( msgf::EventInfo* ei );
	void	changeNote( msgf::EventInfo* ei );
	bool	noteSoundProcess( msgf::TgAudioBuffer& buf );
	
private:
	//	Audio Engines
	msgf::OscPipe*			_osc;
//	msgf::IirFilter*		_flt;
	msgf::AmpPipe*			_amp;
};

#endif /* defined(__mf_note__) */
