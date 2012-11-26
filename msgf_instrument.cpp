//
//  msgf_instrument.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/08.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_instrument.h"
#include "msgf_if.h"
#include "msgf_part.h"
#include "msgf_note.h"
#include "msgf_event_info.h"
#include "msgf_voice_context.h"
using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
Instrument::Instrument( Part* pt, int vid ) :
	_parentPart(pt),
	_voiceId(vid),
	_topNote(0),
	_endNote(0),
	_vc(0),
	_noteCounter(0)
{
	_vc = _parentPart->tgObj()->vc();
}

//---------------------------------------------------------
//		Destructor
//---------------------------------------------------------
Instrument::~Instrument( void )
{
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void Instrument::keyOn( Uint8 note, Uint8 velocity )
{
	Note* nt = new Note(this);
	EventInfo* ei = new EventInfo();

	ei->setVelocity( velocity );
	ei->setNote( note );
	nt->keyOn( ei );

	if ( _topNote == 0 ){
		_topNote = nt;
	}
	_endNote = nt;
	_noteCounter++;
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void Instrument::keyOff( Uint8 note, Uint8 velocity )
{
	Note* kfNote = searchNote( note, DURING_KON );
	kfNote->keyOff();
}

//---------------------------------------------------------
//		Sustain
//---------------------------------------------------------
void Instrument::sustain( Uint8 value )
{
	Note* ntSus = _topNote;
	while ( ntSus != 0 ){
		ntSus->sustain( value );
		ntSus = ntSus->getNextNote();
	}
}

//---------------------------------------------------------
//		All Sound Off
//---------------------------------------------------------
void Instrument::allSoundOff( void )
{
	Note* ntDamp = _topNote;
	while ( ntDamp != 0 ){
		Note* ntNxt = ntDamp->getNextNote();
		ntDamp->damp();
		ntDamp = ntNxt;
	}
}

//---------------------------------------------------------
//		Release Note
//---------------------------------------------------------
void Instrument::releaseNote( Note* nt )
{
	if ( nt == _topNote ) _topNote = nt->getNextNote();
	if ( nt == _endNote ) _endNote = nt->getPrevNote();

	_noteCounter--;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Instrument::process( TgAudioBuffer& buf )
{
	Note* ntPr = _topNote;
	Note* ntTmp = 0;
	
	while ( ntPr != 0 ){
		if ( ntPr->process( buf ) == false ) ntTmp = ntPr;
		else ntTmp = 0;
		ntPr = ntPr->getNextNote();
		if ( ntTmp ) delete ntTmp;
	}
}

//---------------------------------------------------------
//		Private
//---------------------------------------------------------
Note* Instrument::searchNote( Uint8 note, CONDITION cd )
{
	if ( cd == DURING_KON ){
		Note* nt = _topNote;
		while ( nt != 0 ){
			if (( nt->getNote() == note ) && ( nt->conditionKeyOn() == true )){
				return nt;
			}
			nt = nt->getNextNote();
		}
	}
	return 0;
}

