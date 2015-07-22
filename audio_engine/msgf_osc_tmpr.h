//
//  msgf_osc_tmpr.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2015/07/18.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_osc_tmpr__
#define __msgf_osc_tmpr__

#include <stdio.h>
#include <iostream>

#include "msgf_type.h"
#include "msgf_signal_process_core.h"
#include "msgf_voice_context.h"
#include "msgf_note.h"

namespace msgf {
	
	//	to be defined in every Audio Engine that demands an original parameter set
	const int VP_OSCILLATOR_TMPR_ID = 330;
	//---------------------------------------------------------
	typedef enum {
		
		VP_WAVEFORM,			//	0:sine, 1:tri, 2:saw, 3:square, 4:pulse

		VP_OSCILLATOR_MAX
		
	} VoiceParameter_Oscillator;
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
	class CallBackForTemperament {
		//	When use callback, inherit this class
	public:
		virtual double getCent( int doremi, int velVari ) = 0;
	};
	//---------------------------------------------------------
	class TgAudioBuffer;
	//---------------------------------------------------------
	class OscillatorTmpr : public SignalProcessCore {

	public:
		OscillatorTmpr( Note& parent );
		~OscillatorTmpr( void );
		
		void	init( void );
		void	release( void ){}
		void	addGetFuncObj( CallBackForTemperament* tmp ){
			_callBackFunc = tmp;
		}
		void	changeTmpr( void ){ _pitch = calcPitch( _parentNote.getNote() ); }
		
		//	process thread
		void	process( TgAudioBuffer& buf );
		
		//	Accessor
		void	setWaveform( int wvfm ){ _waveform = wvfm; }

	private:
		
		double	calcPitch( const Uint8 note );
		void	generateSine( TgAudioBuffer& buf );
		void	generateTriangle( TgAudioBuffer& buf );
		void	generateSaw( TgAudioBuffer& buf );
		void	generateSquare( TgAudioBuffer& buf );
		void	generatePulse( TgAudioBuffer& buf );
		
		int		getVoicePrm( int id ){ return _parentNote.getVoiceContext()->getParameter( VP_OSCILLATOR_TMPR_ID, id ); }
		
		static const int OCT_A_MAX = 11;
		static const double tPitchOfA[OCT_A_MAX];
		
		//	Basic Reference
		Note&	_parentNote;
		
		//	generate waveform
		int		_waveform;
		double	_pitch;
		double	_crntPhase;
		
		//	CallBack
		CallBackForTemperament* _callBackFunc;
	};
}

#endif /* defined(__msgf_osc_tmpr__) */
