//
//  msgf_type.h
//
//	Musical Sound Generator Framework
//
//  Created by Hasebe Masahiko on 2012/09/23.
//  Copyright (c) 2012 Hasebe Masahiko. All rights reserved.
//

#ifndef __msgf_type__
#define __msgf_type__
#include <stdint.h>

namespace msgf {

	//	define type
	typedef	uint8_t		Uint8;
	typedef	uint16_t	Uint16;
	typedef	uint32_t	Uint32;

	typedef	int8_t		Sint8;
	typedef	int16_t		Sint16;
	typedef	int32_t		Sint32;

	//	define const value
	const int	SAMPLING_FREQUENCY = 44100;

}
#endif
