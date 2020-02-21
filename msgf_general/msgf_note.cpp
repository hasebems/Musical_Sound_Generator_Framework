//
//  msgf_note.cpp
//
//	Musical Sound Generator Framework
//
//  Created by Hasebe Masahiko on 2012/10/08.
//  Copyright (c) 2012 Hasebe Masahiko. All rights reserved.
//

#include "msgf_note.h"
#include "msgf_instrument.h"
#include "msgf_event_info.h"
#include "msgf_voice_context.h"
#include "msgf_audio_buffer.h"
#include "msgf_level_check.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor/Destructor
//---------------------------------------------------------
Note::Note( Instrument* inst ) :
	_parentInst(inst),
	_nextNote(nullptr),
	_prevNote(nullptr),
	_lvlChk(0),
	_dampCounter(0),
	_cndKeyOn(false),
	_cndSustainPedal(false),
	_cndDuringDamp(false)
{
	_vc = _parentInst->getVoiceContext();
}
//---------------------------------------------------------
Note::~Note( void )
{
	if ( _lvlChk ) delete _lvlChk;	//	to make sure
	
	//	Release from List
	_parentInst->releaseNote( this );
	
	if ( _prevNote != nullptr ) _prevNote->setNextNote( _nextNote );
	if ( _nextNote != nullptr ) _nextNote->setPrevNote( _prevNote );
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
	if ( lastKeyOnNote != nullptr ) lastKeyOnNote->setNextNote(this);
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
	if ( _lvlChk ){
		delete _lvlChk;
		_lvlChk = 0;
	}
	
	//	Damp Sound
	_cndDuringDamp = true;
	_dampCounter = 0;
}

//---------------------------------------------------------
//		Manage Note Level
//---------------------------------------------------------
void Note::manageNoteLevel( TgAudioBuffer& buf )
{
	if ( _lvlChk == nullptr ) _lvlChk = new LevelCheck(buf.bufferSize());

	if ( _cndDuringDamp == false ){
		//	Check Level
		double lvl = buf.getSoundLevel();
		_lvlChk->appendLevel(lvl);
	}
	else {
		//	Damp
		for ( int i=0; i<buf.bufferSize(); i++ ){
			double rate;
			if ( _dampCounter <= DAMP_RATE ){
				rate = ((double)(DAMP_RATE-_dampCounter))/DAMP_RATE;
				rate *= rate;
			}
			else rate = 0;
			buf.mulAudioBuffer( i, rate );
			_dampCounter++;
		}
	}
}
