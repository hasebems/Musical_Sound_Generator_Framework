//
//  msgf_iirfilter.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include <math.h>
#include "msgf_iirfilter.h"
#include "msgf_note.h"
#include "msgf_audio_buffer.h"
#include "msgf_voice_context.h"

using namespace msgf;

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------
IirFilter::IirFilter( Note& parent ):
_parentNote(parent),
_x_m2(0),
_x_m1(0),
_y_m2(0),
_y_m1(0)
{
	_cbInst = new FegCallBack( this );
	_eg = new Eg2segment( *_cbInst, parent );
}
//---------------------------------------------------------
IirFilter::~IirFilter( void )
{
	delete _cbInst;
	delete _eg;
}
//---------------------------------------------------------
void IirFilter::init( void )
{
	_baseFc = getVoicePrm( VP_FILTER_CUTOFF );
	_baseQ = getVoicePrm( VP_FILTER_RESO );
	
	double ratio = log(FEG_DEPTH_MAX)/FEG_MAX;
	_frqRatio = exp(ratio);
}

//---------------------------------------------------------
//		Calculate Coef
//---------------------------------------------------------
double IirFilter::calcFreq( double fc, int prm )
{
	return fc*pow( _frqRatio, prm );
}
//---------------------------------------------------------
void IirFilter::setOneCoef( double fc, double qValue, Coef& cf )
{
	double freq = tan(M_PI*fc/SAMPLING_FREQUENCY)/(2.0*M_PI);
	double fc2 = freq*freq;
	double tmp = 1.0 + (2.0*M_PI*freq)/qValue + (4.0*M_PI*M_PI*fc2);
	
	cf._b0 = (4.0*M_PI*M_PI*fc2)/tmp;
	cf._b1 = (8.0*M_PI*M_PI*fc2)/tmp;
	cf._b2 = cf._b0;
	cf._a1 = (8.0*M_PI*M_PI*fc2 - 2.0)/tmp;
	cf._a2 = (1.0 - 2.0*M_PI*freq/qValue + 4.0*M_PI*M_PI*fc2)/tmp;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void IirFilter::process( TgAudioBuffer& buf )
{
	//	check Event
	_eg->periodicOnceEveryProcess();
	
	//	Filter Calculate
	for ( int i=0; i<buf.bufferSize(); i++ ){
		Coef crntCf;
		
		//	Check EG Segment
		_eg->periodicOnceEveryDac( _dacCounter );
		
		//	Calculate Coef
		int egLvl = static_cast<int>(_eg->calcEgLevel() * FEG_MAX);
		double fc = calcFreq( _baseFc, egLvl );
		setOneCoef( fc, _baseQ, crntCf );

		//	Calculate Audio
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
