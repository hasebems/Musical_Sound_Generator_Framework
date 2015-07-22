//
//  tmporg_note.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2015/07/18.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#include "tmporg_note.h"
#include "tmporg_instrument.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor/Destructor
//---------------------------------------------------------
TmporgNote::TmporgNote( Instrument* inst ) :
Note(inst)
{
	_osc = new OscillatorTmpr(*this);
	_flt = new IirFilter(*this);
	_amp = new Amp2seg(*this);
}
//---------------------------------------------------------
TmporgNote::~TmporgNote( void )
{
	delete _amp;
	delete _flt;
	delete _osc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
bool TmporgNote::keyOn( EventInfo* ei )
{
	bool ret = Note::keyOn(ei);		//	required
	
	//	Init
	_osc->init();
	_flt->init();
	_amp->init();
	
	return ret;
}
//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void TmporgNote::keyOff( void )
{
	Note::keyOff();
	_osc->release();
	_flt->release();
	_amp->release();
}
//---------------------------------------------------------
//		Change Note
//---------------------------------------------------------
void TmporgNote::changeNote( EventInfo* ei )
{
}
//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
bool TmporgNote::noteSoundProcess( TgAudioBuffer& buf )
{
	//	Oscillator
	_osc->process(buf);
	
	//	Filter
	_flt->process(buf);
	
	//	Amplitude
	_amp->process(buf);
	
	return true;
}

