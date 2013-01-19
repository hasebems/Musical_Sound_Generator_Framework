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
#include "msgf_signal_process_with_eg.h"
#include "msgf_note.h"
#include "msgf_lfo.h"
#include "msgf_voice_context.h"

namespace msgf {

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
	//---------------------------------------------------------
	class Amplitude : public SignalProcessWithEG {
		
	public:
		Amplitude( Note* parent );
		~Amplitude( void );
		
		void	init( void );
		void	process( TgAudioBuffer& buf );
		
	private:
		//	override
		void	toAttack( void );
		void	toDecay1( void );
		void	toDecay2( void );
		void	toKeyOnSteady( void );
		void	toRelease( void );
		
		//	original
		double	getAegLevel( long crntDac, long targetDac, int startLvl, int targetLvl );
		double	calcVolume( double amp );
		
		//	Get Segment Parameter
		int		getVoicePrm( int id ){ return _parentNote->getVoiceContext()->getParameter( VP_AMPLITUDE_ID, id ); }
		int		getAttackDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_ATTACK_TIME)); }
		int		getDecay1DacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_DECAY1_TIME)); }
		int		getDecay2DacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_DECAY2_TIME)); }
		int		getReleaseDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_AEG_RELEASE_TIME)); }

		//	EG Level
		int		_aegStartLevel;
		int		_aegTargetLevel;
		double	_crntAeg;

		//	LFO
		Lfo*	_am;
		double	_amd;
	};
}
#endif /* defined(__msgf_amplitude__) */
