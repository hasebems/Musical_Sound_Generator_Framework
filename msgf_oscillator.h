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
typedef enum {
		
	SINE,
	TRIANGLE,
	SAW,
	SQUARE,
	PULSE,
	WAVEFORM_MAX

} WAVEFORM;
//---------------------------------------------------------
class TgAudioBuffer;
class Note;
//---------------------------------------------------------
class Oscillator {

public:
	Oscillator( Note* parent );
	~Oscillator( void ){}

	void	process( TgAudioBuffer& buf );

	//	Accessor
	void	setWaveform( int wvfm ){ _waveform = wvfm; }

private:
	double	calcPitch( const Uint8 note );
	void	generateSine( TgAudioBuffer& buf, double phase );
	void	generateTriangle( TgAudioBuffer& buf, double phase );
	void	generateSaw( TgAudioBuffer& buf, double phase );
	void	generateSquare( TgAudioBuffer& buf, double phase );
	void	generatePulse( TgAudioBuffer& buf, double phase );
	
	static const double tPitchOfA[11];
	
	Note*	_parentNote;
	OSCILLATOR_STATE	_state;
	long	_dacCounter;

	int		_waveform;
	double	_pitch;
	double	_crntPhase;
};
}
#endif /* defined(__msgf_oscillator__) */
