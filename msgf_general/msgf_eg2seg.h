//
//  msgf_eg2seg.h
//  ToneGenerator
//
//  Created by Hasebe Masahiko on 2013/01/20.
//  Copyright (c) 2013 Hasebe Masahiko. All rights reserved.
//

#ifndef __msgf_eg2seg__
#define __msgf_eg2seg__

#include <iostream>
#include "msgf_note.h"
#include "msgf_eg.h"

namespace msgf {
	//---------------------------------------------------------
	class Eg2segment : public Eg {
		
		//	2segments mean :
		//		Attack	:	level: EG_ATTACK	->	0/1
		//		Release	:	level: 0/1			->	EG_RELEASE
		//			centerBased : true ->  Base Level 0
		//						: false -> Base Level 1
		
	public:
		Eg2segment( CallBack& cbObj, Note& parent, bool centerBased );
		virtual ~Eg2segment( void ){}
		virtual void	moveToRelease( void ){ toRelease(); }
		
		//	Judge Segment change
		virtual void	periodicOnceEveryProcess( void );
		virtual void	periodicOnceEveryDac( long dacCnt );
		virtual double	calcEgLevel( void );
		
		//	Accessor
		EG_STATE	getEgState( void ) const { return _egState;}
		
	private:
		//	Move to next EG Segment
		void	toAttack( EG_STATE state );	//	EG_NOT_YET/KEY_OFF_STEADY -> ATTACK
		void	toKeyOnSteady( void );		//	keep DECAY2
		void	toRelease( void );			//	any Segment -> RELEASE
		void	toKeyOffSteady( void );		//	keep RELEASE
		
		//	Get Segment Parameter
		int		getAttackDacCount( void ){ return _cbFunc.getEgTime(EG_ATTACK); }
		int		getReleaseDacCount( void ){ return _cbFunc.getEgTime(EG_RELEASE); }
		double	getAttackLevel( void ){ return _cbFunc.getEgLvl(EG_ATTACK); }
		double	getReleaseLevel( void ){ return _cbFunc.getEgLvl(EG_RELEASE); }
		
		//	EG Level
		double		_egStartLevel;			//	-1-0-+1
		double		_egTargetLevel;			//	-1-0-+1
		double		_egCrntLevel;			//	-1-0-+1
		double		_steadyLevel;			//	0/1
		
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
#endif /* defined(__ToneGenerator__msgf_eg__) */
