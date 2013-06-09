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
#include "msgf_audio_buffer.h"
#include "msgf_level_check.h"

using namespace msgf;

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void Instrument::appendNoteList( Note* nt )
{
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
	if ( kfNote ) kfNote->keyOff();
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
void Instrument::releaseNote( const Note* nt )
{
	if ( nt == _topNote ) _topNote = nt->getNextNote();
	if ( nt == _endNote ) _endNote = nt->getPrevNote();

	_noteCounter--;
}

//---------------------------------------------------------
//		Search Minimum Level Note except during Damp
//---------------------------------------------------------
double Instrument::searchMinimumLevelNote( Note** nt )
{
#if 0
	//	return an Note Object which is the minimum level.
	Note* ntPr = _topNote;
	double	minLvl = 1;
	
	while ( ntPr != 0 ){
		LevelCheck* lc = ntPr->getLvlChk();
		if ( lc ){
			double lvl = lc->getMaxLevel();
			if ( minLvl > lvl ){
				minLvl = lvl;
				*nt = ntPr;
			}
		}
		ntPr = ntPr->getNextNote();
	}
	return minLvl;
#endif
	//	return top Note Object as released note.
	Note* ntPr = _topNote;
	
	while ( ntPr != 0 ){
		if ( ntPr->conditionKeyOn() == false ){
			*nt = ntPr;
			goto CHK_LVL;
		}
		ntPr = ntPr->getNextNote();
	}	
	*nt = _topNote;

CHK_LVL:
	if ( *nt ){
		LevelCheck* lc = (*nt)->getLvlChk();
		if ( lc ){
			return lc->getMaxLevel();
		}
	}
	return 1;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Instrument::process( TgAudioBuffer& buf )
{
	Note* ntPr = _topNote;
	Note* ntTmp = 0;
	
	while ( ntPr != 0 ){
		TgAudioBuffer	nbuf;
		nbuf.obtainAudioBuffer(buf.bufferSize());

		if ( ntPr->process( nbuf ) == false ) ntTmp = ntPr;
		else ntTmp = 0;

		//	Add to Buffer and Check no sound
		bool noSound = buf.mixAndCheckNoSound(nbuf);
		nbuf.releaseAudioBuffer();
		
		if (( noSound == true ) && ( ntPr->conditionKeyOn() == false )){
			//	this will be released
			ntTmp = ntPr;
		}
		else ntTmp = 0;
		
		ntPr = ntPr->getNextNote();
		if ( ntTmp ) delete ntTmp;
	}
}

//---------------------------------------------------------
//		Protected
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

