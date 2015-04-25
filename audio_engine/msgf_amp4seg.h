//
//  msgf_amp4seg.h
//
//	Musical Sound Generator Framework
//
//  Created by 長谷部 雅彦 on 2012/10/31.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_amp4seg__
#define __msgf_amp4seg__

#include <iostream>
#include "msgf_type.h"
#include "msgf_signal_process_core.h"
#include "msgf_note.h"
#include "msgf_lfo.h"
#include "msgf_voice_context.h"
#include "msgf_eg4seg.h"

namespace msgf {

	//	to be defined in every Audio Engine that demands an original parameter set
	const int VP_AMPLITUDE_ID = 200;
	//---------------------------------------------------------
	typedef enum {
		
		VP_VOLUME,				//	0 - 100
		
		VP_AEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
		VP_AEG_DECAY1_LEVEL,	//	0 - 100
		VP_AEG_DECAY1_TIME,		//	0 - 1000 (*10ms)
		VP_AEG_DECAY2_LEVEL,	//	0 - 100
		VP_AEG_DECAY2_TIME,		//	0 - 1000 (*10ms)
		VP_AEG_RELEASE_TIME,	//	0 - 1000 (*10ms)

		VP_LFO_AMD,				//	0 - 100 %
		
		VP_AMPLITUDE_MAX
		
	} VoiceParameter_Amplitude;
	//---------------------------------------------------------
	class TgAudioBuffer;
	class AegCallBack;
	//---------------------------------------------------------
	class Amplitude : public SignalProcessCore {

		friend class AegCallBack;

	public:
		Amplitude( Note& parent );
		~Amplitude( void );
		
		void	init( void );
		void	release( void ){ _eg->moveToRelease(); }

		//	process thread
		void	process( TgAudioBuffer& buf );

		static const int AMP_PRM_MAX = 100;
		
	private:		
		//	original
		double	calcMidiVolume( double amp );
		
		//	Get Segment Parameter
		int		getVoicePrm( int id ){ return _parentNote.getVoiceContext()->getParameter( VP_AMPLITUDE_ID, id ); }
		int		getAttackDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_ATTACK_TIME)); }
		int		getDecay1DacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_DECAY1_TIME)); }
		int		getDecay2DacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_DECAY2_TIME)); }
		int		getReleaseDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_RELEASE_TIME)); }

		//	Basic Reference
		Note&	_parentNote;

		//	EG Level
		AegCallBack*	_cbInst;
		Eg4segment*		_eg;

		//	LFO
		Lfo*	_am;
		double	_amd;
	};
	//---------------------------------------------------------
	class AegCallBack : public CallBack {
		
	public:
		AegCallBack( Amplitude* amp ):
		_amp(amp){}
		virtual double	getEgLvl( EG_STATE prm )
		{
			switch (prm){
				case EG_NOT_YET: return 0;
				case EG_ATTACK:	 return 1;
				case EG_DECAY1:	 return static_cast<double>(_amp->getVoicePrm( VP_AEG_DECAY1_LEVEL ))/100;
				case EG_DECAY2:	 return static_cast<double>(_amp->getVoicePrm( VP_AEG_DECAY2_LEVEL ))/100;
				case EG_RELEASE: return 0;
				default: return 0;
			}
		}
		virtual int		getEgTime( EG_STATE prm )
		{
			switch (prm){
				case EG_ATTACK:	 return _amp->getAttackDacCount();
				case EG_DECAY1:	 return _amp->getDecay1DacCount();
				case EG_DECAY2:	 return _amp->getDecay2DacCount();
				case EG_RELEASE: return _amp->getReleaseDacCount();
				default: return 0;
			}
		}
	private:
		Amplitude*	_amp;
	};
}
#endif /* defined(__msgf_amp4seg__) */
