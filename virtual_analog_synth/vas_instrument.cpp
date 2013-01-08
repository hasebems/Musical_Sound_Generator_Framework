//
//  vas_instrument.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_event_info.h"

#include "vas_instrument.h"
#include "vas_voice_context.h"
#include "vas_note.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
VasInstrument::VasInstrument( Part* pt, int vid ):
Instrument(pt,vid)
{
	_vc = new VasVoiceContext();
}
//---------------------------------------------------------
VasInstrument::~VasInstrument( void )
{
	delete _vc;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void VasInstrument::keyOn( Uint8 note, Uint8 velocity )
{
	Note* nt = new VasNote(this);
	EventInfo* ei = new EventInfo();
	
	ei->setVelocity( velocity );
	ei->setNote( note );
	
	if ( nt->keyOn( ei ) == true ){
		appendNoteList( nt );
	}
}
