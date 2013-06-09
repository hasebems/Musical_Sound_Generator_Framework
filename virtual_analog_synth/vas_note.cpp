//
//  vas_note.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/08.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "vas_note.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor/Destructor
//---------------------------------------------------------
VasNote::VasNote( Instrument* inst ) :
Note(inst)
{
	_osc = new Oscillator(*this);
	_flt = new IirFilter(*this);
	_amp = new Amplitude(*this);
}
//---------------------------------------------------------
VasNote::~VasNote( void )
{
	delete _amp;
	delete _flt;
	delete _osc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
bool VasNote::keyOn( EventInfo* ei )
{
	bool ret = Note::keyOn(ei);		//	required
	
	//	Init
	_osc->init();
	_flt->init();
	_amp->init();

	return ret;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void VasNote::keyOff( void )
{
	Note::keyOff();
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
bool VasNote::process( TgAudioBuffer& buf )
{
	//	Oscillator
	_osc->process(buf);
	
	//	Filter
	_flt->process(buf);
	
	//	Amplitude
	_amp->process(buf);

	//	Level Check	(required)
	manageNoteLevel(buf);

	return true;
}
