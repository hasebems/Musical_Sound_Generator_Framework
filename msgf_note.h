//
//  msgf_note.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/08.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_note__
#define __msgf_note__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
//---------------------------------------------------------
class EventInfo;
class TgAudioBuffer;
class Instrument;
class VoiceContext;
class Oscillator;
	class Filter;
class Amplitude;
//---------------------------------------------------------
class Note {

public:
	Note( Instrument* inst );
	~Note( void );
	
	void	keyOn( EventInfo* ei );
	void	sustain( Uint8 value );
	void	keyOff( void );
	void	damp( void );
	void	releaseMe( void );

	bool	process( TgAudioBuffer& buf );

	//	accessor
	Note*	getNextNote( void ){ return _nextNote; }
	Note*	getPrevNote( void ){ return _prevNote; }
	void	setNextNote( Note* nt ){ _nextNote = nt; }
	void	setPrevNote( Note* nt ){ _prevNote = nt; }

	VoiceContext*	getVoiceContext( void ){ return _vc; }
	Instrument*		getInstrument( void ){ return _parentInst; }

	Uint8	getNote( void ){ return _noteNumber; }
	Uint8	getVelocity( void ){ return _velocity; }
	
	bool	conditionKeyOn( void ){ return _cndKeyOn; }
	bool	conditionSustainPedal( void ){ return _cndSustainPedal; }

private:
	Instrument*		_parentInst;
	VoiceContext*	_vc;
	Note*	_nextNote;
	Note*	_prevNote;

	//	basic attribute
	Uint8	_noteNumber;
	Uint8	_velocity;
	
	//	condition
	bool	_cndKeyOn;
	bool	_cndSustainPedal;

	long	_audioCounter;

	//	Audio Engines
	Oscillator*		_osc;
	Filter*			_flt;
	Amplitude*		_amp;
};
}
#endif /* defined(__msgf_note__) */
