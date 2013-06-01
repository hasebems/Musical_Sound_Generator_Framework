//
//  msgf_eg2seg.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_eg2seg.h"

using namespace msgf;

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void Eg2segment::toAttack( void )
{
	//	time
	_egState = EG_ATTACK;
	_egStartDac = 0;
	_egTargetDac = _egStartDac + getAttackDacCount();

	//	level
	_egStartLevel = getAttackLevel();
	
}
//---------------------------------------------------------
void Eg2segment::toKeyOnSteady( void )
{
	//	time
	_egState = EG_KEY_ON_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;

	//	level
	_egCrntLevel = _egTargetLevel = 0;
}
//---------------------------------------------------------
void Eg2segment::toRelease( void )
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
void Eg2segment::toKeyOffSteady( void )
{
	//	time
	_egState = EG_KEY_OFF_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;

	//	level
	
}

//---------------------------------------------------------
//		Called from Process Function
//---------------------------------------------------------
void Eg2segment::periodicOnceEveryProcess( void )
{
	switch (_egState){
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
void Eg2segment::periodicOnceEveryDac( long dacCnt )
{
	_dacCounter = dacCnt;
	if ( _dacCounter < _egTargetDac ) return;
	
	switch (_egState){
		case EG_ATTACK:
			toKeyOnSteady();
			break;
		case EG_RELEASE:
			toKeyOffSteady();
			break;
		default: break;
	}
}
//---------------------------------------------------------
double Eg2segment::calcEgLevel( void )
{
	long time = _dacCounter-_egStartDac;
	long targetTime = _egTargetDac-_egStartDac;
	
	if ( targetTime == 0 ) return _egCrntLevel;
	
	if ( time >= targetTime ) time = targetTime-1;
	
	if ( _egState == EG_ATTACK ){
		_egCrntLevel = (targetTime-1-time)*_egStartLevel/targetTime;
	}
	else if ( _egState == EG_RELEASE ){
		_egCrntLevel = time*(_egTargetLevel-_egStartLevel)/targetTime + _egStartLevel;
	}
	
	return _egCrntLevel;
}
