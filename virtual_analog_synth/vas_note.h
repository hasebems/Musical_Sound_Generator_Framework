//
//  vas_note.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/08.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __ToneGenerator__vas_note__
#define __ToneGenerator__vas_note__

#include <iostream>
#include "msgf_note.h"

//---------------------------------------------------------
class msgf::TgAudioBuffer;
class msgf::EventInfo;
class msgf::Oscillator;
class msgf::Filter;
class msgf::Amplitude;
//---------------------------------------------------------
class VasNote : public msgf::Note {
	
public:
	VasNote( msgf::Instrument* inst );
	~VasNote( void );
	bool	keyOn( msgf::EventInfo* ei );
//	void	sustain( msgf::Uint8 value );
	void	keyOff( void );
//	void	damp( void );
	bool	process( msgf::TgAudioBuffer& buf );
	
private:
	//	Audio Engines
	msgf::Oscillator*		_osc;
	msgf::Filter*			_flt;
	msgf::Amplitude*		_amp;
};
#endif /* defined(__ToneGenerator__vas_note__) */
