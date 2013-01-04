//
//  msgf_lfo.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/12/09.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include <math.h>
#include "msgf_lfo.h"

using namespace msgf;

//---------------------------------------------------------
//		Variables
//---------------------------------------------------------
int		Lfo::_samplingFrequency = 44100;
double	Lfo::_frequency = 0;
int		Lfo::_delayDacCount = 0;		//	LFO Delay Dac Count
int		Lfo::_fadeInDacCount = 0;		//	LFO Fade In Dac Count

//---------------------------------------------------------
//		Constructor
//---------------------------------------------------------



//---------------------------------------------------------
//		LFO Start
//---------------------------------------------------------
void Lfo::start( void )
{	
	_dacCounter = 0;
	_phase = 0;
	_deltaPhase = _frequency/_samplingFrequency;
}

//---------------------------------------------------------
//		Set Coef
//---------------------------------------------------------
void Lfo::setCoef( void )
{
	switch ( _wave ){
		default:
		case LFO_SAW: _x1=0; _x2=2; _y=2; _z=0; break;
		case LFO_TRI: _x1=0.5; _x2=1.5; _y=4; _z=0; break;
		case LFO_SQU: _x1=0.5; _x2=1.5; _y=100000; _z=0; break;
		case LFO_SIN: _x1=0.5; _x2=1.5; _y=2*M_PI; _z=1/6.78; break;
	}
}

//---------------------------------------------------------
//		Process
//---------------------------------------------------------
void Lfo::process( int sampleNum, double* buf )
{
	if ( _dacCounter >= 0 ){
		for ( int i=0; i<sampleNum; i++ ){
			double value = ( fmin( fmax( _phase, _x1-_phase ), _x2-_phase) - 0.5)*_y;
			value = value - value*value*value*_z;
			
			_phase += _deltaPhase;
			if ( _phase >= 1.0 ) _phase -= 1.0;

			if ( value > 1.0 )
				value = 1.0;
			if ( value < -1.0 )
				value = -1.0;

			//	Fadein, Delay
			if ( _dacCounter < _fadeInDacCount ) value = 0;
			else if ( _dacCounter < _fadeInDacCount+_delayDacCount ){
				value = value*(_dacCounter-_fadeInDacCount)/_delayDacCount;
			}
			
			buf[i] = value;
			_dacCounter++;
		}
	}
}
