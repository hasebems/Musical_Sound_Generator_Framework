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
//		Initialize
//---------------------------------------------------------
Amplitude::Amplitude( Note* parent ):
	SignalProcessWithEG(parent),
	_crntAeg(0){}
//---------------------------------------------------------
Amplitude::~Amplitude( void )
{
	delete _am;
}
//---------------------------------------------------------
void Amplitude::init( void )
{
	//	LFO Construct
	_am = new Lfo();
	
	//	LFO Settings only for Amplitude
	_am->setWave(LFO_TRI);
	_am->setDirection(LFO_LOWER);
	_am->setCoef();
	_am->start();
	_amd = static_cast<double>(getVoicePrm(VP_LFO_AMD))/100;
}

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Amplitude::toAttack( void )
{
	//	time
	SignalProcessWithEG::toAttack();

	//	level
	_aegStartLevel = EG_LEVEL_MIN;
	_aegTargetLevel = EG_LEVEL_MAX;
}
//---------------------------------------------------------
void Amplitude::toDecay1( void )
{
	//	time
	SignalProcessWithEG::toDecay1();
	
	//	level
	_aegStartLevel = EG_LEVEL_MAX;
	_aegTargetLevel = getVoicePrm(VP_AEG_DECAY1_LEVEL);
}
//---------------------------------------------------------
void Amplitude::toDecay2( void )
{
	//	time
	SignalProcessWithEG::toDecay2();
	
	//	level
	_aegStartLevel = _aegTargetLevel;
	_aegTargetLevel = getVoicePrm(VP_AEG_DECAY2_LEVEL);
}
//---------------------------------------------------------
void Amplitude::toKeyOnSteady( void )
{
	//	time
	SignalProcessWithEG::toKeyOnSteady();

	//	level
	_aegStartLevel = _aegTargetLevel;
}
//---------------------------------------------------------
void Amplitude::toRelease( void )
{
	//	time
	SignalProcessWithEG::toRelease();

	//	level
	_aegStartLevel = static_cast<int>(_crntAeg);
	_aegTargetLevel = EG_LEVEL_MIN;
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
	double	vol = getVoicePrm(VP_VOLUME);
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
	//	check Event
	checkEvent();

	//	get LFO pattern
	double*	lfoBuf = new double[buf.bufferSize()];
	_am->process( buf.bufferSize(), lfoBuf );

	//	write Buffer
	for ( int i=0; i<buf.bufferSize(); i++ ){

		//	Check AEG Segment
		checkSegmentEnd();

		//	calc real amplitude
		double aeg = getAegLevel( _dacCounter-_egStartDac, _egTargetDac-_egStartDac, _aegStartLevel, _aegTargetLevel );
		_crntAeg = aeg;
		aeg /= EG_LEVEL_MAX;

		//	LFO
		aeg *= (_amd*lfoBuf[i]+1)*aeg;

		//	calculate Volume
		double vol = calcVolume( aeg );
		
		buf.mulAudioBuffer( i, vol*vol );
		_dacCounter++;
	}
}
