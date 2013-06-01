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
	_instFactory = new InstrumentFactory();
	int voiceId = getInstrumentId( 0, 0, 0 );
	_inst = _instFactory->getInstrument( this, voiceId );
}
//---------------------------------------------------------
Part::~Part( void )
{
	if ( _inst != 0 ) delete _inst;
	delete _instFactory;
}

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void Part::keyOn( int note, int velocity )
{
	if ( _inst ) _inst->keyOn( note, velocity );
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void Part::keyOff( int note, int velocity )
{
	if ( _inst ) _inst->keyOff( note, velocity );
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
			if ( _inst ) _inst->sustain( value );
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
	
	if ( _inst ){
		_inst->allSoundOff();
	 	delete _inst;
	}
	int voiceId = getInstrumentId( _cc0_msb, _cc32_lsb, number );
	_inst = _instFactory->getInstrument( this, voiceId );
}

//---------------------------------------------------------
//		Pitch Bend
//---------------------------------------------------------
void Part::pitchBend( int value )
{
	_pitchBendValue = value;
}
