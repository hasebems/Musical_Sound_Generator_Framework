//
//  msgf_audio_buffer.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/23.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_audio_buffer__
#define __msgf_audio_buffer__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
	//---------------------------------------------------------
//	#define		SMPL_FREQUENCY		44100
	//---------------------------------------------------------
	class TgAudioBuffer {

	public:
		//	process thread(PT)
		void	obtainAudioBuffer( int bsz )
		{
			_bufSize = bsz;
			_abuf = new double[bsz];
			for ( int i=0; i<bsz; i++ ){ *(_abuf + i ) = 0; }
		}
		void	releaseAudioBuffer( void ){ delete[] _abuf; }	//	PT

		void	setAudioBuffer( int index, double value ){ _abuf[index] = value; }	//	PT
		void	addAudioBuffer( int index, double value ){ _abuf[index] += value; }	//	PT
		void	mulAudioBuffer( int index, double value ){ _abuf[index] *= value; }	//	PT
		double	getAudioBuffer( int index ) const { return _abuf[index]; }

		int		bufferSize( void ){ return _bufSize; }			//	PT
		bool	mixAndCheckNoSound( TgAudioBuffer &srcBuf );	//	PT
		double	getSoundLevel( void );	//	PT
	
	private:
		int			_bufSize;
		double*		_abuf;		//	-1 -- 0 -- 1
	};
}
#endif /* defined(__msgf_audio_buffer__) */
