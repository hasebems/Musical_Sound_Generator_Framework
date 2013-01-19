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
SignalProcessWithEG(parent),
_fegStartLevel(0),
_fegCrntLevel(0),
_fegTargetLevel(0),
_x_m2(0),
_x_m1(0),
_y_m2(0),
_y_m1(0)
{}
//---------------------------------------------------------
void Filter::init( void )
{
	_baseFc = getVoicePrm( VP_FILTER_CUTOFF );
	_baseQ = getVoicePrm( VP_FILTER_RESO );

	double ratio = log(FEG_DEPTH_MAX)/FEG_MAX;
	_frqRatio = exp(ratio);

	setOneCoef( (double)_baseFc, ((double)_baseQ)/10, _center );
}
//---------------------------------------------------------
Filter::~Filter( void )
{
}

//---------------------------------------------------------
//		Calculate Coef
//---------------------------------------------------------
double Filter::calcFreq( double fc, int prm )
{
	return fc*pow( _frqRatio, prm );
}
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
	//	time
	SignalProcessWithEG::toAttack();

	//	level
	_fegStartLevel = getVoicePrm(VP_FEG_ATTACK_LEVEL);
}
//---------------------------------------------------------
void Filter::toKeyOnSteady( void )
{
	//	time
	SignalProcessWithEG::toKeyOnSteady();

	//	level
	_fegTargetLevel = _fegCrntLevel = 0;
}
//---------------------------------------------------------
void Filter::toRelease( void )
{
	//	time
	SignalProcessWithEG::toRelease();

	//	level
	_fegTargetLevel = getVoicePrm(VP_FEG_RELEASE_LEVEL);
	_fegStartLevel = _fegCrntLevel;
}

//---------------------------------------------------------
//		Get FEG Coef
//---------------------------------------------------------
void Filter::getFegCoef( Coef& cf )
{
	long time = _dacCounter-_egStartDac;
	long targetTime = _egTargetDac-_egStartDac;

//	if ( targetTime == 0 ){
//		cf = _center;
//		return;
//	}
	
	if ( time >= targetTime ) time = targetTime-1;

	if ( _state == ATTACK ){
		_fegCrntLevel = (targetTime-1-time)*_fegStartLevel/targetTime;
	}
	else if ( _state == RELEASE ){
		_fegCrntLevel = time*(_fegTargetLevel-_fegStartLevel)/targetTime + _fegStartLevel;
	}

	double fc = calcFreq( _baseFc, _fegCrntLevel );
	setOneCoef( fc, _baseQ, cf );
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Filter::process( TgAudioBuffer& buf )
{
	//	check Event
	checkEvent();

	//	Filter Calculate
	for ( int i=0; i<buf.bufferSize(); i++ ){
		Coef crntCf;

		//	Check AEG Segment
		checkSegmentEnd2seg();

		getFegCoef(crntCf);

		//	Calculate Filter
		double input = buf.getAudioBuffer(i);
		double output = crntCf._b0*input + crntCf._b1*_x_m1 + crntCf._b2*_x_m2;
		output += -crntCf._a1*_y_m1 - crntCf._a2*_y_m2;
	
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