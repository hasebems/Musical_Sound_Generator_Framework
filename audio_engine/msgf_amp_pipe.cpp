//
//  msgf_amp_pipe.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/12/08.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_amp_pipe.h"
#include "msgf_audio_buffer.h"
#include "msgf_instrument.h"
#include "msgf_part.h"

using namespace msgf;

//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
AmpPipe::AmpPipe( Note& parent ):
_parentNote(parent),
_targetVol(0),
_realVol(0)
{
	_cbInst = new AegPCallBack( this );
	_eg = new Eg2segment( *_cbInst, parent, false );
	_am = new Lfo();
}
//---------------------------------------------------------
AmpPipe::~AmpPipe( void )
{
	delete _cbInst;
	delete _eg;
	delete _am;
}
//---------------------------------------------------------
void AmpPipe::init( void )
{
	clearDacCounter();
	
	//	LFO Settings only for Amplitude
	_am->setWave(LFO_TRI);
	_am->setDirection(LFO_LOWER);
	_am->setCoef();
	_am->start();
	_amd = static_cast<double>(getVoicePrm(VP_LFO_AMD))/AMP_PRM_MAX;

	_startNcEgDac = -1;
}

//---------------------------------------------------------
void AmpPipe::changeNote( void )
{
	_startNcEgDac = _dacCounter;
}

//---------------------------------------------------------
//		Calculate Note Change EG
//---------------------------------------------------------
const int 		NCEG_DOWN_TIME = 1000;		//	*dac count
const int		NCEG_MIN_TIME  = 800;		//	*dac count
const int 		NCEG_UP_TIME   = 3000;		//	*dac count
const double	NCEG_MIN_LVL   = 0.1f;
//---------------------------------------------------------
double AmpPipe::calcNcEg( double amp )
{
	if ( _startNcEgDac == -1 ) return amp;

	long	counterDiff = _dacCounter - _startNcEgDac;
	
	if ( _dacCounter > _startNcEgDac+NCEG_UP_TIME+NCEG_MIN_TIME+NCEG_DOWN_TIME ){
		_startNcEgDac = -1;
		return amp;
	}
	else if ( _dacCounter > _startNcEgDac+NCEG_MIN_TIME+NCEG_DOWN_TIME ){
		//	Level Up
		double tpdf = (counterDiff-NCEG_DOWN_TIME)*((1-NCEG_MIN_LVL)/NCEG_UP_TIME);
		if ( tpdf > 1-NCEG_MIN_LVL ) return amp;
		else return amp*(NCEG_MIN_LVL + tpdf);
	}
	else if ( _dacCounter > _startNcEgDac+NCEG_DOWN_TIME ){
		return amp*NCEG_MIN_LVL;
	}
	else if ( _dacCounter > _startNcEgDac ){
		//	Level Down
		return amp*(1 - counterDiff*((1-NCEG_MIN_LVL)/NCEG_DOWN_TIME));
	}

	return amp;
}

//---------------------------------------------------------
//		Calculate MIDI Volume
//---------------------------------------------------------
double AmpPipe::calcMidiVolume( double amp )
{
	Part*	pt = _parentNote.getInstrument()->getPart();
	Uint8 midiVal = pt->getCc7();
	amp = (amp*midiVal)/127;
	midiVal = pt->getCc11();
	amp = (amp*midiVal)/127;
	
	return amp;
}

//---------------------------------------------------------
//		Interporate Volume
//---------------------------------------------------------
const double DEEMED_SAME_VOLUME = 1.0001;
const double VOLUME_ITP_RATE = 0.01;			//	/1Dac
//---------------------------------------------------------
void AmpPipe::interporateVolume( void )
{
	if (( _targetVol*DEEMED_SAME_VOLUME >= _realVol ) &&
		( _targetVol/DEEMED_SAME_VOLUME <= _realVol )){
		_realVol = _targetVol;
	}
	else {
		if ( _targetVol > _realVol ){
			_realVol = _realVol + (_targetVol-_realVol)*VOLUME_ITP_RATE;
		}
		else {
			_realVol = _realVol - (_realVol-_targetVol)*VOLUME_ITP_RATE;
		}
	}
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void AmpPipe::process( TgAudioBuffer& buf )
{
	//	check Event
	_eg->periodicOnceEveryProcess();
	
	//	get LFO pattern
	double*	lfoBuf = new double[buf.bufferSize()];
	_am->process( buf.bufferSize(), lfoBuf );
	
	double	processVol = getVoicePrm(VP_VOLUME);
	processVol = calcMidiVolume(processVol);
	
	//	write Buffer
	for ( int i=0; i<buf.bufferSize(); i++ ){
		
		//	Check AEG Segment
		_eg->periodicOnceEveryDac(_dacCounter);
		
		//	calc real amplitude
		double tempAmp = _eg->calcEgLevel();
		
		//	LFO
		tempAmp = (_amd*lfoBuf[i]+1)*tempAmp;
		
		//	Note Change EG
		tempAmp = calcNcEg( tempAmp );
		
		//	calculate Volume
		_targetVol = tempAmp * (processVol/AMP_PRM_MAX);
		
		//	volume interporate
		interporateVolume();

		//	reflect volume to buffer
		buf.mulAudioBuffer( i, _realVol*_realVol );
		_dacCounter++;
	}
	
	delete[] lfoBuf;
}
