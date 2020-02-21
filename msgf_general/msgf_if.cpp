//
//  msgf_if.cpp
//
//	Musical Sound Generator Framework
//
//  Created by Hasebe Masahiko on 2012/09/22.
//  Copyright (c) 2012 Hasebe Masahiko. All rights reserved.
//

#include "msgf_if.h"
#include "msgf_note.h"

using namespace msgf;

//---------------------------------------------------------
//		msgf Constructor/Destructor
//---------------------------------------------------------
Msgf::Msgf( void )
{
	for ( int i=0; i<MAX_PART_NUM; i++ ){
		_part[i] = new Part(this);
	}
}
//---------------------------------------------------------
Msgf::~Msgf()
{
	for ( int i=0; i<MAX_PART_NUM; i++ ){
		delete _part[i];
	}
}

//---------------------------------------------------------
//		msgf Entrance
//---------------------------------------------------------
void Msgf::sendMessage( int maxMessage, Uint8* message )
{
	if ( maxMessage	== 1 ) makeChMessage( *message );

	else {
		analyzePartOfChMessage( *message, *(message+1), *(message+2) );
	}
}

//---------------------------------------------------------
//		Make united MIDI Ch Message
//---------------------------------------------------------
void Msgf::makeChMessage( Uint8 byteData )
{
	//	if Exclusive, Don't do anything
	if (( byteData & 0xf0 ) == 0xf0 ) return;

	if (( _tempCounter == 0 ) && ( byteData & 0x80 )){
		_tempFirstByte = byteData;
		_tempCounter = 1;
	}
	else if (( _tempCounter == 1 ) && (( byteData & 0x80 ) == 0 )){
		_tempSecondByte = byteData;
		switch ( _tempFirstByte ) {
			case 0xd0:
			case 0xc0:
				_tempCounter = 0;
				analyzePartOfChMessage( _tempFirstByte, _tempSecondByte, DONT_CARE );
				break;
				
			default:
				_tempCounter = 2;
				break;
		}
	}
	else if (( _tempCounter == 2 ) && (( byteData & 0x80 ) == 0 )){
		_tempCounter = 0;
		analyzePartOfChMessage( _tempFirstByte, _tempSecondByte, byteData );
	}	
}

//---------------------------------------------------------
//		Analyze & Call Part
//---------------------------------------------------------
void Msgf::analyzePartOfChMessage( Uint8 dt1, Uint8 dt2, Uint8 dt3 )
{
	int part =  dt1 & 0x0f;
	int status = dt1 & 0xf0;

	if ( status == 0xf0 ) {
		for ( int i=0; i<MAX_PART_NUM; i++ ){
			_part[i]->specialCommand( dt1, dt2, dt3 );
		}
	}

	if ( part >= MAX_PART_NUM ) return;
	
	switch ( status ) {
		case 0x80:
			_part[part]->keyOff( dt2, dt3 );
			break;
		case 0x90:
			if ( dt3 == 0 ) _part[part]->keyOff( dt2, 0x40 );
			else _part[part]->keyOn( dt2, dt3 );
			break;
		case 0xb0:
			_part[part]->controlChange( dt2, dt3 );
			break;
		case 0xc0:
			_part[part]->programChange( dt2 );
			break;
		case 0xe0:{
			int bendValue = dt3;
			bendValue += dt2*128;
			bendValue -= 8192;
			_part[part]->pitchBend( bendValue );
			break;
		}
		default:
			break;
	}
}

//---------------------------------------------------------
//		Analyze & Call Part
//---------------------------------------------------------
void Msgf::reduceResource( void )
{
	double	lvl = 1;
	Note*	ntRsc = nullptr;

	for ( int i=0; i<MAX_PART_NUM; i++ ){
		Note *ntPtr = nullptr;
		double ptLvl = _part[i]->getInstrument()->searchMinimumLevelNote( &ntPtr );
		if (( ntPtr != nullptr ) && ( ptLvl < lvl )){
			lvl = ptLvl;
			ntRsc = ntPtr;
		}
	}

	if ( ntRsc ) ntRsc->damp();
}

//---------------------------------------------------------
//		Audio Process Function
//---------------------------------------------------------
void Msgf::process( TgAudioBuffer& inputBuf )
{
	for ( int i=0; i<MAX_PART_NUM; i++ ){
		_part[i]->process( inputBuf );
	}
}

