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
SignalProcessCore(parent),
_fegStartLevel(0),
_fegCrntLevel(0),
_fegLevel(0),
_x_m2(0),
_x_m1(0),
_y_m2(0),
_y_m1(0)
{}
//---------------------------------------------------------
void Filter::init( void )
{
	int fc = getVoicePrm( VP_FILTER_CUTOFF );
	int reso = getVoicePrm( VP_FILTER_RESO );
	setCoef( fc, ((double)reso)/10 );
}
//---------------------------------------------------------
Filter::~Filter( void )
{
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

	ratio = -log(FEG_DEPTH_MAX)/FEG_MAX;
	ratio = exp(ratio);
	fctmp = fc;
	for ( int j=0; j<FEG_MAX; j++ ){
		fctmp = fctmp*ratio;
		setOneCoef( fctmp, qValue, _lower[j] );
	}
}

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Filter::toAttack( void )
{
	//	time
	_state = ATTACK;
	_egStartDac = _dacCounter = 0;
	_egTargetDac = _egStartDac
		+ getTotalDacCount(getVoicePrm(VP_FEG_ATTACK_TIME));

	//	level
	_fegLevel = _fegStartLevel = getVoicePrm(VP_FEG_ATTACK_LEVEL);
}
//---------------------------------------------------------
void Filter::toSteady( void )
{
	//	time
	_state = KEY_ON_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;

	//	level
	_fegLevel = 0;
}
//---------------------------------------------------------
void Filter::toRelease( void )
{
	//	time
	_state = RELEASE;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac
		+ getTotalDacCount(getVoicePrm(VP_FEG_RELEASE_TIME));

	//	level
	_fegLevel = getVoicePrm(VP_FEG_RELEASE_LEVEL);
	_fegStartLevel = _fegCrntLevel;
}

//---------------------------------------------------------
//		Get FEG Coef
//---------------------------------------------------------
Coef* Filter::getFegCoef( void )
{
	long time = _dacCounter-_egStartDac;
	long targetTime = _egTargetDac-_egStartDac;

	if ( targetTime == 0 ) return &_center;
	if ( time >= targetTime ) time = targetTime-1;

	if ( _state == ATTACK ){
		_fegCrntLevel = (targetTime-1-time)*_fegLevel/targetTime;
	}
	else if ( _state == RELEASE ){
		_fegCrntLevel = time*(_fegLevel-_fegStartLevel)/targetTime + _fegStartLevel;
	}
		
	if ( _fegCrntLevel > 0 ) return &_upper[_fegCrntLevel];
	else if ( _fegCrntLevel < 0 ) return &_lower[-_fegCrntLevel];
	return &_center;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Filter::checkEvent( void )
{
	switch (_state){
		case EG_NOT_YET:{
			if ( _parentNote->conditionKeyOn() == true ){
				//	Start key On
				toAttack();
			}
			break;
		}
		case ATTACK:
		case DECAY1:
		case DECAY2:
		case KEY_ON_STEADY:{
			if ( _parentNote->conditionKeyOn() == false ){
				//	Key Off
				toRelease();
			}
			break;
		}
		default: break;
	}
}
//---------------------------------------------------------
void Filter::checkSegmentEnd( void )
{
	if ( _dacCounter < _egTargetDac ) return;

	switch (_state){
		case ATTACK: toSteady(); break;
		default: break;
	}	
}
//---------------------------------------------------------
void Filter::process( TgAudioBuffer& buf )
{
	//	check Event
	checkEvent();

	//	Filter Calculate
	for ( int i=0; i<buf.bufferSize(); i++ ){
		Coef* crntCf = &_center;

		//	Check AEG Segment
		checkSegmentEnd();

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