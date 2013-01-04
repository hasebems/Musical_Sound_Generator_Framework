//
//  msgf_signal_process_core.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/25.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_signal_process_core__
#define __msgf_signal_process_core__

#include <iostream>
#include "msgf_type.h"
#include "msgf_audio_buffer.h"

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
	class Note;
	//---------------------------------------------------------
	class SignalProcessCore {
		
	public:
		SignalProcessCore( Note* parent ):
			_parentNote(parent),
			_dacCounter(-1),
			_egStartDac(0),
			_egTargetDac(0),
			_state(EG_NOT_YET) {}
		virtual ~SignalProcessCore( void ){}

		virtual void	init( void ) = 0;
		virtual void	process( TgAudioBuffer& buf ) = 0;
		
	protected:
		//	Move to next EG Segment
		virtual void	toAttack( void ){}			//	EG_NOT_YET -> ATTACK
		virtual void	toDecay1( void ){}			//	ATTACK -> DECAY1
		virtual void	toDecay2( void ){}			//	DECAY1 -> DECAY2
		virtual void	toKeyOnSteady( void ){}		//	keep DECAY2
		virtual void	toRelease( void ){}			//	any Segment -> RELEASE
		virtual void	toKeyOffSteady( void ){}	//	keep RELEASE

		//	Judge Segment change
		virtual	void	checkEvent( void ){}
		virtual void	checkSegmentEnd( void ){}
		
		int		getTotalDacCount( const int time ){return time*(SMPL_FREQUENCY/100);}
		
		//	Basic Variables
		Note*	_parentNote;
		long	_dacCounter;		//	should be incremented by buffer size in process

		//	EG Manage
		long	_egStartDac;
		long	_egTargetDac;
		EG_STATE	_state;
	};
}
#endif
