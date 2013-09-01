//
//  msgf_amp2seg.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_amp2seg.h"
#include "msgf_audio_buffer.h"
#include "msgf_instrument.h"
#include "msgf_part.h"

using namespace msgf;

//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
Amp2seg::Amp2seg( Note& parent ):
_parentNote(parent),
_oldVol(0)
{
	_cbInst = new Aeg2CallBack( this );
	_eg = new Eg2segment( *_cbInst, parent, false );
	_am = new Lfo();
}
//---------------------------------------------------------
Amp2seg::~Amp2seg( void )
{
	delete _cbInst;
	delete _eg;
	delete _am;
}
//---------------------------------------------------------
void Amp2seg::init( void )
{
	clearDacCounter();
	
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
double Amp2seg::calcMidiVolume( double amp )
{
	Part*	pt = _parentNote.getInstrument()->getPart();
	Uint8 midiVal = pt->getCc7();
	amp = (amp*midiVal)/127;
	midiVal = pt->getCc11();
	amp = (amp*midiVal)/127;
	
	return amp;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Amp2seg::process( TgAudioBuffer& buf )
{
	//	check Event
	_eg->periodicOnceEveryProcess();
	
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
		double	vol = getVoicePrm(VP_VOLUME);
		vol = aeg * (vol/AMP_PRM_MAX);
		vol = calcMidiVolume( vol );

		//	volume interporate
		if (( _oldVol != 0 ) && ( _oldVol < vol )){
			if ( _oldVol*4 < vol ){
				vol = _oldVol*4;
			}
		}
		_oldVol = vol;
		
		buf.mulAudioBuffer( i, vol*vol );
		_dacCounter++;
	}
}