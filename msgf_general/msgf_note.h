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
#include "msgf_audio_buffer.h"

namespace msgf {

	const int DAMP_RATE = 400;		// * dac time(22.68usec)
	//---------------------------------------------------------
	class EventInfo;
	class TgAudioBuffer;
	class Instrument;
	class VoiceContext;
	class LevelCheck;
	//---------------------------------------------------------
	class Note {
		
	public:
		Note( Instrument* inst );
		virtual	~Note( void );
		
		//	Basic IF
		virtual bool	keyOn( EventInfo* ei );
		virtual void	sustain( Uint8 value );
		virtual void	keyOff( void );
		virtual void	damp( void );

		//	process thread
		virtual bool	process( TgAudioBuffer& buf )
		{
			manageNoteLevel( buf );
			return false;
		}
		
		//	accessor
		Note*	getNextNote( void ) const { return _nextNote; }
		Note*	getPrevNote( void ) const { return _prevNote; }
		void	setNextNote( Note* nt ){ _nextNote = nt; }
		void	setPrevNote( Note* nt ){ _prevNote = nt; }
		LevelCheck*	getLvlChk( void ) const { return _lvlChk; }
		
		VoiceContext*	getVoiceContext( void ) const { return _vc; }
		Instrument*		getInstrument( void ) const { return _parentInst; }
		
		Uint8	getNote( void ) const { return _noteNumber; }
		Uint8	getVelocity( void ) const { return _velocity; }
		
		bool	conditionKeyOn( void ) const { return _cndKeyOn; }
		bool	conditionSustainPedal( void ) const { return _cndSustainPedal; }
		bool	conditionDuringDamp( void ) const { return _cndDuringDamp; }
		
	protected:
		void	manageNoteLevel( TgAudioBuffer& buf );	//	called in process()
		
	private:
		//	relation
		Instrument*		_parentInst;
		VoiceContext*	_vc;
		Note*			_nextNote;
		Note*			_prevNote;
		LevelCheck*		_lvlChk;
		
		//	basic attribute
		Uint8	_noteNumber;
		Uint8	_velocity;
		long	_dampCounter;
		
		//	condition
		bool	_cndKeyOn;
		bool	_cndSustainPedal;
		bool	_cndDuringDamp;
	};
}
#endif /* defined(__msgf_note__) */
