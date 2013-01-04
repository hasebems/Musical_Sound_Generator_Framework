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
#include "msgf_voice_context.h"
#include "msgf_note.h"
#include "msgf_lfo.h"

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
//---------------------------------------------------------
class Oscillator : public SignalProcessCore {

public:
	Oscillator( Note* parent ):
	SignalProcessCore(parent) {}
	~Oscillator( void );

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

	int		getVoicePrm( VoiceParameterId id ){ return _parentNote->getVoiceContext()->getParameter( id ); }
	double	calcPitch( const Uint8 note );
	void	calcPegPitch( double pch );
	double	getPegCurrentPitch( void );

	double	calcDeltaLFO( double lfoDpt, double diff );
	void	generateSine( TgAudioBuffer& buf, double* lfobuf, double diff );
	void	generateTriangle( TgAudioBuffer& buf, double* lfobuf, double diff );
	void	generateSaw( TgAudioBuffer& buf, double* lfobuf, double diff );
	void	generateSquare( TgAudioBuffer& buf, double* lfobuf, double diff );
	void	generatePulse( TgAudioBuffer& buf, double* lfobuf, double diff );
	
	static const double tPitchOfA[11];

	//	PEG
	double	_upper[PEG_MAX];
	double	_lower[PEG_MAX];
	int		_pegStartLevel;
	int		_pegCrntLevel;
	int		_pegLevel;

	//	generate waveform
	int		_waveform;
	double	_pitch;
	double	_crntPhase;

	//	LFO
	Lfo*	_pm;
	double	_pmd;
};
}
#endif /* defined(__msgf_oscillator__) */
