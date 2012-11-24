//
//  msgf_filter.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/18.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include <math.h>
#include "msgf_filter.h"
#include "msgf_note.h"
#include "msgf_audio_buffer.h"
#include "msgf_voice_context.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
Filter::Filter( Note* parent ):
_parentNote(parent),
_dacCounter(-1),
_startDac(0),
_targetDac(0),
_fegStartLevel(0),
_fegCrntLevel(0),
_fegLevel(0),
_state(FEG_NOT_YET),
_x_m2(0),
_x_m1(0),
_y_m2(0),
_y_m1(0)
{
	int fc = _parentNote->getVoiceContext()->getParameter( VP_FILTER_CUTOFF );
	int reso = _parentNote->getVoiceContext()->getParameter( VP_FILTER_RESO );
	setCoef( fc, ((double)reso)/10 );
}

//---------------------------------------------------------
//		Calculate Coef
//---------------------------------------------------------
void Filter::setCoef( double fc, double qValue )
{
	setOneCoef( fc, qValue, _center );

	double fctmp, ratio = log(FEG_DEPTH_MAX)/FEG_MAX;
	ratio = exp(ratio);
	fctmp = fc;
	for ( int i=0; i<FEG_MAX; i++ ){
		fctmp = fctmp*ratio;
		setOneCoef( fctmp, qValue, _upper[i] );
	}

	ratio = -log(2)/FEG_MAX;
	ratio = exp(ratio);
	fctmp = fc;
	for ( int j=0; j<FEG_MAX; j++ ){
		fctmp = fctmp*ratio;
		setOneCoef( fctmp, qValue, _lower[j] );
	}
}

//---------------------------------------------------------
//		Calculate Coef
//---------------------------------------------------------
void Filter::setOneCoef( double fc, double qValue, Coef& cf )
{
	double freq = tan(M_PI*fc/SMPL_FREQUENCY)/(2.0*M_PI);
	double fc2 = freq*freq;
	double tmp = 1.0 + (2.0*M_PI*freq)/qValue + (4.0*M_PI*M_PI*fc2);
	
	cf._b0 = (4.0*M_PI*M_PI*fc2)/tmp;
	cf._b1 = (8.0*M_PI*M_PI*fc2)/tmp;
	cf._b2 = cf._b0;
	cf._a1 = (8.0*M_PI*M_PI*fc2 - 2.0)/tmp;
	cf._a2 = (1.0 - 2.0*M_PI*freq/qValue + 4.0*M_PI*M_PI*fc2)/tmp;
}

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Filter::toAttack( void )
{
	_state = FEG_ATTACK;
	_startDac = _dacCounter = 0;
	_targetDac = _startDac
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_FEG_ATTACK_TIME));
	_fegLevel = _fegStartLevel = _parentNote->getVoiceContext()->getParameter(VP_FEG_ATTACK_LEVEL);
}
//---------------------------------------------------------
void Filter::toSteady( void )
{
	_state = KEY_ON_STEADY;
	_startDac = _dacCounter;
	_targetDac = _startDac;
	_fegLevel = 0;
}
//---------------------------------------------------------
void Filter::toRelease( void )
{
	_state = FEG_RELEASE;
	_startDac = _dacCounter;
	_targetDac = _startDac
		+ getTotalDacCount(_parentNote->getVoiceContext()->getParameter(VP_FEG_RELEASE_TIME));
	_fegLevel = _parentNote->getVoiceContext()->getParameter(VP_FEG_RELEASE_LEVEL);
	_fegStartLevel = _fegCrntLevel;
}

//---------------------------------------------------------
//		Convert Time(10msec) to Dac count
//---------------------------------------------------------
int Filter::getTotalDacCount( int time )
{
	return time*(SMPL_FREQUENCY/100);
}

//---------------------------------------------------------
//		Get FEG Coef
//---------------------------------------------------------
Coef* Filter::getFegCoef( void )
{
	long time = _dacCounter-_startDac;
	long targetTime = _targetDac-_startDac;

	if ( targetTime == 0 ) return &_center;
	if ( time >= targetTime ) time = targetTime-1;

	if ( _state == FEG_ATTACK ){
		_fegCrntLevel = (targetTime-1-time)*_fegLevel/targetTime;
	}
	else if ( _state == FEG_RELEASE ){
		_fegCrntLevel = time*(_fegLevel-_fegStartLevel)/targetTime + _fegStartLevel;
	}
		
	if ( _fegCrntLevel > 0 ) return &_upper[_fegCrntLevel];
	else if ( _fegCrntLevel < 0 ) return &_lower[-_fegCrntLevel];
	return &_center;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Filter::process( TgAudioBuffer& buf )
{
	//	FEG State
	if ( _state == FEG_NOT_YET ){
		if ( _parentNote->conditionKeyOn() == true ){
			//	Start key On
			toAttack();
		}
	}
	if ( _state <= KEY_ON_STEADY ){
		if ( _parentNote->conditionKeyOn() == false ){
			//	Key Off
			toRelease();
		}
	}
	
	//	Filter Calculate
	for ( int i=0; i<buf.bufferSize(); i++ ){
		Coef* crntCf = &_center;

		if ( _dacCounter >= _targetDac ){
			switch (_state){
				case FEG_ATTACK: toSteady(); break;
				default: break;
			}
		}
		crntCf = getFegCoef();
		
		//	Calculate Filter
		double input = buf.getAudioBuffer(i);
		double output = crntCf->_b0*input + crntCf->_b1*_x_m1 + crntCf->_b2*_x_m2;
		output += -crntCf->_a1*_y_m1 - crntCf->_a2*_y_m2;
	
		//	Limmiter
		if ( output > 1 ) output = 1;
		else if ( output < -1 ) output = -1;
	
		_y_m2 = _y_m1;
		_y_m1 = output;
		_x_m2 = _x_m1;
		_x_m1 = input;

		buf.setAudioBuffer( i, output );
		_dacCounter++;
	}
}