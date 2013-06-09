//
//  msgf_audio_buffer.cpp
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/23.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#include "msgf_audio_buffer.h"
using namespace msgf;

//---------------------------------------------------------
//		Mix And Check
//---------------------------------------------------------
const double	DAMP_LIMIT_DEPTH = 0.0001;
//---------------------------------------------------------
bool TgAudioBuffer::mixAndCheckNoSound( TgAudioBuffer &srcBuf )
{
	int cnt = 0;
	for ( int i=0; i<_bufSize; i++ ){
		double val = srcBuf.getAudioBuffer(i);
		addAudioBuffer( i, val );
		if ( val < DAMP_LIMIT_DEPTH ){
			cnt++;
		}
	}

	if ( cnt >= _bufSize ){
		return true;
	}
	else {
		return false;
	}
}

//---------------------------------------------------------
//		Get Sound Level (MAX Level every buffer)
//---------------------------------------------------------
double TgAudioBuffer::getSoundLevel( void )
{
	double lvl = 0;
	for ( int i=0; i<_bufSize; i++ ){
		double newlvl = getAudioBuffer(i);
		if ( lvl < newlvl ){
			lvl = newlvl;
		}
	}
	return lvl;
}