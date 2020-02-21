//
//  msgf_if.h
//
//	Musical Sound Generator Framework
//
//  Created by Hasebe Masahiko on 2012/09/22.
//  Copyright (c) 2012 Hasebe Masahiko. All rights reserved.
//

#ifndef __msgf_if__
#define __msgf_if__

#include <iostream>

#include "msgf_type.h"
#include "msgf_part.h"
#include "msgf_voice_context.h"

namespace msgf {
	//---------------------------------------------------------
	const int MAX_PART_NUM = 1;
	const Uint8	DONT_CARE = 0xff;

	//---------------------------------------------------------
	class TgAudioBuffer;
	//---------------------------------------------------------
	class Msgf {

	public:
		Msgf();
		~Msgf();
	
		void	sendMessage( int maxMessage, Uint8* message );
		void	makeChMessage( Uint8 byteData );
		void	analyzePartOfChMessage( Uint8 dt1, Uint8 dt2, Uint8 dt3 );
		void	reduceResource( void );

		//	process thread
		void	process( TgAudioBuffer& inputBuf );

		//	Accessor
		Part*	part( int number ) const { return _part[number]; }
		VoiceContext*	vc( void ) const { return _vc; }

	private:
		//	Composite Object
		Part*	_part[MAX_PART_NUM];

		//	Member Variables
		Uint8	_tempFirstByte;
		Uint8	_tempSecondByte;
		int		_tempCounter;

		VoiceContext*	_vc;
	};
}
#endif /* defined(__msgf_if__) */
