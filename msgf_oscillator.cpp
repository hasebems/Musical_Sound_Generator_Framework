//
//  msgf_oscillator.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/23.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include <math.h>
#include "msgf_oscillator.h"
#include "msgf_audio_buffer.h"
#include "msgf_note.h"
using namespace msgf;

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Oscillator::process( TgAudioBuffer& buf )
{
	double	diff = 0;
	
	if ( _state == NOT_YET ){
		if ( _parentNote->conditionKeyOn() == true ){
			_dacCounter = 0;
			_crntPhase = 0;
			_state = STEADY_STATE;
		}
	}

	if ( _state == STEADY_STATE	){
		//	Calclate pitch
		_pitch = calcPitch( _parentNote->getNote() );
		diff = (2 * M_PI * _pitch )/ SMPL_FREQUENCY;

		for ( int i=0; i<buf.bufferSize(); i++ ){
			//	write wave
			buf.setAudioBuffer( i, generateWave(_crntPhase) );
			_crntPhase += diff;
		}
		_dacCounter += buf.bufferSize();
	}
}

//---------------------------------------------------------
//		Calculate Pitch
//---------------------------------------------------------
const double Oscillator::tPitchOfA[10] =
{
	27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080
};
//---------------------------------------------------------
double Oscillator::calcPitch( const Uint8 note )
{
	int toneName, octave;

	if ( note >= 9 ){
		toneName = (note-9)%12;
		octave = (note-9)/12 + 1;
	}
	else {
		toneName = note+3;
		octave = 0;
	}

	double ap = tPitchOfA[octave];
	double ratio = exp(log(2)/12);
	for ( int i=0; i<toneName; i++ ){
		ap *= ratio;
	}

	return ap;
}

//---------------------------------------------------------
//		Generate Wave
//---------------------------------------------------------
double Oscillator::generateWave( double phase )
{
	return sin(phase);
}
