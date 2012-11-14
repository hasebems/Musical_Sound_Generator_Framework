//
//  msgf_part.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/09/23.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_part.h"
#include "msgf_voice_context.h"
#include "msgf_if.h"

using namespace msgf;

extern int voiceContext_getVoiceId( Uint8 msb, Uint8 lsb, Uint8 pcNum );

//---------------------------------------------------------
//		Part Constructor/Destructor
//---------------------------------------------------------
Part::Part( Msgf* ptg ) :
	_tg(ptg),
	_inst(0),
	_cc0_msb(0),
	_cc32_lsb(0),
	_cc1_modulationWheel(0),
	_cc7_volume(100),
	_cc10_pan(64),
	_cc11_expression(127),
	_cc64_sustain(0),
	_cc66_sostenuto(0),
	_cc65_portamento(0),
	_cc5_portamentoTime(0),
	_cc126_mono(1),
	_programNumber(0),
	_pitchBendValue(0)
{
	int voiceId = ptg->vc()->getVoiceId( 0, 0, 0 );
	_inst = new Instrument( this, voiceId );
}
//---------------------------------------------------------
Part::~Part( void )
{
	if ( _inst != 0 ) delete _inst;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void Part::keyOn( int note, int velocity )
{
	_inst->keyOn( note, velocity );
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void Part::keyOff( int note, int velocity )
{
	_inst->keyOff( note, velocity );
}

//---------------------------------------------------------
//		Controll Change
//---------------------------------------------------------
void Part::controlChange( int controller, int value )
{
	switch ( controller ){
		case 0:		_cc0_msb = value; break;
		case 1:		_cc1_modulationWheel = value; break;
		case 7:		_cc7_volume = value; break;
		case 10:	_cc10_pan = value; break;
		case 11:	_cc11_expression = value; break;
		case 32:	_cc32_lsb = value; break;
		case 5:		_cc5_portamentoTime = value; break;
		case 64:{
			_cc64_sustain = value;
			_inst->sustain( value );
			break;
		}
		case 65:	_cc65_portamento = value; break;
		case 66:	_cc66_sostenuto = value; break;
		default: break;
	}
}

//---------------------------------------------------------
//		Program Change
//---------------------------------------------------------
void Part::programChange( int number )
{
	_programNumber = number;
	
	_inst->allSoundOff();
	if ( _inst != 0 ) delete _inst;
	int voiceId = _tg->vc()->getVoiceId( _cc0_msb, _cc32_lsb, number );
	_inst = new Instrument( this, voiceId );
}

//---------------------------------------------------------
//		Pitch Bend
//---------------------------------------------------------
void Part::pitchBend( int value )
{
	_pitchBendValue = value;
}
