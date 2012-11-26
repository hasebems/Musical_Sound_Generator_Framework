//
//  msgf_amplitude.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/31.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_amplitude__
#define __msgf_amplitude__

#include <iostream>
#include "msgf_type.h"
#include "msgf_signal_process_core.h"

namespace msgf {
//---------------------------------------------------------
class TgAudioBuffer;
class Note;
//---------------------------------------------------------
class Amplitude : public SignalProcessCore {
	
public:
	Amplitude( Note* parent ):
		SignalProcessCore(parent) {}
	~Amplitude( void ){}
	
	void	init( void ){}
	void	process( TgAudioBuffer& buf );
	
private:
	void	toAttack( void );
	void	toDecay1( void );
	void	toDecay2( void );
	void	toDecay2Steady( void );
	void	toRelease( void );
	
//	int		getTotalDacCount( int time );
	double	getAegLevel( long crntDac, long targetDac, int startLvl, int targetLvl );
	double	calcVolume( double amp );
	
//	Note*	_parentNote;
//	EG_STATE	_state;
//	long	_dacCounter;

//	long	_startDac;
//	long	_targetDac;
	int		_startLvl;
	int		_targetLvl;

};
}
#endif /* defined(__msgf_amplitude__) */
