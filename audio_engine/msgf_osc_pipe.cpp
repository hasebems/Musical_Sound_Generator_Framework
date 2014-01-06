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
	
	_note = _parentNote.getNote() + getVoicePrm(VP_TRANSPOSE);
	_pitch = calcPitch( _note );
	if ( phaseReset == true ) _crntPhase = 0;
	_prtmState = NO_MOVE;
	_carrierFreq = getVoicePrm(VP_CARRIER_FREQ);
	_carrierLevel = static_cast<double>(getVoicePrm(VP_CARRIER_LEVEL))/100;

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
	int		prtmDiff = getVoicePrm(VP_PORTAMENTO_DIFF);

	newNote = _note = _parentNote.getNote() + getVoicePrm(VP_TRANSPOSE);
	_sourcePitch = _pitch;

	switch ( _prtmState ){
		case NO_MOVE: _portamentoCounter = 0;	// go below
		case WAITING_PRTM:{
			_prtmState = WAITING_PRTM;
			_targetPitch = _pitch;
			break;
		}
		case FAST_MOVE:
		case SLOW_MOVE:{
			double tgtPitch = calcPitch(newNote);
			double tgtCent = 1200*log(tgtPitch/_pitch)/log(2);
			if (( tgtCent > prtmDiff*100 ) || ( tgtCent < -prtmDiff*100 )){
				_prtmState = FAST_MOVE;
				if ( tgtCent > 0 ){	//	Upper
					_targetPitch = calcPitch( _note - prtmDiff );
				}
				else {				//	Lower
					_targetPitch = calcPitch( _note + prtmDiff );
				}
			}
			else {
				_portamentoCounter = 0;
				_prtmState = SLOW_MOVE;
				_targetPitch = calcPitch( newNote );
				if ( tgtCent > 0 ){	//	Upper
					setPortamentoCounter(tgtCent);
				}
				else {				//	Lower
					setPortamentoCounter(-tgtCent);
				}
			}
			break;
		}
		default: break;
	}
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

	//	calculate tuning
	int	tune = getVoicePrm(VP_TUNING);
	if ( tune != 0 ){
		double fct = tune*log(2)/1200;
		ap *= exp(fct);
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
//		Calcrate Delta considering LFO
//---------------------------------------------------------
double OscPipe::calcDeltaLFO( double lfoDpt, double diff )
{
	return (1+(lfoDpt*_pmd))*diff;	// add LFO pattern
}

//---------------------------------------------------------
//		Manage Portamento State
//---------------------------------------------------------
void OscPipe::stateOfWaitingPortamento( void )
{
	int		waitingDcnt = getVoicePrm(VP_WAITING_DCNT);
	int		prtmDiff = getVoicePrm(VP_PORTAMENTO_DIFF);
	
	if ( waitingDcnt <= _portamentoCounter ){
		//	Move to next state
		Uint8	tgtNt;
		double tmpPitch = calcPitch(_note);
		double tgtCent = 1200*log(tmpPitch/_pitch)/log(2);
		if ( _sourcePitch > tmpPitch ){		//	Move Down
			if ( tgtCent > prtmDiff*(-100) ){
				_targetPitch = tmpPitch;
				_prtmState = SLOW_MOVE;
				setPortamentoCounter(prtmDiff*100);
			}
			else {
				tgtNt = _note + prtmDiff;
				_targetPitch = calcPitch( tgtNt );
				_prtmState = FAST_MOVE;
			}
		}
		else {	//	Move Up
			if ( tgtCent < prtmDiff*100 ){
				_targetPitch = tmpPitch;
				_prtmState = SLOW_MOVE;
				setPortamentoCounter(prtmDiff*100);
			}
			else {
				tgtNt = _note - prtmDiff;
				_targetPitch = calcPitch( tgtNt );
				_prtmState = FAST_MOVE;
			}
		}
		_portamentoCounter = 0;
		_sourcePitch = _pitch;
	}
}
//---------------------------------------------------------
void OscPipe::stateOfFastMove( void )
{
	int		fastMvDcnt = getVoicePrm(VP_FAST_MOVE_DCNT);
	int		prtmDiff = getVoicePrm(VP_PORTAMENTO_DIFF);

	if ( fastMvDcnt <= _portamentoCounter ){
		//	Move to next state
		_prtmState = SLOW_MOVE;
		_portamentoCounter = 0;
		_sourcePitch = _pitch;
		_targetPitch = calcPitch( _note );
		setPortamentoCounter(prtmDiff*100);
	}
	else{
		_pitch = _sourcePitch + ((_targetPitch - _sourcePitch)*_portamentoCounter)/fastMvDcnt;
	}
}
//---------------------------------------------------------
void OscPipe::stateOfSlowMove( void )
{
	if ( _portamentoCounter > _maxPortamentoCounter ){
		//	Move to next state
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
}
//---------------------------------------------------------
void OscPipe::managePortamentoState( void )
{
	switch ( _prtmState ){
		case WAITING_PRTM:
			stateOfWaitingPortamento();
			break;
			
		case FAST_MOVE:
			stateOfFastMove();
			break;

		case SLOW_MOVE:
			stateOfSlowMove();
			break;

		default: break;
	}

	_portamentoCounter++;
}

//---------------------------------------------------------
//		Set Portamento Counter / _targetCent
//---------------------------------------------------------
void OscPipe::setPortamentoCounter( double centDiff )
{
	int		prtm = getVoicePrm(VP_PORTAMENTO);
	
	if ( getVoicePrm(VP_PORTAMENTO_MODE) ){
		//	time constant
		_maxPortamentoCounter = (prtm*SAMPLING_FREQUENCY)/100;
	}
	else {
		//	rate constant
		_maxPortamentoCounter = (centDiff*prtm*SAMPLING_FREQUENCY)/(100*100);
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
	//	FM
	return 0.5*sin(phase + _carrierLevel*sin(phase*_carrierFreq));
}
//---------------------------------------------------------
//double OscPipe::generateWave( double phase )
//{
//	//	Sine Summing
//	double wave = 0;
//	for ( int j=1; j<4; j++ ){
//		wave += 0.5*sin(phase*j)/(j*j);
//	}
//
//	return wave;
//}
