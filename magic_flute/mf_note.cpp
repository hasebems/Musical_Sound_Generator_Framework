//
//  mf_note.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/19.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "mf_note.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor/Destructor
//---------------------------------------------------------
MfNote::MfNote( Instrument* inst ) :
Note(inst)
{
	_osc = new OscPipe(*this);
//	_flt = new IirFilter(*this);
	_amp = new AmpPipe(*this);
}
//---------------------------------------------------------
MfNote::~MfNote( void )
{
	delete _amp;
//	delete _flt;
	delete _osc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
bool MfNote::keyOn( EventInfo* ei )
{
	bool ret = Note::keyOn(ei);		//	required
	
	//	Init
	_osc->init(true);
//	_flt->init();
	_amp->init();
	
	return ret;
}
//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void MfNote::keyOff( void )
{
	Note::keyOff();
	_osc->release();
//	_flt->release();
	_amp->release();
}
//---------------------------------------------------------
//		Alternate Key On
//---------------------------------------------------------
void MfNote::keyOnAlternate( EventInfo* ei )
{
	//	Set Variables
	setConditionKeyOn();
	setNote( ei->getNote() );
	setVelocity( ei->getVelocity() );

	//	Init
	_osc->init(false);
//	_flt->init();
	_amp->init();
}
//---------------------------------------------------------
//		Change Note
//---------------------------------------------------------
void MfNote::changeNote( EventInfo* ei )
{
	bool	changeNote = true;
	
	if ( getNote() != ei->getNote() ){
		setNote( ei->getNote() );
	}
	else changeNote = false;
	
	//	Init
	_osc->changeNote(changeNote);
	_amp->changeNote(changeNote);
}
//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
bool MfNote::noteSoundProcess( TgAudioBuffer& buf )
{
	//	Oscillator
	_osc->process(buf);
	
	//	Filter
//	_flt->process(buf);
	
	//	Amplitude
	_amp->process(buf);
	
	return true;
}

