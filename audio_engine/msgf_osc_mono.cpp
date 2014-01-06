//
//  msgf_osc_mono.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include <math.h>
#include "msgf_osc_mono.h"
#include "msgf_audio_buffer.h"
#include "msgf_note.h"

using namespace msgf;

//---------------------------------------------------------
//		Initialize
//---------------------------------------------------------
OscMono::OscMono( Note& parent ):
_parentNote(parent)
{
	_cbInst = new PegCallBack( this );
	_eg = new Eg2segment( *_cbInst, parent, true );
	_pm = new Lfo();
}
//---------------------------------------------------------
OscMono::~OscMono( void )
{
	delete _pm;
	delete _cbInst;
	delete _eg;
}
//---------------------------------------------------------
void OscMono::init( bool phaseReset )
{
	clearDacCounter();
	
	_waveform = getVoicePrm( VP_WAVEFORM );
	_note = _parentNote.getNote() + getVoicePrm(VP_TRANSPOSE);
	_pitch = calcPitch( _note );
	if ( phaseReset == true ) _crntPhase = 0;
	_cndDuringPortamento = false;
	
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
void OscMono::changeNote( void )
{
	Uint8	oldNote, newNote;
	
	oldNote = _note;
	newNote = _note = _parentNote.getNote() + getVoicePrm(VP_TRANSPOSE);
	
	//	Analyze Portamento Difference
	int diff = getVoicePrm(VP_PORTAMENTO_DIFF);
	if ( diff == 0 ) _pitchOrg = _pitch;
	else {
		if ( newNote > oldNote ){
			if ( newNote - oldNote > diff ){
				oldNote = newNote - diff;
				_pitchOrg = calcPitch(oldNote);
			}
			else _pitchOrg = _pitch;
		}
		else {
			if ( oldNote - newNote > diff ){
				oldNote = newNote + diff;
				_pitchOrg = calcPitch(oldNote);
			}
			else _pitchOrg = _pitch;
		}
	}
	
	//	Set Variables
	_pitchTarget = calcPitch( newNote );
	_cndDuringPortamento = true;
	_portamentoCounter = 0;
	
	if ( getVoicePrm(VP_PORTAMENTO_MODE) ){
		//	time constant
		_maxPortamentoCounter = (getVoicePrm(VP_PORTAMENTO)*SAMPLING_FREQUENCY)/100;
	}
	else {
		//	rate constant
		Uint8	noteDiff;
		if ( newNote > oldNote ) noteDiff = newNote - oldNote;
		else noteDiff = oldNote - newNote;
		_maxPortamentoCounter = (noteDiff*getVoicePrm(VP_PORTAMENTO)*SAMPLING_FREQUENCY)/100;
	}

	if ( getVoicePrm(VP_PORTAMENTO_CURVE) == 0 ){
		_targetCent = 1200*log(_pitchTarget/_pitchOrg)/log(2);
	}
	else _targetCent = 0;
}

//---------------------------------------------------------
//		Calculate Pitch
//---------------------------------------------------------
const double OscMono::tPitchOfA[11] =
{
	//	-3     9     21  33   45   57   69   81    93    105   117
	13.75, 27.5, 55, 110, 220, 440, 880, 1760, 3520, 7040, 14080
};
//---------------------------------------------------------
double OscMono::calcPitch( const Uint8 note )
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
double OscMono::getPegPitch( int depth )
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
void OscMono::process( TgAudioBuffer& buf )
{
	//	check Event
	_eg->periodicOnceEveryProcess();
	
	if ( _eg->getEgState() != EG_NOT_YET ){

		//	Check EG Segment
		_eg->periodicOnceEveryDac( _dacCounter );
		
		//	Get EG Level & Pitch
		int egLvl = static_cast<int>(_eg->calcEgLevel() * PEG_MAX);
		_pitch = getPegPitch(egLvl);

		//	get LFO pattern
		double*	lfoBuf = new double[buf.bufferSize()];
		_pm->process( buf.bufferSize(), lfoBuf );

		//	Generate each samples
		for ( int i=0; i<buf.bufferSize(); i++ ){
			double	waveAmp;
			switch ( _waveform ){
				default:
				case SINE		: waveAmp = generateSine(_crntPhase); break;
				case TRIANGLE	: waveAmp = generateTriangle(_crntPhase); break;
			}
			
			//	Write Audio Buffer
			buf.setAudioBuffer( i, waveAmp );

			//	calcurate Portamento
			if ( _cndDuringPortamento ) calcPortamento();

			//	Calculate next _crntPhase from Pitch
			double	diff = (2 * M_PI * _pitch )/ SAMPLING_FREQUENCY;
			_crntPhase += calcDeltaLFO( lfoBuf[i], diff );
		}

		_dacCounter += buf.bufferSize();
		
		delete[] lfoBuf;
	}
}

//---------------------------------------------------------
//		Calcrate Delta considering LFO
//---------------------------------------------------------
double OscMono::calcDeltaLFO( double lfoDpt, double diff )
{
	return (1+(lfoDpt*_pmd))*diff;	// add LFO pattern
}

//---------------------------------------------------------
//		Calcrate Portamento
//---------------------------------------------------------
void OscMono::calcPortamento( void )
{
	if ( _targetCent == 0 ){
		_pitch = _pitchOrg + ((_pitchTarget - _pitchOrg)*_portamentoCounter)/_maxPortamentoCounter;
	}
	else {
		double _crntCent = _targetCent*_portamentoCounter/_maxPortamentoCounter;
		_pitch = exp(_crntCent*log(2)/1200)*_pitchOrg;
	}

	_portamentoCounter++;
	if ( _portamentoCounter > _maxPortamentoCounter ){
		_cndDuringPortamento = false;
		_pitch = _pitchTarget;
	}
}

//---------------------------------------------------------
//		Generate Wave
//---------------------------------------------------------
double OscMono::generateTriangle( double phase )
{
	//	write Triangle wave
	double amp, ps = fmod(phase,(2*M_PI))/(2*M_PI);
	if ( ps < 0.5 ) amp = 2*ps - 0.5;
	else amp = 2 - 2*ps;

	return amp;
}

