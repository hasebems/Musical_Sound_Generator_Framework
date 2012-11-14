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

namespace msgf {
//---------------------------------------------------------
typedef enum {
	
	AMPLITUDE_NOT_YET,
	ATTACK,
	DECAY1,
	DECAY2,
	DECAY2_STEADY,
	RELEASE,
	DAMP,
	AMPLITUDE_STATE_MAX
	
} AMPLITUDE_STATE;
//---------------------------------------------------------
class TgAudioBuffer;
class Note;
//---------------------------------------------------------
class Amplitude {
	
public:
	Amplitude( Note* parent ):
		_parentNote( parent ),
		_state(AMPLITUDE_NOT_YET),
		_dacCounter(-1) {}
	~Amplitude( void ){}
	
	void	process( TgAudioBuffer& buf );
	
private:
	void	toAttack( void );
	void	toDecay1( void );
	void	toDecay2( void );
	void	toDecay2Steady( void );
	void	toRelease( void );
	
	int		getTotalDacCount( int time );
	double	getAegLevel( long crntDac, long targetDac, int startLvl, int targetLvl );
	double	calcVolume( double amp );
	
	Note*	_parentNote;
	AMPLITUDE_STATE	_state;
	long	_dacCounter;

	long	_startDac;
	long	_targetDac;
	int		_startLvl;
	int		_targetLvl;

};
}
#endif /* defined(__msgf_amplitude__) */
