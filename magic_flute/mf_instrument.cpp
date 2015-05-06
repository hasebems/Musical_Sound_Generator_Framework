//
//  mf_instrument.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/19.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_event_info.h"

#include "mf_instrument.h"
#include "mf_voice_context.h"
#include "mf_note.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
MfInstrument::MfInstrument( Part* pt, int vid ):
Instrument(pt,vid),
_lastNote(0x3c),
_konCount()
{
	_vc = new MfVoiceContext( vid );
}
//---------------------------------------------------------
MfInstrument::~MfInstrument( void )
{
	delete _vc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void MfInstrument::keyOn( Uint8 note, Uint8 velocity )
{
	EventInfo* ei = new EventInfo(note, velocity);

	if ( _topNote != 0 ){
		if ( _topNote->conditionKeyOn() == true ){
			//	Change Note
			_topNote->changeNote( ei );
		}
		else {
			//	Alternate KeyOn
			_topNote->keyOnAlternate( ei );
		}
	}
	else {
		Note* nt = new MfNote(this);
		if ( nt->keyOn( ei ) == true ){
			//	New Key On
			appendNoteList( nt );
		}
	}

	_konCount[note]++;
	_lastNote = note;
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void MfInstrument::keyOff( Uint8 note, Uint8 velocity )
{
	Note* kfNote = searchNote( note, DURING_KON );
	if ( kfNote ){
		//	check duplicated KeyOn & let it not KeyOff
		if ( _konCount[note] == 1 ){
			kfNote->keyOff();
		}
	}

	if ( _konCount[note] > 0 ){
		_konCount[note]--;
	}
}

//---------------------------------------------------------
//		All Sound Off
//---------------------------------------------------------
void MfInstrument::allSoundOff( void )
{
	Instrument::allSoundOff();
	
	for ( int i=0; i<128; i++ ){
		_konCount[i] = 0;
	}
}

