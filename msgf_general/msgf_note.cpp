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
#include "msgf_level_check.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor/Destructor
//---------------------------------------------------------
Note::Note( Instrument* inst ) :
	_parentInst(inst),
	_nextNote(0),
	_prevNote(0),
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
	
	if ( _prevNote != 0 ) _prevNote->setNextNote( _nextNote );
	if ( _nextNote != 0 ) _nextNote->setPrevNote( _prevNote );
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
	if ( _lvlChk == 0 ) _lvlChk = new LevelCheck(buf.bufferSize());

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
