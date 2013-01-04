//
//  msgf_lfo.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/12/09.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_lfo__
#define __msgf_lfo__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
	//---------------------------------------------------------
	typedef enum {
		BOTH,
		UPPER,
		LOWER,
		LFO_DIRECTION_MAX
	} LFO_DIRECTION;
	//---------------------------------------------------------
	typedef enum {
		LFO_TRI,
		LFO_SAW,
		LFO_SQU,
		LFO_SIN,
		LFO_WAVE_MAX
	} LFO_WAVE;
	//---------------------------------------------------------
	class Lfo {

	public:
		Lfo( void ):
			_depth(0),
			_wave(LFO_TRI),
			_direction(BOTH),
			_x1(0),_x2(0),_y(0),_z(0),
			_dacCounter(-1) {}
		void			start( void );
		void			reset( void );
		void			setCoef( void );
		void			process( int sampleNum, double* buf );
		
		void			setSamplingFrequency( int sFreq ){ _samplingFrequency = sFreq; }
		void			setFrequency( double freq ){ _frequency = freq; }
		void			setDelay( int cnt ){ _delayDacCount = cnt*_samplingFrequency/100; }
		void			setFadeIn( int cnt ){ _fadeInDacCount = cnt*_samplingFrequency/100; }

		void			setDepth( double dpt ){ _depth = dpt; }
		void			setWave( LFO_WAVE wv ){ _wave = wv; }
		void			setDerection( LFO_DIRECTION dir ){ _direction = dir; }
		
	private:
		//	Common Variables
		static int		_samplingFrequency;
		static double	_frequency;			//	LFO Freq.
		static int		_delayDacCount;		//	LFO Delay Dac Count
		static int		_fadeInDacCount;	//	LFO Fade In Dac Count

		//	Each Variables
		double			_depth;				//	LFO Depth
		LFO_WAVE		_wave;				//	LFO Wave
		LFO_DIRECTION	_direction;
		
		double			_deltaPhase;			//	LFO Phase
		double			_phase;
		long			_dacCounter;

		//	LFO Coef
		double			_x1;
		double			_x2;
		double			_y;
		double			_z;
	};
}
#endif /* defined(__msgf_lfo__) */
