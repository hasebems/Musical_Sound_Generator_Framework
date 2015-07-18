//
//  msgf_instrument.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/08.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_instrument__
#define __msgf_instrument__

#include <iostream>
#include "msgf_type.h"
#include "msgf_voice_context.h"

namespace msgf {
	//---------------------------------------------------------
	typedef enum {
		
		DURING_KON,
		MAX_CONDITION
		
	} CONDITION;
	//---------------------------------------------------------
	class Note;
	class TgAudioBuffer;
	class Part;
	//---------------------------------------------------------
	class Instrument {
		
	public:
		Instrument( Part* pt, int vid ):
			_parentPart(pt),
			_voiceId(vid),
			_topNote(0),
			_endNote(0),
			_vc(0),
			_noteCounter(0){}
		virtual	~Instrument( void ){}

		//	Basic IF
		virtual void	keyOn( Uint8 note, Uint8 velocity ) = 0;
		virtual void	keyOff( Uint8 note, Uint8 velocity );
		virtual void	expression( Uint8 value ){}
		virtual void	sustain( Uint8 value );
		virtual void	allSoundOff( void );
		virtual void	specialCommand( Uint8 parameter1, Uint8 parameter2 ){}

		//	process thread
		virtual void	process( TgAudioBuffer& buf );

		void	releaseNote( const Note* nt );
		double	searchMinimumLevelNote( Note** nt );
		
		//	Accessor
		int		getVoiceId( void ) const { return _voiceId; }
		VoiceContext*	getVoiceContext( void ) const { return _vc; }
		Part*	getPart( void ) const { return _parentPart; }
		Note*	getTopNote( void ) const { return _topNote; }
		Note*	getEndNote( void ) const { return _endNote; }
		
	protected:
		void	appendNoteList( Note* nt );
		Note*	searchNote( Uint8 note, CONDITION cd );
		
		Part*	_parentPart;
		VoiceContext*	_vc;

		int		_voiceId;
		Note*	_topNote;
		Note*	_endNote;
		int		_noteCounter;
	};
}
#endif /* defined(__msgf_instrument__) */
