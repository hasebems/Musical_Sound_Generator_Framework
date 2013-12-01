//
//  msgf_osc_pipe.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/12/01.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __ToneGenerator__msgf_osc_pipe__
#define __ToneGenerator__msgf_osc_pipe__

#include <iostream>
#include <math.h>
#include "msgf_type.h"
#include "msgf_signal_process_core.h"
#include "msgf_voice_context.h"
#include "msgf_note.h"
#include "msgf_eg2seg.h"
#include "msgf_lfo.h"

namespace msgf {
	
	//	to be defined every Audio Engine that wants an original parameter set
	const int VP_OSC_PIPE_ID = 320;
	//---------------------------------------------------------
	typedef enum {
		
		VP_TUNING,				//	-100 - 100(cent)
		VP_TRANSPOSE,			//	-24 - 24(seminote)
		VP_WAVEFORM,			//	0:sine, 1:tri
		
		VP_PORTAMENTO_MODE,		//	0:rate constant, 1:time constant
		VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
		VP_PORTAMENTO,			//	0 - ??? (rate: *10ms/100cent, time: *10ms)

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
	typedef enum {
		
		NO_MOVE,
		WAITING_PRTM,	//	Time of from event to begin to move
		FAST_MOVE,		//	fast move time
		SLOW_MOVE,		//	slow move time
		PRTM_STATE_MAX
		
	} PRTM_STATE;
	
	//---------------------------------------------------------
	class TgAudioBuffer;
	class PegCallBack;
	//---------------------------------------------------------
	class OscPipe : public SignalProcessCore {
		
		friend class PegCallBack;
		
	public:
		OscPipe( Note& parent );
		~OscPipe( void );
		
		void	init( void ){ init(true);}
		void	init( bool phaseReset );
		void	changeNote( void );
		void	release( void ){}
		
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
		void	managePortamentoState( void );
		void	setPortamentoCounter( void );
		double	generateWave( double phase );
		
		int		getVoicePrm( int id ){ return _parentNote.getVoiceContext()->getParameter( VP_OSC_PIPE_ID, id ); }
		
		static const double tPitchOfA[11];

		const int PRTM_SLOW_DIFF = 2;
		const int PRTM_WAITING_TIME = 2000;		//	*dac count
		const int PRTM_FAST_MOVE_TIME = 500;	//	*dac count

		//	Basic Reference
		Note&	_parentNote;
		
		//	generate waveform
		int		_waveform;
		Uint8	_note;
		double	_pitch;
		double	_crntPhase;
		
		//	portamento
		int		_portamentoCounter;
		int		_maxPortamentoCounter;
		double	_pitchOrg;
		double	_pitchTarget;
		double	_targetCent;
		Uint8	_srcNote;
		Uint8	_tgtNote;
		PRTM_STATE	_prtmState;
		
		//	LFO
		Lfo*	_pm;
		double	_pmd;
	};
}
#endif /* defined(__ToneGenerator__msgf_osc_pipe__) */
