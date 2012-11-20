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
#include "msgf_voice_context.h"
using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
Oscillator::Oscillator( Note* parent ):
_parentNote( parent ),
_state(NOT_YET),
_dacCounter(-1)
{
	_waveform = _parentNote->getVoiceContext()->getParameter( VP_WAVEFORM );
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Oscillator::process( TgAudioBuffer& buf )
{
	if ( _state == NOT_YET ){
		if ( _parentNote->conditionKeyOn() == true ){
			_dacCounter = 0;
			_crntPhase = 0;
			_state = STEADY_STATE;
		}
	}

	if ( _state == STEADY_STATE	){
		switch ( _waveform ){
			default:
			case SINE		: generateSine(buf,_crntPhase); break;
			case TRIANGLE	: generateTriangle(buf,_crntPhase); break;
			case SAW		: generateSaw(buf,_crntPhase); break;
			case SQUARE		: generateSquare(buf,_crntPhase); break;
			case PULSE		: generatePulse(buf,_crntPhase); break;
		}
		_dacCounter += buf.bufferSize();
	}
}

//---------------------------------------------------------
//		Calculate Pitch
//---------------------------------------------------------
const double Oscillator::tPitchOfA[11] =
{
//	-3     9     21  33   45   57   69   81    93    105   117
	13.75, 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080
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
void Oscillator::generateSine( TgAudioBuffer& buf, double phase )
{
	double	diff = 0;

	//	Calclate pitch
	_pitch = calcPitch( _parentNote->getNote() );
	diff = (2 * M_PI * _pitch )/ SMPL_FREQUENCY;
	
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Sine wave
		buf.setAudioBuffer( i, sin(_crntPhase) );
		_crntPhase += diff;
	}	
}
//---------------------------------------------------------
void Oscillator::generateTriangle( TgAudioBuffer& buf, double phase )
{
	double	diff = 0;
	
	//	Calclate pitch
	_pitch = calcPitch( _parentNote->getNote() );
	diff = (2 * M_PI * _pitch )/ SMPL_FREQUENCY;
	
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Triangle wave
		double amp, ps = fmod(_crntPhase,(2*M_PI))/(2*M_PI);
		if ( ps < 0.5 ) amp = 2*ps - 0.5;
		else amp = 2 - 2*ps;
		buf.setAudioBuffer( i, amp );
		_crntPhase += diff;
	}
}
//---------------------------------------------------------
void Oscillator::generateSaw( TgAudioBuffer& buf, double phase )
{
	double	diff = 0;
	
	//	Calclate pitch
	_pitch = calcPitch( _parentNote->getNote() );
	diff = (2 * M_PI * _pitch )/ SMPL_FREQUENCY;
	int maxOverTone = 20000/_pitch;

	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Saw wave
		double saw = 0;
		for ( int j=1; j<maxOverTone; j++ ){
			saw += 0.25*sin(_crntPhase*j)/j;
		}
		buf.setAudioBuffer( i, saw );
		_crntPhase += diff;
	}
}
//---------------------------------------------------------
void Oscillator::generateSquare( TgAudioBuffer& buf, double phase )
{
	double	diff = 0;
	
	//	Calclate pitch
	_pitch = calcPitch( _parentNote->getNote() );
	diff = (2 * M_PI * _pitch )/ SMPL_FREQUENCY;
	int maxOverTone = 20000/_pitch;

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
		_crntPhase += diff;
	}
}
//---------------------------------------------------------
void Oscillator::generatePulse( TgAudioBuffer& buf, double phase )
{
	double	diff = 0;
	
	//	Calclate pitch
	_pitch = calcPitch( _parentNote->getNote() );
	diff = (2 * M_PI * _pitch )/ SMPL_FREQUENCY;
	
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Square wave
		double amp, ps = fmod(_crntPhase,(2*M_PI))/(2*M_PI);
		if ( ps < 0.1 ) amp = 0.5;
		else if ( ps < 0.2 ) amp = -0.5;
		else amp = 0;
		buf.setAudioBuffer( i, amp );
		_crntPhase += diff;
	}
}

