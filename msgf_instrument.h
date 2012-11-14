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

namespace msgf {
//---------------------------------------------------------
typedef enum {

	DURING_KON,
	MAX_CONDITION
	
} CONDITION;
//---------------------------------------------------------
class VoiceContext;
class Note;
class TgAudioBuffer;
class Part;
//---------------------------------------------------------
class Instrument {
	
public:
	Instrument( Part* pt, int vid );
	~Instrument( void );

	void	keyOn( Uint8 note, Uint8 velocity );
	void	keyOff( Uint8 note, Uint8 velocity );
	void	sustain( Uint8 value );
	void	allSoundOff( void );
	void	releaseNote( Note* nt );
	void	process( TgAudioBuffer& buf );
	
	//	Accessor
	int		getVoiceId( void ){ return _voiceId; }
	VoiceContext*	getVoiceContext( void ){ return _vc; }
	Part*	getPart( void ){ return _parentPart; }
	Note*	getTopNote( void ){ return _topNote; }
	Note*	getEndNote( void ){ return _endNote; }
	
private:
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
