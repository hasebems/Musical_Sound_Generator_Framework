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
#include "msgf_signal_process_core.h"

namespace msgf {
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
class Oscillator : public SignalProcessCore {

public:
	Oscillator( Note* parent ):
	SignalProcessCore(parent) {}

	~Oscillator( void ){}

	void	init( void );
	void	checkEvent( void );
	void	checkSegmentEnd( void );
	void	process( TgAudioBuffer& buf );

	//	Accessor
	void	setWaveform( int wvfm ){ _waveform = wvfm; }

	static const int PEG_MAX = 60;
	static const int PEG_DEPTH_MAX = 2; // /2 Octave

private:
	//	override
	void	toAttack( void );
	void	toSteady( void );
	void	toRelease( void );

	double	calcPitch( const Uint8 note );
	void	calcPegPitch( double pch );
	double	getFegCurrentPitch( void );

	void	generateSine( TgAudioBuffer& buf, double diff );
	void	generateTriangle( TgAudioBuffer& buf, double diff );
	void	generateSaw( TgAudioBuffer& buf, double diff );
	void	generateSquare( TgAudioBuffer& buf, double diff );
	void	generatePulse( TgAudioBuffer& buf, double diff );
	
	static const double tPitchOfA[11];

	//	PEG
	double	_upper[PEG_MAX];
	double	_lower[PEG_MAX];
	int		_pegStartLevel;
	int		_pegCrntLevel;
	int		_pegLevel;

	int		_waveform;
	double	_pitch;
	double	_crntPhase;
};
}
#endif /* defined(__msgf_oscillator__) */
