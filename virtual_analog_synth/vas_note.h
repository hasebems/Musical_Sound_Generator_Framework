//
//  vas_note.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/08.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __vas_note__
#define __vas_note__

#include <iostream>
#include "msgf_note.h"
#include "msgf_iirfilter.h"
#include "msgf_osc_synth.h"
#include "msgf_amp4seg.h"

//---------------------------------------------------------
class VasNote : public msgf::Note {
	
public:
	VasNote( msgf::Instrument* inst );
	~VasNote( void );
	bool	keyOn( msgf::EventInfo* ei );
//	void	sustain( msgf::Uint8 value );
	void	keyOff( void );
//	void	damp( void );
	bool	noteSoundProcess( msgf::TgAudioBuffer& buf );
	
private:
	//	Audio Engines
	msgf::Oscillator*		_osc;
	msgf::IirFilter*		_flt;
	msgf::Amplitude*		_amp;
};
#endif /* defined(__vas_note__) */
