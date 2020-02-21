//
//  msgf_instrument_for_check.cpp
//  ToneGenerator
//
//  Created by Hasebe Masahiko on 2013/05/25.
//  Copyright (c) 2013 Hasebe Masahiko. All rights reserved.
//

#include <math.h>
#include "msgf_instrument_for_check.h"
#include "msgf_audio_buffer.h"

using namespace msgf;

//---------------------------------------------------------
//		Key On
//---------------------------------------------------------
void InstrumentForCheck::keyOn( Uint8 note, Uint8 velocity )
{
	_crntPhase = 0;
	_diff = (2 * M_PI * 880 )/ SAMPLING_FREQUENCY;
	_soundOn = true;
}

//---------------------------------------------------------
//		Key Off
//---------------------------------------------------------
void InstrumentForCheck::keyOff( Uint8 note, Uint8 velocity )
{
	_soundOn = false;
}

//---------------------------------------------------------
//		process
//---------------------------------------------------------
void InstrumentForCheck::process( TgAudioBuffer& buf )
{
	if ( _soundOn == true ){
		for ( int i=0; i<buf.bufferSize(); i++ ){
			//	write Sine wave
			buf.setAudioBuffer( i, sin(_crntPhase) );
			_crntPhase += _diff;
		}
	}
	else {
		for ( int i=0; i<buf.bufferSize(); i++ ){
			buf.setAudioBuffer( i, 0 );
		}
	}
}
