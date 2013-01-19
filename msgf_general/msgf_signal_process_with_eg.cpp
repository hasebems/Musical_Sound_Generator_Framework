//
//  msgf_signal_process_with_eg.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/13.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_signal_process_with_eg.h"

using namespace msgf;

//---------------------------------------------------------
//		Move to next segment
//---------------------------------------------------------
void SignalProcessWithEG::toAttack( void )
{
	//	time
	_state = ATTACK;
	_egStartDac = _dacCounter = 0;
	_egTargetDac = _egStartDac + getAttackDacCount();
}
//---------------------------------------------------------
void SignalProcessWithEG::toDecay1( void )
{
	//	time
	_state = DECAY1;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac + getDecay1DacCount();
}
//---------------------------------------------------------
void SignalProcessWithEG::toDecay2( void )
{
	//	time
	_state = DECAY2;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac + getDecay2DacCount();
}
//---------------------------------------------------------
void SignalProcessWithEG::toKeyOnSteady( void )
{
	//	time
	_state = KEY_ON_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;
}
//---------------------------------------------------------
void SignalProcessWithEG::toRelease( void )
{
	//	time
	_state = RELEASE;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac + getReleaseDacCount();
}
//---------------------------------------------------------
void SignalProcessWithEG::toKeyOffSteady( void )
{
	//	time
	_state = KEY_OFF_STEADY;
	_egStartDac = _dacCounter;
	_egTargetDac = _egStartDac;
}

//---------------------------------------------------------
//		Process Function
//---------------------------------------------------------
void SignalProcessWithEG::checkEvent( void )
{
	switch (_state){
		case EG_NOT_YET:{
			if ( _parentNote->conditionKeyOn() == true ){
				//	Start key On
				toAttack();
			}
			break;
		}
		case ATTACK:
		case DECAY1:
		case DECAY2:
		case KEY_ON_STEADY:{
			if ( _parentNote->conditionKeyOn() == false ){
				//	Key Off
				toRelease();
			}
			break;
		}
		default: break;
	}
}
//---------------------------------------------------------
void SignalProcessWithEG::checkSegmentEnd( void )
{
	if ( _dacCounter < _egTargetDac ) return;
	
	switch (_state){
		case ATTACK:
			toDecay1();
			break;
		case DECAY1:
			toDecay2();
			break;
		case DECAY2:
			toKeyOnSteady();
			break;
		case RELEASE:
			toKeyOffSteady();
			break;
		default: break;
	}
}
//---------------------------------------------------------
void SignalProcessWithEG::checkSegmentEnd2seg( void )
{
	if ( _dacCounter < _egTargetDac ) return;
	
	switch (_state){
		case ATTACK:
			toKeyOnSteady();
			break;
		case RELEASE:
			toKeyOffSteady();
			break;
		default: break;
	}
}
//---------------------------------------------------------
void SignalProcessWithEG::process( TgAudioBuffer& buf )
{
	//	check Event
	checkEvent();
	
	//	Filter Calculate
	for ( int i=0; i<buf.bufferSize(); i++ ){
		
		//	Check AEG Segment
		checkSegmentEnd();

		//	Audio Processing
		
		//	Counter Increment
		_dacCounter++;
	}
}
