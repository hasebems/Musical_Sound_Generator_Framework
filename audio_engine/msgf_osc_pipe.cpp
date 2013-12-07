//
//  msgf_osc_pipe.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/12/01.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include <math.h>
#include "msgf_osc_pipe.h"
#include "msgf_audio_buffer.h"
#include "msgf_note.h"

using namespace msgf;

const int OscPipe::PRTM_SLOW_DIFF = 2;
const int OscPipe::PRTM_WAITING_TIME = 2000;		//	*dac count
const int OscPipe::PRTM_FAST_MOVE_TIME = 500;	//	*dac count


//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
OscPipe::OscPipe( Note& parent ):
_parentNote(parent)
{
	_pm = new Lfo();
}
//---------------------------------------------------------
OscPipe::~OscPipe( void )
{
	delete _pm;
}
//---------------------------------------------------------
void OscPipe::init( bool phaseReset )
{
	clearDacCounter();
	
	_waveform = getVoicePrm( VP_WAVEFORM );
	_note = _parentNote.getNote();
	_pitch = calcPitch( _note );
	if ( phaseReset == true ) _crntPhase = 0;
	_prtmState = NO_MOVE;
	
	//	LFO Settings as delegation who intend to use LFO
	_pm->setFrequency(static_cast<double>(getVoicePrm(VP_LFO_FREQUENCY))/10);
	_pm->setDelay(getVoicePrm(VP_LFO_DELAY_TIME));
	_pm->setFadeIn(getVoicePrm(VP_LFO_FADEIN_TIME));
	
	//	LFO Settings only for Pitch
	_pm->setWave(LFO_TRI);
	_pm->setCoef();
	_pm->start();
	_pmd = static_cast<double>(getVoicePrm(VP_LFO_PMD))/100;
}
//---------------------------------------------------------
void OscPipe::changeNote( void )
{
	Uint8	newNote;
	double	tgtCent;

	newNote = _note = _parentNote.getNote();
	tgtCent = 1200*log(calcPitch(newNote)/_pitch)/log(2);
	_sourcePitch = _pitch;

	if ( tgtCent > PRTM_SLOW_DIFF*100 ){
		//	Move Upper & Far
		if (( _prtmState == NO_MOVE ) || ( _prtmState == WAITING_PRTM )){
			_prtmState = WAITING_PRTM;
			_targetPitch = _pitch;
			if ( _prtmState == NO_MOVE ){
				_portamentoCounter = 0;
			}
		}
		else {
			_prtmState = FAST_MOVE;
			_targetPitch = calcPitch( _note - PRTM_SLOW_DIFF );
		}
	}

	else if ( tgtCent > 0 ){
		//	Move Upper & Slow
		_portamentoCounter = 0;
		_prtmState = SLOW_MOVE;
		_targetPitch = calcPitch( newNote );
		setPortamentoCounter(tgtCent);
	}

	else if ( tgtCent < -PRTM_SLOW_DIFF*100 ){
		//	Move Down & Far
		if (( _prtmState == NO_MOVE ) || ( _prtmState == WAITING_PRTM )){
			_prtmState = WAITING_PRTM;
			_targetPitch = _pitch;
			if ( _prtmState == NO_MOVE ){
				_portamentoCounter = 0;
			}
		}
		else {
			_prtmState = FAST_MOVE;
			_targetPitch = calcPitch( _note - PRTM_SLOW_DIFF );
		}
	}

	else {
		//	Move Down & Slow
		_portamentoCounter = 0;
		_prtmState = SLOW_MOVE;
		_targetPitch = calcPitch( newNote );
		setPortamentoCounter(-tgtCent);
	}
}

