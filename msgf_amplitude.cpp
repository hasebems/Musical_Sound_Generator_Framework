//
//  msgf_amplitude.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/31.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_amplitude.h"
#include "msgf_note.h"
#include "msgf_audio_buffer.h"
#include "msgf_voice_context.h"
#include "msgf_instrument.h"
#include "msgf_part.h"
using namespace msgf;

//---------------------------------------------------------
#define EG_LEVEL_MAX	100
#define EG_LEVEL_MIN	0

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Amplitude::toAttack( void )
{
	_state = ATTACK;
	_startDac = _dacCounter = 0;
	_targetDac = _startDac
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_AEG_ATTACK_TIME));
	_startLvl = EG_LEVEL_MIN;
	_targetLvl = EG_LEVEL_MAX;
}
//---------------------------------------------------------
void Amplitude::toDecay1( void )
{
	_state = DECAY1;
	_startDac = _dacCounter;
	_targetDac = _startDac +
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_AEG_DECAY1_TIME));
	_startLvl = EG_LEVEL_MAX;
	_targetLvl = _parentNote->getVoiceContext()->getParameter(VP_AEG_DECAY1_LEVEL);
}
//---------------------------------------------------------
void Amplitude::toDecay2( void )
{
	_state = DECAY2;
	_startDac = _dacCounter;
	_targetDac = _startDac +
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_AEG_DECAY2_TIME));
	_startLvl = _targetLvl;
	_targetLvl = _parentNote->getVoiceContext()->getParameter(VP_AEG_DECAY2_LEVEL);
}
//---------------------------------------------------------
void Amplitude::toDecay2Steady( void )
{
	_state = DECAY2_STEADY;
	_startDac = _dacCounter;
	_startLvl = _targetLvl;
}
//---------------------------------------------------------
void Amplitude::toRelease( void )
{
	_state = RELEASE;
	_startLvl = static_cast<int>(getAegLevel( _dacCounter-_startDac, _targetDac-_startDac, _startLvl, _targetLvl ));
	_startDac = _dacCounter;
	_targetDac = _startDac
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_AEG_RELEASE_TIME));
	_targetLvl = EG_LEVEL_MIN;
}

//---------------------------------------------------------
//		Convert Time(10msec) to Dac count
//---------------------------------------------------------
int Amplitude::getTotalDacCount( int time )
{
	return time*(SMPL_FREQUENCY/100);
}

//---------------------------------------------------------
//		Calculate AEG
//---------------------------------------------------------
double Amplitude::getAegLevel( long crntDac, long targetDac, int startLvl, int targetLvl )
{
	if ( targetDac == 0 ) return startLvl;
	return (double)((targetLvl - startLvl)*crntDac)/targetDac + startLvl;
}

//---------------------------------------------------------
//		Calculate Volume
//---------------------------------------------------------
double Amplitude::calcVolume( double amp )
{
	double	vol = _parentNote->getVoiceContext()->getParameter(VP_VOLUME);
	amp *= (vol/EG_LEVEL_MAX);

	Uint8 midiVal = _parentNote->getInstrument()->getPart()->getCc7();
	amp = (amp*midiVal)/127;
	midiVal = _parentNote->getInstrument()->getPart()->getCc11();
	amp = (amp*midiVal)/127;

	return amp;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Amplitude::process( TgAudioBuffer& buf )
{
	// check Event
	if ( _state == AMPLITUDE_NOT_YET ){
		if ( _parentNote->conditionKeyOn() == true ){
			//	Start key On
			toAttack();
		}
	}
	if ( _state <= DECAY2_STEADY ){
		if ( _parentNote->conditionKeyOn() == false ){
			//	Key Off
			toRelease();
		}
	}

	//	write Buffer
	for ( int i=0; i<buf.bufferSize(); i++ ){

		//	Change AEG Segment
		if ( _dacCounter >= _targetDac ){
			switch (_state){
				case ATTACK:
					toDecay1();
					break;
				case DECAY1:
					toDecay2();
					break;
				case DECAY2:{
					if ( _targetLvl != EG_LEVEL_MIN ){
					}
					break;
				}
				case RELEASE:
				default: break;
			}
		}

		//	calc real amplitude
		double aeg = getAegLevel( _dacCounter-_startDac, _targetDac-_startDac, _startLvl, _targetLvl );
		aeg /= EG_LEVEL_MAX;

		//	calculate Volume
		double vol = calcVolume( aeg );
		
		buf.mulAudioBuffer( i, vol*vol );
		_dacCounter++;
	}
}
