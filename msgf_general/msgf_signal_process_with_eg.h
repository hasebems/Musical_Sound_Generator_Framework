//
//  msgf_signal_process_with_eg.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/13.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __ToneGenerator__msgf_signal_process_with_eg__
#define __ToneGenerator__msgf_signal_process_with_eg__

#include <iostream>
#include "msgf_type.h"
#include "msgf_audio_buffer.h"
#include "msgf_voice_context.h"
#include "msgf_signal_process_core.h"
#include "msgf_note.h"

namespace msgf {
	//---------------------------------------------------------
	typedef enum {
		
		EG_NOT_YET,
		ATTACK,
		DECAY1,
		DECAY2,
		KEY_ON_STEADY,
		RELEASE,
		KEY_OFF_STEADY,
		STATE_MAX
		
	} EG_STATE;
	
	//---------------------------------------------------------
	class SignalProcessWithEG : public SignalProcessCore {

	public:
		SignalProcessWithEG( Note* parent ):
		SignalProcessCore(),
		_parentNote(parent),
		_state(EG_NOT_YET) {}
		virtual ~SignalProcessWithEG( void ){}
		void	process( TgAudioBuffer& buf );

	protected:
		//	Move to next EG Segment
		virtual void	toAttack( void );			//	EG_NOT_YET -> ATTACK
		virtual void	toDecay1( void );			//	ATTACK -> DECAY1
		virtual void	toDecay2( void );			//	DECAY1 -> DECAY2
		virtual void	toKeyOnSteady( void );		//	keep DECAY2
		virtual void	toRelease( void );			//	any Segment -> RELEASE
		virtual void	toKeyOffSteady( void );		//	keep RELEASE

		//	Judge Segment change
		virtual void	checkEvent( void );
		virtual void	checkSegmentEnd( void );
		virtual void	checkSegmentEnd2seg( void );

		//	Get Segment Parameter
		virtual int		getAttackDacCount( void ){ return 0; }
		virtual int		getDecay1DacCount( void ){ return 0; }
		virtual int		getDecay2DacCount( void ){ return 0; }
		virtual int		getReleaseDacCount( void ){ return 0; }

		//	Basic Variables
		Note*		_parentNote;
		
		//	EG Manage
		long		_egStartDac;
		long		_egTargetDac;
		EG_STATE	_state;
	};
}
#endif /* defined(__ToneGenerator__msgf_signal_process_with_eg__) */
