//
//  msgf_note.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/08.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_note.h"
#include "msgf_instrument.h"
#include "msgf_event_info.h"
#include "msgf_voice_context.h"
#include "msgf_audio_buffer.h"
using namespace msgf;

//---------------------------------------------------------
//		Constructor/Destructor
//---------------------------------------------------------
Note::Note( Instrument* inst ) :
	_parentInst(inst),
	_nextNote(0),
	_prevNote(0),
	_cndKeyOn(false),
	_cndSustainPedal(false)
{
	_vc = _parentInst->getVoiceContext();
}
//---------------------------------------------------------
Note::~Note( void )
{
	//	Release from List
	_parentInst->releaseNote( this );
	
	if ( _prevNote != 0 ) _prevNote->setNextNote( _nextNote );
	if ( _nextNote != 0 ) _nextNote->setPrevNote( _prevNote );
	_prevNote = _nextNote = 0;
	_parentInst = 0;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
bool Note::keyOn( EventInfo* ei )
{
	//	Set Variables
	_cndKeyOn = true;
	_noteNumber = ei->getNote();
	_velocity = ei->getVelocity();

	//	Make List
	Note* lastKeyOnNote = _parentInst->getEndNote();
	if ( lastKeyOnNote != 0 ) lastKeyOnNote->setNextNote(this);
	_prevNote = lastKeyOnNote;
	_nextNote = 0;

	return true;
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void Note::keyOff( void )
{
	_cndKeyOn = false;
}

//---------------------------------------------------------
//		Sustain
//---------------------------------------------------------
void Note::sustain( Uint8 value )
{
	if ( value >= 64 ){
		_cndSustainPedal	= true;
	}
	else {
		_cndSustainPedal	= false;
	}
}
	
//---------------------------------------------------------
//		Damp
//---------------------------------------------------------
void Note::damp( void )
{
	_cndKeyOn = false;
	
	//	Damp Sound

	
}
