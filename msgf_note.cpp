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
#include "msgf_oscillator.h"
#include "msgf_filter.h"
#include "msgf_amplitude.h"
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
	_cndSustainPedal(false),
	_audioCounter(0)
{
	_vc = _parentInst->getVoiceContext();
	_osc = new Oscillator(this);
	_flt = new Filter(this);
	_amp = new Amplitude(this);
}
//---------------------------------------------------------
Note::~Note( void )
{
	releaseMe();
	delete _amp;
	delete _flt;
	delete _osc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void Note::keyOn( EventInfo* ei )
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

	//	Init
	_osc->init();
	_flt->init();
	_amp->init();
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

//---------------------------------------------------------
//		Release Me
//---------------------------------------------------------
void Note::releaseMe( void )
{
	//	Release from List
	_parentInst->releaseNote( this );
	
	if ( _prevNote != 0 ) _prevNote->setNextNote( _nextNote );
	if ( _nextNote != 0 ) _nextNote->setPrevNote( _prevNote );
	_prevNote = _nextNote = 0;
	_parentInst = 0;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
bool Note::process( TgAudioBuffer& buf )
{
	TgAudioBuffer	nbuf;
	nbuf.obtainAudioBuffer(buf.bufferSize());
	
	//	Oscillator
	_osc->process(nbuf);
	
	//	Filter
	_flt->process(nbuf);
	
	//	Amplitude
	_amp->process(nbuf);
	
	//	Effect

	
	//	Add to Buffer and Check no sound
	bool noSound = buf.mixAndCheckNoSound(nbuf);
	nbuf.releaseAudioBuffer();
		
	if (( noSound == true ) && ( _cndKeyOn == false )){
		//	this will be released
		return false;
	}

	return true;
}
