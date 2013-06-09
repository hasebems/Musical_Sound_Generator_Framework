//
//  msgf_level_check.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/06/08.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_level_check__
#define __msgf_level_check__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
	//---------------------------------------------------------
	const int 	MEASUREMENT_TIME_MS = 100;	//	time(msec) updating level
	//---------------------------------------------------------
	class LevelCheck {
		//	This object stocks last MEASUREMENT_TIME_MS levels by one buffer.
		
	public:
		//---------------------------------------------------------
		LevelCheck( int bufSz )
		{
			int	dac = (SAMPLING_FREQUENCY*MEASUREMENT_TIME_MS)/1000;
			_maxSize = dac/bufSz;
			_writePtr = 0;
			_buf = new double[_maxSize];
		}
		//---------------------------------------------------------
		~LevelCheck( void )
		{
			delete[] _buf;
		}
		//---------------------------------------------------------
		void 	appendLevel( double lvl )
		{
			_buf[_writePtr++] = lvl;
			if ( _writePtr >= _maxSize ) _writePtr = 0;
		}
		//---------------------------------------------------------
		double	getMaxLevel( void )
		{
			double lvl = 0;
			for ( int i=0; i<_maxSize; i++ ){
				if ( lvl < _buf[i] ) lvl = _buf[i];
			}
			return lvl;
		}
		
	private:
		double*	_buf;
		int		_maxSize;
		int		_writePtr;
	};
}
#endif /* defined(__ToneGenerator__msgf_level_check__) */
