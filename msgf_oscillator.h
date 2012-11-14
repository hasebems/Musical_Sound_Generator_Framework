//
//  msgf_oscillator.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/23.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_oscillator__
#define __msgf_oscillator__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
//---------------------------------------------------------
typedef enum {

	NOT_YET,
	STEADY_STATE,
	STOP_STATE,
	OSCILLATOR_STATE_MAX
	
} OSCILLATOR_STATE;
//---------------------------------------------------------
class TgAudioBuffer;
class Note;
//---------------------------------------------------------
class Oscillator {

public:
	Oscillator( Note* parent ):
		_parentNote( parent ),
		_state(NOT_YET),
		_dacCounter(-1) {}
	~Oscillator( void ){}

	void	process( TgAudioBuffer& buf );
	
private:
	double	calcPitch( const Uint8 note );
	double	generateWave( double phase );

	static const double tPitchOfA[10];
	
	Note*	_parentNote;
	OSCILLATOR_STATE	_state;
	long	_dacCounter;

	double	_pitch;
	double	_crntPhase;
};
}
#endif /* defined(__msgf_oscillator__) */
