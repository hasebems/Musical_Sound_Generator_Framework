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
	
	//	to be defined in every Audio Engine that demands an original parameter set
	const int VP_OSC_PIPE_ID = 320;
	//---------------------------------------------------------
	typedef enum {
		
		//	Overtone Type
		VP_OVERTONE_TYPE,		//	0:Sine, 1:LikeRecorder, 2:LikeOboe
		//	Pitch
		VP_TUNING,				//	-100 - 100[cent]
		VP_TRANSPOSE,			//	-24 - 24(seminote)
		//	Portamento
		VP_PORTAMENTO_MODE,		//	0:rate constant, 1:time constant
		VP_PORTAMENTO_CURVE,	//	0:cent linear, 1:freqency linear
		//	LFO
		VP_LFO_FREQUENCY,		//	0 - ??? (*0.1 =[Hz])
		VP_LFO_DELAY_TIME,		//	0 - ??? (*10 =[ms])
		VP_LFO_FADEIN_TIME,		//	0 - ??? (*10 =[ms])
		//	Note Change EG
		VP_WAITING_DCNT,		//	0 - inf (*22.67 =[us])
		VP_FAST_MOVE_DCNT,		//	0 - inf	(*22.67 =[us])
		VP_PORTAMENTO_DIFF,		//	1 - 24(semitone)

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
		void	changeNote( bool chgNote );
		void	release( void ){}
		
		//	process thread
		void	process( TgAudioBuffer& buf );
		
		static const int PEG_MAX = 60;
		static const int PEG_DEPTH_MAX = 2; // /2 Octave
		static const int MAX_OVERTONE = 8;
		
	private:
		
		double	calcPitch( const Uint8 note );
		double	getPegPitch( int depth );
		void	reflectMidiController( void );
		double	calcDeltaLFO( double lfoDpt );
		void	stateOfWaitingPortamento( void );
		void	stateOfFastMove( void );
		void	stateOfSlowMove( void );
		void	managePortamentoState( void );
		void	setPortamentoCounter( double centDiff );
		double	generateWave( double phase );
		
		int		getVoicePrm( int id ){ return _parentNote.getVoiceContext()->getParameter( VP_OSC_PIPE_ID, id ); }
		
		static const double tPitchOfA[11];
		static const int MAX_OVERTONE_TYPE = 3;
		static const int tOverToneRatio[MAX_OVERTONE_TYPE][MAX_OVERTONE];

		//	Basic Reference
		Note&		_parentNote;
		
		//	generate waveform
		Uint8		_note;
		double		_pitch;
		double		_pitchAdj;
		double		_crntPhase;
		double		_cent2pitch;
		double		_chromaticRatio;

		//	portamento
		int			_portamentoCounter;
		int			_prtmDiff;				//	copy from VP_PORTAMENTO_DIFF
		int			_maxPortamentoCounter;	//	prohibit 0
		double		_sourcePitch;
		double		_targetPitch;
		double		_targetCent;
		PRTM_STATE	_prtmState;
		
		//	LFO
		Lfo*	_pm;
	};
}
#endif /* defined(__ToneGenerator__msgf_osc_pipe__) */