//---------------------------------------------------------
//		Calculate Pitch
//---------------------------------------------------------
const double OscPipe::tPitchOfA[11] =
{
	//	-3     9     21  33   45   57   69   81    93    105   117
	13.75, 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080
};
//---------------------------------------------------------
double OscPipe::calcPitch( const Uint8 note )
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
double OscPipe::getPegPitch( int depth )
{
	if ( depth == 0 ) return _pitch;
	
	double pttmp = _pitch;
	if ( depth > 0 ){
		double ratio = log(PEG_DEPTH_MAX)/PEG_MAX;
		ratio = exp(ratio);
		for ( int i=0; i<depth; i++ ){
			pttmp = pttmp*ratio;
		}
	}
	else {
		depth = 0-depth;
		double ratio = -log(PEG_DEPTH_MAX)/PEG_MAX;
		ratio = exp(ratio);
		for ( int i=0; i<depth; i++ ){
			pttmp = pttmp*ratio;
		}
	}
	
	return pttmp;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void OscPipe::process( TgAudioBuffer& buf )
{
	//	get LFO pattern
	double*	lfoBuf = new double[buf.bufferSize()];
	_pm->process( buf.bufferSize(), lfoBuf );
		
	//	Generate each samples
	for ( int i=0; i<buf.bufferSize(); i++ ){
		double	waveAmp = generateWave(_crntPhase);
			
		//	Write Audio Buffer
		buf.setAudioBuffer( i, waveAmp );
			
		//	calcurate Portamento
		if ( _prtmState != NO_MOVE ) managePortamentoState();
			
		//	Calculate next _crntPhase from Pitch
		double	diff = (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
		_crntPhase += calcDeltaLFO( lfoBuf[i], diff );
	}
		
	_dacCounter += buf.bufferSize();
		
	delete[] lfoBuf;
}

//---------------------------------------------------------
//		Calcrate Delta considering LFO
//---------------------------------------------------------
double OscPipe::calcDeltaLFO( double lfoDpt, double diff )
{
	return (1+(lfoDpt*_pmd))*diff;	// add LFO pattern
}

//---------------------------------------------------------
//		Manage Portamento State
//---------------------------------------------------------
void OscPipe::managePortamentoState( void )
{
	double	tmpPitch;
	Uint8	tgtNt;
	
	switch ( _prtmState ){
		case WAITING_PRTM:{
			if ( PRTM_WAITING_TIME <= _portamentoCounter ){
				_prtmState = FAST_MOVE;
				tmpPitch = calcPitch(_note);
				if ( _sourcePitch > tmpPitch ){	//	Move Down
					tgtNt = _note + PRTM_SLOW_DIFF;
				}
				else {	//	Move Upper
					tgtNt = _note - PRTM_SLOW_DIFF;
				}
				_portamentoCounter = 0;
				_sourcePitch = _pitch;
				_targetPitch = calcPitch( tgtNt );
			}
			break;
		}

		case FAST_MOVE:{
			if ( PRTM_FAST_MOVE_TIME <= _portamentoCounter ){
				_prtmState = SLOW_MOVE;
				_portamentoCounter = 0;
				_sourcePitch = _pitch;
				_targetPitch = calcPitch( _note );
				setPortamentoCounter(PRTM_SLOW_DIFF*100);
			}
			else{
				_pitch = _sourcePitch + ((_targetPitch - _sourcePitch)*_portamentoCounter)/PRTM_FAST_MOVE_TIME;
			}
			break;
		}

		case SLOW_MOVE:{
			if ( _portamentoCounter > _maxPortamentoCounter ){
				_prtmState = NO_MOVE;
				_pitch = _targetPitch;
			}
			else {
				if ( _targetCent == 0 ){
					//	freq. linear
					_pitch = _sourcePitch + ((_targetPitch - _sourcePitch)*_portamentoCounter)/_maxPortamentoCounter;
				}
				else {
					//	cent linear
					double _crntCent = _targetCent*_portamentoCounter/_maxPortamentoCounter;
					_pitch = exp(_crntCent*log(2)/1200)*_sourcePitch;
				}
			}
			break;
		}
		default: break;
	}

	_portamentoCounter++;
}

//---------------------------------------------------------
//		Set Portamento Counter / _targetCent
//---------------------------------------------------------
void OscPipe::setPortamentoCounter( double centDiff )
{
	if ( getVoicePrm(VP_PORTAMENTO_MODE) ){
		//	time constant
		_maxPortamentoCounter = (getVoicePrm(VP_PORTAMENTO)*SAMPLING_FREQUENCY)/100;
	}
	else {
		//	rate constant
		_maxPortamentoCounter = (centDiff*getVoicePrm(VP_PORTAMENTO)*SAMPLING_FREQUENCY)/(100*100);
	}
	
	if ( getVoicePrm(VP_PORTAMENTO_CURVE) == 0 ){
		//	cent linear
		_targetCent = 1200*log(_targetPitch/_sourcePitch)/log(2);
	}
	else _targetCent = 0;
}

//---------------------------------------------------------
//		Generate Wave
//---------------------------------------------------------
double OscPipe::generateWave( double phase )
{
	double wave = 0;
	for ( int j=1; j<4; j++ ){
		wave += 0.25*sin(phase*j)/(j+j);
	}
	
	return wave;
}
