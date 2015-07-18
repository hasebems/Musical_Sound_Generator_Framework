//
//  msgf_part.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/09/23.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_part__
#define __msgf_part__

#include <iostream>
#include "msgf_type.h"
#include "msgf_instrument.h"
#include "msgf_configuration.h"

namespace msgf {
	//---------------------------------------------------------
	class Msgf;
	class TgAudioBuffer;
	//---------------------------------------------------------
	class Part {

	public:
		//	Constructor/Destructor
		Part( Msgf* ptg );
		~Part( void );

		//	IF Function
		void	keyOn( int velocity, int note );
		void	keyOff( int velocity, int note );
		void	controlChange( int controller, int value );
		void	programChange( int number );
		void	pitchBend( int valule );
		void	specialCommand( Uint8 p0, Uint8 p1, Uint8 p2 ) const { if (_inst){ _inst->specialCommand(p0,p1,p2);} }
		int		getInstrumentId( Uint8 msb, Uint8 lsb, Uint8 pcNum ) const { return 16384*msb + 128*lsb + pcNum;}

		//	process thread
		void	process( TgAudioBuffer& buf ){ _inst->process( buf ); }

		//	Accessor
		Msgf*	tgObj( void ) const { return _tg; }
		Uint8	getCc7( void ) const { return _cc7_volume; }
		Uint8	getCc11( void ) const { return _cc11_expression; }
		Uint8	getCc1( void ) const { return _cc1_modulationWheel; }
		Uint8	getCc5( void ) const { return _cc5_portamentoTime; }
		int		getNoteShift( void ) const { return (int)_cc12_noteShift-64; }
		int		getTune( void ) const { return (int)_cc13_tune-64; }
		Instrument*	getInstrument( void ) const { return _inst; }

	private:
		//	Part Latest Parameter Value
		Uint8	_cc0_msb;
		Uint8	_cc1_modulationWheel;
		Uint8	_cc5_portamentoTime;
		Uint8	_cc7_volume;
		Uint8	_cc10_pan;
		Uint8	_cc11_expression;
		Uint8	_cc12_noteShift;	//	out of MIDI
		Uint8	_cc13_tune;			//	out of MIDI
		Uint8	_cc32_lsb;
		Uint8	_cc64_sustain;
		Uint8	_cc65_portamento;
		Uint8	_cc66_sostenuto;
		Uint8	_cc126_mono;
		Uint8	_programNumber;
		int		_pitchBendValue;
		
		//	Composite Object
		InstrumentFactory* _instFactory;
		Instrument*	_inst;

		//	Association
		Msgf*	_tg;
	};
}
#endif /* defined(__msgf_part__) */
