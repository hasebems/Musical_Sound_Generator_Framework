//
//  tmporg_instrument.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2015/07/18.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#include "msgf_event_info.h"

#include "tmporg_instrument.h"
#include "tmporg_voice_context.h"
#include "tmporg_note.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
TmporgInstrument::TmporgInstrument( Part* pt, int vid ):
Instrument(pt,vid),
_lastNote(0x3c),
_konCount()
{
	_vc = new TmporgVoiceContext( vid );
	_setPitchCounter = 0;
	for ( int i=0; i<4; i++ ){
		for ( int j=0; j<12; j++ ){
			_tmperamentPitch[i][j] = 0;
		}
	}
}
//---------------------------------------------------------
TmporgInstrument::~TmporgInstrument( void )
{
	delete _vc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void TmporgInstrument::keyOn( Uint8 note, Uint8 velocity )
{
	TmporgNote* nt = new TmporgNote(this);
	EventInfo* ei = new EventInfo();

	//	regist callback
	nt->getOscTmpr()->addGetFuncObj(this);
	
	ei->setVelocity( velocity );
	ei->setNote( note );
	
	if ( nt->keyOn( ei ) == true ){
		appendNoteList( nt );
	}
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void TmporgInstrument::keyOff( Uint8 note, Uint8 velocity )
{
	Note* nt = _topNote;
	Note* ntOk = 0;

	while ( nt != 0 ){
		if (( nt->getNote() == note ) &&
			( nt->conditionKeyOn() == true ) &&
			( nt->getVelocity()%4 == velocity%4)){
			ntOk = nt;
			break;
		}
		nt = nt->getNextNote();
	}

	if ( ntOk ){
		//	correspond velocity
		ntOk->keyOff();
	}
	else {
		//	retry normal keyoff
		Instrument::keyOff(note,velocity);
	}
}

//---------------------------------------------------------
//		Each Key Pitch Command
//---------------------------------------------------------
void TmporgInstrument::specialCommand( Uint8 statusByte, Uint8 parameter1, Uint8 parameter2 )
{
	//	F1:start, F2:others(MAX 48)
	//	parameter1 * 256 + parameter2 = rawdata[0-65535]
	//	pitch[cent] = (rawdata - 0x8000)/100
	//		327.67 ... 0 ... -327.68[cent]
	
	if ( statusByte == 0xf1 ){
		_setPitchCounter = 0;
	}

	//	remake pitch value from Uint8-value
	Uint16 tmpPitch = ((Uint16)parameter1<<8) | (Uint16)parameter2;
	int tmpPitchInt = static_cast<int>(tmpPitch) - 32768;
	double tmpPitchDouble = static_cast<double>(tmpPitchInt) / 100;
	_tmperamentPitch[_setPitchCounter/12][_setPitchCounter%12] = tmpPitchDouble;

	_setPitchCounter++;
	if ( _setPitchCounter >= 48 ){
		_setPitchCounter = 47;

		//	Pitch Change for current sounding note
		Note* nt = _topNote;
		while ( nt != 0 ){
			TmporgNote* ntt = dynamic_cast<TmporgNote*>(nt);
			ntt->getOscTmpr()->changeTmpr();
			nt = nt->getNextNote();
		}

	}
}
//---------------------------------------------------------
//		Get Each Key Pitch
//---------------------------------------------------------
double TmporgInstrument::getCent( int doremi, int velVari )
{
	if ( doremi >= 12 ) doremi = 11;
	if ( velVari >= 4 ) velVari = 3;
	return _tmperamentPitch[velVari][doremi];
}

