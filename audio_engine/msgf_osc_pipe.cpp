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
#include "msgf_part.h"

using namespace msgf;

//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
OscPipe::OscPipe( Note& parent ):
_parentNote(parent)
{
	_pm = new Lfo();
	_cent2pitch = log(2)/1200;
	_chromaticRatio = exp(log(2)/12);
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
	_pitchAdj = 1;
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
			double tgtCent = log(tgtPitch/_pitch)/_cent2pitch;
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

	//	reflect MIDI controller
	reflectMidiController();
	
	//	Generate each samples
	for ( int i=0; i<buf.bufferSize(); i++ ){
		double	waveAmp = generateWave(_crntPhase);
		
		//	Write Audio Buffer
		buf.setAudioBuffer( i, waveAmp );
		
		//	calcurate Portamento
		if ( _prtmState != NO_MOVE ) managePortamentoState();
		
		//	Calculate next _crntPhase from Pitch
		double	diff = (2 * M_PI * _pitch * _pitchAdj)/ SAMPLING_FREQUENCY;
		_crntPhase += calcDeltaLFO( lfoBuf[i] )*diff;
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
	int toneName, octave, realNote = note;

	if ( realNote >= 9 ){
		toneName = (realNote-9)%12;
		octave = (realNote-9)/12 + 1;
	}
	else {
		toneName = realNote+3;
		octave = 0;
	}
	
	double ap = tPitchOfA[octave];
	for ( int i=0; i<toneName; i++ ){
		ap *= _chromaticRatio;
	}

	//	calculate tuning
	int	tune = getVoicePrm(VP_TUNING);
	if ( tune != 0 ){
		double fct = tune*_cent2pitch;
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
//		Reflect MIDI Controller
//---------------------------------------------------------
#define EXPANSION_BEGINNING_VALUE		97
//---------------------------------------------------------
void OscPipe::reflectMidiController( void )
{
	Part*	pt = _parentNote.getInstrument()->getPart();

	//	Pitch Up by Expression
	Uint8	midiExp = pt->getCc11();
	double	expLvl = static_cast<double>(midiExp) - EXPANSION_BEGINNING_VALUE;
	if ( expLvl < 0 ) expLvl = 0;	//	expLvl : 0 - 30
	expLvl /= 2;					//	expLvl : 0 - 15(cent)

	//	Tune
	int		tune = pt->getTune();
	expLvl += tune;
	
	double fct = expLvl*_cent2pitch;
	_pitchAdj = exp(fct);

	//	Note Shift
	int noteDiff = pt->getNoteShift();
	if ( noteDiff < 0 ){
		for ( int i=0; i<noteDiff; i++ ){
			_pitchAdj *= _chromaticRatio;
		}
	}
	else {
		double ratio = -1*_chromaticRatio;
		for ( int i=0; i<(-noteDiff); i++ ){
			_pitchAdj *= ratio;
		}
	}
}

//---------------------------------------------------------
//		Calcrate Delta considering LFO
//---------------------------------------------------------
double OscPipe::calcDeltaLFO( double lfoDpt )
{
	Part*	pt = _parentNote.getInstrument()->getPart();
	double	pmd = static_cast<double>(pt->getCc1())/(128*10);
	return (1+(lfoDpt*pmd));	// add LFO pattern
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
		double tgtCent = log(tmpPitch/_pitch)/_cent2pitch;
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
			_pitch = exp(_crntCent*_cent2pitch)*_sourcePitch;
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
	Part*	pt = _parentNote.getInstrument()->getPart();
	int		prtm = static_cast<int>(pt->getCc5())/5;
	
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
		_targetCent = log(_targetPitch/_sourcePitch)/_cent2pitch;
	}
	else _targetCent = 0;
}

//---------------------------------------------------------
//		Generate Wave
//---------------------------------------------------------
double OscPipe::generateWave( double phase )
{
	//	Sine Wave
	return sin(phase);
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
