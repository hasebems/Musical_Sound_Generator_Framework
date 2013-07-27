//
//  msgf_eg4seg.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/02/02.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_eg4seg__
#define __msgf_eg4seg__

#include <iostream>
#include "msgf_note.h"
#include "msgf_eg.h"

namespace msgf {
	//---------------------------------------------------------
	class Eg4segment : public Eg {
		
		//	4segments mean :
		//		Attack	:	level: EG_NOT_YET	->	EG_ATTACK
		//		Decay1	:	level: EG_ATTACK	->	EG_DECAY1
		//		Decay2	:	level: EG_DECAY1	->	EG_DECAY2
		//		Release	:	level: EG_DECAY2	->	EG_RELEASE

	public:
		Eg4segment( CallBack& cbObj, Note& parent ):
		_cbFunc(cbObj),
		_parentNote(parent),
		_egState(EG_NOT_YET),
		_dacCounter(0){}
		virtual ~Eg4segment( void ){}
		virtual void	moveToRelease( void ){ toRelease(); }
		
		//	Judge Segment change
		virtual void	periodicOnceEveryProcess( void );		//	PT
		virtual void	periodicOnceEveryDac( long dacCnt );	//	PT
		virtual double	calcEgLevel( void );					//	PT
		
	private:
		//	Move to next EG Segment
		void	toAttack( void );			//	EG_NOT_YET -> ATTACK
		void	toDecay1( void );			//	ATTACK -> DECAY1
		void	toDecay2( void );			//	DECAY1 -> DECAY2
		void	toKeyOnSteady( void );		//	keep DECAY2
		void	toRelease( void );			//	any Segment -> RELEASE
		void	toKeyOffSteady( void );		//	keep RELEASE
		
		//	Get Segment Parameter
		int		getAttackDacCount( void ){ return _cbFunc.getEgTime(EG_ATTACK); }
		int		getDecay1DacCount( void ){ return _cbFunc.getEgTime(EG_DECAY1); }
		int		getDecay2DacCount( void ){ return _cbFunc.getEgTime(EG_DECAY2); }
		int		getReleaseDacCount( void ){ return _cbFunc.getEgTime(EG_RELEASE); }
		double	getInitialLevel( void ){ return _cbFunc.getEgLvl(EG_NOT_YET); }
		double	getAttackLevel( void ){ return _cbFunc.getEgLvl(EG_ATTACK); }
		double	getDecay1Level( void ){ return _cbFunc.getEgLvl(EG_DECAY1); }
		double	getDecay2Level( void ){ return _cbFunc.getEgLvl(EG_DECAY2); }
		double	getReleaseLevel( void ){ return _cbFunc.getEgLvl(EG_RELEASE); }
		
		//	EG Level
		double		_egStartLevel;			//	-1-0-+1
		double		_egTargetLevel;			//	-1-0-+1
		double		_egCrntLevel;			//	-1-0-+1
		
		//	EG Time Manage
		long		_egStartDac;
		long		_egTargetDac;
		long		_dacCounter;
		EG_STATE	_egState;
		
		//	Relation
		CallBack&	_cbFunc;
		Note&		_parentNote;
	};
}
#endif /* defined(__ToneGenerator__msgf_eg4seg__) */
