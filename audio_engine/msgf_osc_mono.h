//
//  msgf_osc_mono.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/07/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_osc_mono__
#define __msgf_osc_mono__

#include <iostream>
#include <math.h>
#include "msgf_type.h"
#include "msgf_signal_process_core.h"
#include "msgf_voice_context.h"
#include "msgf_note.h"
#include "msgf_eg2seg.h"
#include "msgf_lfo.h"

namespace msgf {
	
	//	to be defined in every Audio Engine that demands an original parameter set
	const int VP_OSC_MONO_ID = 310;
	//---------------------------------------------------------
	typedef enum {
		
		VP_TUNING,				//	-100 - 100(cent)
		VP_TRANSPOSE,			//	-24 - 24(seminote)
		VP_WAVEFORM,			//	0:sine, 1:tri

		VP_PORTAMENTO_MODE,		//	0:rate constant, 1:time constant
		VP_PORTAMENTO_DIFF,		//	0:full, 1: Max100cent, 2: Max200cent ...
		VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
		VP_PORTAMENTO,			//	0 - ??? (rate: *10ms/100cent, time: *10ms)
		
		VP_PEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
		VP_PEG_ATTACK_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
		VP_PEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
		VP_PEG_RELEASE_LEVEL,	//	-60 - 0 - +60 (1/2 - 2[times*Hz])
		
		VP_LFO_FREQUENCY,		//	0 - ??? (*0.1[Hz])
		VP_LFO_DELAY_TIME,		//	0 - ??? (*10ms)
		VP_LFO_FADEIN_TIME,		//	0 - ??? (*10mc)
		VP_LFO_PMD,				//	0 - 100 %
		
		VP_OSCILLATOR_MAX
		
	} VoiceParameter_OscMono;
	//---------------------------------------------------------
	typedef enum {
		
		SINE,
		TRIANGLE,
		WAVEFORM_MAX
		
	} WAVEFORM;
	
	//---------------------------------------------------------
	class TgAudioBuffer;
	class PegCallBack;
	//---------------------------------------------------------
	class OscMono : public SignalProcessCore {
		
		friend class PegCallBack;
		
	public:
		OscMono( Note& parent );
		~OscMono( void );
		
		void	init( void ){ init(true);}
		void	init( bool phaseReset );
		void	changeNote( void );
		void	release( void ){ _eg->moveToRelease(); }
		
		//	process thread
		void	process( TgAudioBuffer& buf );
		
		//	Accessor
		void	setWaveform( int wvfm ){ _waveform = wvfm; }
		
		static const int PEG_MAX = 60;
		static const int PEG_DEPTH_MAX = 2; // /2 Octave
		
	private:
		
		double	calcPitch( const Uint8 note );
		double	getPegPitch( int depth );
		
		double	calcDeltaLFO( double lfoDpt, double diff );
		void	calcPortamento( void );
		double	generateSine( double phase ){ return sin(phase);}
		double	generateTriangle( double phase );
		
		int		getVoicePrm( int id ){ return _parentNote.getVoiceContext()->getParameter( VP_OSC_MONO_ID, id ); }
		int		getAttackDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_PEG_ATTACK_TIME)); }
		int		getReleaseDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_PEG_RELEASE_TIME)); }
		
		static const double tPitchOfA[11];
		
		//	Basic Reference
		Note&	_parentNote;
		
		//	PEG
		PegCallBack*	_cbInst;
		Eg2segment*		_eg;
		
		//	generate waveform
		int		_waveform;
		Uint8	_note;
		double	_pitch;
		double	_crntPhase;

		//	portamento
		bool	_cndDuringPortamento;
		int		_portamentoCounter;
		int		_maxPortamentoCounter;
		double	_pitchOrg;
		double	_pitchTarget;
		double	_targetCent;
		
		//	LFO
		Lfo*	_pm;
		double	_pmd;
	};
	//---------------------------------------------------------
	class PegCallBack : public CallBack {
		
	public:
		PegCallBack( OscMono* osc ):
		_osc(osc){}
		virtual double	getEgLvl( EG_STATE prm )
		{
			switch (prm){
				case EG_ATTACK:	 return static_cast<double>(_osc->getVoicePrm( VP_PEG_ATTACK_LEVEL ))/100;
				case EG_RELEASE: return static_cast<double>(_osc->getVoicePrm( VP_PEG_RELEASE_LEVEL ))/100;
				default: return 0;
			}
		}
		virtual int		getEgTime( EG_STATE prm )
		{
			switch (prm){
				case EG_ATTACK:	 return _osc->getAttackDacCount();
				case EG_RELEASE: return _osc->getReleaseDacCount();
				default: return 0;
			}
		}
	private:
		OscMono*	_osc;
	};
}
#endif /* defined(__msgf_osc_mono__) */
