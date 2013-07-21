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
Instrument(pt,vid)
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
	EventInfo* ei = new EventInfo();
	
	ei->setVelocity( velocity );
	ei->setNote( note );
	
	if ( _topNote != 0 ){
		if ( _topNote->conditionKeyOn() == true ){
			//	Change Note
			_topNote->changeNote( ei );
		}
		else {
			//
		}
	}
	else {
		Note* nt = new MfNote(this);
		if ( nt->keyOn( ei ) == true ){
			//	New Key On
			appendNoteList( nt );
		}
	}
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void MfInstrument::keyOff( Uint8 note, Uint8 velocity )
{
	Note* kfNote = searchNote( note, DURING_KON );
	if ( kfNote ) kfNote->keyOff();
}
