//
//  msgf_amplitude.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/31.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_amplitude.h"
#include "msgf_audio_buffer.h"
#include "msgf_instrument.h"
#include "msgf_part.h"

using namespace msgf;

//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
Amplitude::Amplitude( Note& parent ):
_parentNote(parent)
{
	_cbInst = new AegCallBack( this );
	_eg = new Eg4segment( *_cbInst, parent );
}
//---------------------------------------------------------
Amplitude::~Amplitude( void )
{
	delete _cbInst;
	delete _eg;
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
	_amd = static_cast<double>(getVoicePrm(VP_LFO_AMD))/AMP_PRM_MAX;
}

//---------------------------------------------------------
//		Calculate Volume
//---------------------------------------------------------
double Amplitude::calcVolume( double amp )
{
	double	vol = getVoicePrm(VP_VOLUME);
	amp *= (vol/AMP_PRM_MAX);

	Uint8 midiVal = _parentNote.getInstrument()->getPart()->getCc7();
	amp = (amp*midiVal)/127;
	midiVal = _parentNote.getInstrument()->getPart()->getCc11();
	amp = (amp*midiVal)/127;

	return amp;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Amplitude::process( TgAudioBuffer& buf )
{
	//	check Event
	_eg->periodicOnceEveryProcesses();

	//	get LFO pattern
	double*	lfoBuf = new double[buf.bufferSize()];
	_am->process( buf.bufferSize(), lfoBuf );

	//	write Buffer
	for ( int i=0; i<buf.bufferSize(); i++ ){

		//	Check AEG Segment
		_eg->periodicOnceEveryDac(_dacCounter);

		//	calc real amplitude
		double aeg = _eg->calcEgLevel();

		//	LFO
		aeg *= (_amd*lfoBuf[i]+1)*aeg;

		//	calculate Volume
		double vol = calcVolume( aeg );
		
		buf.mulAudioBuffer( i, vol*vol );
		_dacCounter++;
	}
}
