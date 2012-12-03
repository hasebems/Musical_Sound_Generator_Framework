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
void Oscillator::init( void )
{
	_waveform = _parentNote->getVoiceContext()->getParameter( VP_WAVEFORM );
	_pitch = calcPitch( _parentNote->getNote() );
	calcPegPitch( _pitch );

	_crntPhase = 0;
	_pegStartLevel = 0;
	_pegCrntLevel = 0;
	_pegLevel = 0;
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
void Oscillator::calcPegPitch( double pch )
{
	double pttmp, ratio = log(PEG_DEPTH_MAX)/PEG_MAX;
	int	i;

	ratio = exp(ratio);
	pttmp = pch;
	for ( i=0; i<PEG_MAX; i++ ){
		pttmp = pttmp*ratio;
		_upper[i] = pttmp;
	}
	
	ratio = -log(PEG_DEPTH_MAX)/PEG_MAX;
	ratio = exp(ratio);
	pttmp = pch;
	for ( i=0; i<PEG_MAX; i++ ){
		pttmp = pttmp*ratio;
		_lower[i] = pttmp;
	}
}

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Oscillator::toAttack( void )
{
	//	time
	_state = ATTACK;
	_egStartDac = _dacCounter = 0;
	_egTargetDac = _egStartDac
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_PEG_ATTACK_TIME));

	//	level
	_pegLevel = _pegStartLevel = _parentNote->getVoiceContext()->getParameter(VP_PEG_ATTACK_LEVEL);
}
//---------------------------------------------------------
void Oscillator::toSteady( void )
{
	//	time
	_state = KEY_ON_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;

	//	level
	_pegLevel = 0;
}
//---------------------------------------------------------
void Oscillator::toRelease( void )
{
	//	time
	_state = RELEASE;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_PEG_RELEASE_TIME));

	//	level
	_pegLevel = _parentNote->getVoiceContext()->getParameter(VP_PEG_RELEASE_LEVEL);
	_pegStartLevel = _pegCrntLevel;
}

//---------------------------------------------------------
//		Get PEG Current Pitch
//---------------------------------------------------------
double Oscillator::getFegCurrentPitch( void )
{
	long time = _dacCounter-_egStartDac;
	long targetTime = _egTargetDac-_egStartDac;
	
	if ( targetTime == 0 ) return _pitch;
	if ( time >= targetTime ) time = targetTime-1;
	
	if ( _state == ATTACK ){
		_pegCrntLevel = (targetTime-1-time)*_pegLevel/targetTime;
	}
	else if ( _state == RELEASE ){
		_pegCrntLevel = time*(_pegLevel-_pegStartLevel)/targetTime + _pegStartLevel;
	}
	
	if ( _pegCrntLevel > 0 ) return _upper[_pegCrntLevel];
	else if ( _pegCrntLevel < 0 ) return _lower[-_pegCrntLevel];
	return _pitch;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Oscillator::process( TgAudioBuffer& buf )
{
	if ( _state == EG_NOT_YET ){
		if ( _parentNote->conditionKeyOn() == true ){
			//	Start key On
			toAttack();
		}
	}
	else if ( _state <= KEY_ON_STEADY ){
		if ( _parentNote->conditionKeyOn() == false ){
			//	Key Off
			toRelease();
		}
	}
	
	if ( _state != EG_NOT_YET ){

		if ( _dacCounter >= _egTargetDac ){
			switch (_state){
				case ATTACK: toSteady(); break;
				default: break;
			}
		}
		double	pch = getFegCurrentPitch();
		double	diff = (2 * M_PI * pch )/ SMPL_FREQUENCY;

		switch ( _waveform ){
			default:
			case SINE		: generateSine(buf,diff); break;
			case TRIANGLE	: generateTriangle(buf,diff); break;
			case SAW		: generateSaw(buf,diff); break;
			case SQUARE		: generateSquare(buf,diff); break;
			case PULSE		: generatePulse(buf,diff); break;
		}
		_dacCounter += buf.bufferSize();
	}
}

//---------------------------------------------------------
//		Generate Wave
//---------------------------------------------------------
void Oscillator::generateSine( TgAudioBuffer& buf, double diff )
{
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	write Sine wave
		buf.setAudioBuffer( i, sin(_crntPhase) );
		_crntPhase += diff;
	}	
}
//---------------------------------------------------------
void Oscillator::generateTriangle( TgAudioBuffer& buf, double diff )
{
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
void Oscillator::generateSaw( TgAudioBuffer& buf, double diff )
{
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
void Oscillator::generateSquare( TgAudioBuffer& buf, double diff )
{
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
void Oscillator::generatePulse( TgAudioBuffer& buf, double diff )
{
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

