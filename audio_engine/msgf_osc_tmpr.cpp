//
//  msgf_osc_tmpr.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2015/07/18.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#include "msgf_osc_tmpr.h"

#include <math.h>
#include "msgf_audio_buffer.h"
#include "msgf_note.h"
#include "msgf_part.h"

using namespace msgf;

//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
OscillatorTmpr::OscillatorTmpr( Note& parent ):
_parentNote(parent),
_callBackFunc(0)
{
}
//---------------------------------------------------------
OscillatorTmpr::~OscillatorTmpr( void )
{
}
//---------------------------------------------------------
void OscillatorTmpr::init( void )
{
	clearDacCounter();
	
	_waveform = getVoicePrm( VP_WAVEFORM );
	_pitch = calcPitch( _parentNote.getNote() );
	_crntPhase = 0;
}

//---------------------------------------------------------
//		Calculate Pitch
//---------------------------------------------------------
const double OscillatorTmpr::tPitchOfA[11] =
{
	//	-3     9     21  33   45   57   69   81    93    105   117
	13.75, 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080
};
//---------------------------------------------------------
double OscillatorTmpr::calcPitch( const Uint8 note )
{
	Part*	pt = _parentNote.getInstrument()->getPart();
	int ptTune = pt->getTune();
	int doremi, from_a, octave;
	
	//	determine doremi/octave
	if ( note >= 9 ){
		from_a = (note-9)%12;
		octave = (note-9)/12 + 1;
		if ( octave >= OCT_A_MAX ){
			octave = OCT_A_MAX;
		}
	}
	else {
		from_a = note+3;
		octave = 0;
	}
	
	//	calculate Frequency(pitch) of equal temp.
	double ap = tPitchOfA[octave];
	ap += ((double)ptTune)*pow(2,octave-5);
	
	double ratio = exp(log(2)/12);
	for ( int i=0; i<from_a; i++ ){
		ap *= ratio;
	}

	//	get temperament pitch from Instrument
	doremi = from_a-3;
	if ( doremi < 0 ){ doremi+=12;}
	double centDiff = 0;
	if ( _callBackFunc != 0 ){
		Uint8 velVari = 3 - _parentNote.getVelocity() % 4;
		centDiff = _callBackFunc->getCent( doremi, velVari );
	}

	//	calculate temperament
	double diff = centDiff*log(2)/1200;
	ap *= exp(diff);

	return ap;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void OscillatorTmpr::process( TgAudioBuffer& buf )
{
	switch ( _waveform ){
		default:
		case SINE		: generateSine(buf); break;
		case TRIANGLE	: generateTriangle(buf); break;
		case SAW		: generateSaw(buf); break;
		case SQUARE		: generateSquare(buf); break;
		case PULSE		: generatePulse(buf); break;
	}
	
	_dacCounter += buf.bufferSize();
}

//---------------------------------------------------------
//		Generate Wave
//---------------------------------------------------------
#define 	ABORT_FREQUENCY			12000
//---------------------------------------------------------
void OscillatorTmpr::generateSine( TgAudioBuffer& buf )
{
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Sine wave
		buf.setAudioBuffer( i, sin(_crntPhase) );
		_crntPhase += (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
	}
}
//---------------------------------------------------------
void OscillatorTmpr::generateTriangle( TgAudioBuffer& buf )
{
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Triangle wave
		double amp, ps = fmod(_crntPhase,(2*M_PI))/(2*M_PI);
		if ( ps < 0.5 ) amp = 2*ps - 0.5;
		else amp = 2 - 2*ps;
		buf.setAudioBuffer( i, amp );
		_crntPhase += (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
	}
}
//---------------------------------------------------------
void OscillatorTmpr::generateSaw( TgAudioBuffer& buf )
{
	int maxOverTone = ABORT_FREQUENCY/_pitch;
	
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Saw wave
		double saw = 0;
		for ( int j=1; j<maxOverTone; j++ ){
			saw += 0.25*sin(_crntPhase*j)/j;
		}
		buf.setAudioBuffer( i, saw );
		_crntPhase += (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
	}
}
//---------------------------------------------------------
void OscillatorTmpr::generateSquare( TgAudioBuffer& buf )
{
	int maxOverTone = ABORT_FREQUENCY/_pitch;
	
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Square wave
		double sqr = 0;
		for ( int j=1; j<maxOverTone; j+=2 ){
			sqr += 0.25*sin(_crntPhase*j)/j;
		}
		
		//double amp, ps = fmod(_crntPhase,(2*M_PI))/(2*M_PI);
		//if ( ps < 0.5 ) amp = 0.5;
		//else amp = -0.5;
		
		buf.setAudioBuffer( i, sqr );
		_crntPhase += (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
	}
}
//---------------------------------------------------------
void OscillatorTmpr::generatePulse( TgAudioBuffer& buf )
{
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Square wave
		double amp, ps = fmod(_crntPhase,(2*M_PI))/(2*M_PI);
		if ( ps < 0.1 ) amp = 0.5;
		else if ( ps < 0.2 ) amp = -0.5;
		else amp = 0;
		buf.setAudioBuffer( i, amp );
		_crntPhase += (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
	}
}
