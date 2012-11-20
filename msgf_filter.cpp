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
_x_m2(0),
_x_m1(0),
_y_m2(0),
_y_m1(0),
_a1(0),
_a2(0),
_b0(0),
_b1(0),
_b2(0)
{
	int fc = _parentNote->getVoiceContext()->getParameter( VP_FILTER_CUTOFF );
	int reso = _parentNote->getVoiceContext()->getParameter( VP_FILTER_RESO );
	setCoef( fc, ((double)reso)/10 );
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Filter::setCoef( double fc, double qValue )
{
	double freq = tan(M_PI*fc/SMPL_FREQUENCY)/(2.0*M_PI);
	double fc2 = freq*freq;
	double tmp = 1.0 + (2.0*M_PI*freq)/qValue + (4.0*M_PI*M_PI*fc2);

	_b0 = (4.0*M_PI*M_PI*fc2)/tmp;
	_b1 = (8.0*M_PI*M_PI*fc2)/tmp;
	_b2 = _b0;
	_a1 = (8.0*M_PI*M_PI*fc2 - 2.0)/tmp;
	_a2 = (1.0 - 2.0*M_PI*freq/qValue + 4.0*M_PI*M_PI*fc2)/tmp;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void Filter::process( TgAudioBuffer& buf )
{
	for ( int i=0; i<buf.bufferSize(); i++ ){
		//	Calculate Filter
		double input = buf.getAudioBuffer(i);
		double output = _b0*input + _b1*_x_m1 + _b2*_x_m2;
		output += -_a1*_y_m1 - _a2*_y_m2;
	
		//	Limmiter
		if ( output > 1 ) output = 1;
		else if ( output < -1 ) output = -1;
	
		_y_m2 = _y_m1;
		_y_m1 = output;
		_x_m2 = _x_m1;
		_x_m1 = input;
		buf.setAudioBuffer( i, output );
	}
}