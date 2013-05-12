//
//  vas_instrument.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __vas_instrument__
#define __vas_instrument__

#include <iostream>
#include "msgf_instrument.h"

//---------------------------------------------------------
class VasInstrument : public msgf::Instrument {
	
public:
	VasInstrument( msgf::Part* pt, int vid );
	~VasInstrument( void );

	void	keyOn( msgf::Uint8 note, msgf::Uint8 velocity );
};
#endif /* defined(__vas_instrument__) */
