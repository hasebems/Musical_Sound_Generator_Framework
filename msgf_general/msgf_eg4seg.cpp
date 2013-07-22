//
//  msgf_eg4seg.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/02/02.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_eg4seg.h"

using namespace msgf;

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Eg4segment::toAttack( void )
{
	//	time
	_egState = EG_ATTACK;
	_egStartDac = 0;
	_egTargetDac = _egStartDac + getAttackDacCount();
	
	//	level
	_egStartLevel = getInitialLevel();
	_egTargetLevel = getAttackLevel();
}
//---------------------------------------------------------
void Eg4segment::toDecay1( void )
{
	//	time
	_egState = EG_DECAY1;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac + getDecay1DacCount();
	
	//	level
	_egStartLevel = _egTargetLevel;
	_egTargetLevel = getDecay1Level();
}
//---------------------------------------------------------
void Eg4segment::toDecay2( void )
{
	//	time
	_egState = EG_DECAY2;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac + getDecay2DacCount();
	
	//	level
	_egStartLevel = _egTargetLevel;
	_egTargetLevel = getDecay2Level();
}
//---------------------------------------------------------
void Eg4segment::toKeyOnSteady( void )
{
	//	time
	_egState = EG_KEY_ON_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;
	
	//	level
	_egCrntLevel = _egTargetLevel;
}
//---------------------------------------------------------
void Eg4segment::toRelease( void )
{
	//	time
	_egState = EG_RELEASE;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac + getReleaseDacCount();
	
	//	level
	_egTargetLevel = getReleaseLevel();
	_egStartLevel = _egCrntLevel;
}
//---------------------------------------------------------
void Eg4segment::toKeyOffSteady( void )
{
	//	time
	_egState = EG_KEY_OFF_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;
	
	//	level
	_egCrntLevel = _egTargetLevel;	
}

//---------------------------------------------------------
//		Called from Process Function
//---------------------------------------------------------
void Eg4segment::periodicOnceEveryProcess( void )
{
	switch (_egState){
		case EG_KEY_OFF_STEADY:
		case EG_NOT_YET:{
			if ( _parentNote.conditionKeyOn() == true ){
				//	Start key On
				toAttack();
			}
			break;
		}
		case EG_ATTACK:
		case EG_DECAY1:
		case EG_DECAY2:
		case EG_KEY_ON_STEADY:{
			if ( _parentNote.conditionKeyOn() == false ){
				//	Key Off
				toRelease();
			}
			break;
		}
		default: break;
	}
}
//---------------------------------------------------------
void Eg4segment::periodicOnceEveryDac( long dacCnt )
{
	_dacCounter = dacCnt;
	if ( _dacCounter < _egTargetDac ) return;
	
	switch (_egState){
		case EG_ATTACK:
			toDecay1();
			break;
		case EG_DECAY1:
			toDecay2();
			break;
		case EG_DECAY2:
			toKeyOnSteady();
			break;
		case EG_RELEASE:
			toKeyOffSteady();
			break;
		default: break;
	}
}
//---------------------------------------------------------
double Eg4segment::calcEgLevel( void )
{
	long time = _dacCounter-_egStartDac;
	long targetTime = _egTargetDac-_egStartDac;
	double	diffLvl = _egTargetLevel-_egStartLevel;
	
	if ( targetTime == 0 ) return _egCrntLevel;
	
	//	Just in case
	if ( time >= targetTime ) time = targetTime-1;

	time = (time/4)<<2;		//	thin out for performance
	_egCrntLevel = diffLvl*time/targetTime + _egStartLevel;
	
	return _egCrntLevel;
}

