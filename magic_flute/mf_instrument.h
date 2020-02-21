//
//  mf_instrument.h
//  ToneGenerator
//
//  Created by Hasebe Masahiko on 2013/07/19.
//  Copyright (c) 2013 Hasebe Masahiko. All rights reserved.
//

#ifndef __mf_instrument__
#define __mf_instrument__

#include <iostream>
#include "msgf_instrument.h"

//---------------------------------------------------------
class MfInstrument : public msgf::Instrument {
	
public:
	MfInstrument( msgf::Part* pt, int vid );
	~MfInstrument( void );
	
	void	keyOn( msgf::Uint8 note, msgf::Uint8 velocity );
	void	keyOff( msgf::Uint8 note, msgf::Uint8 velocity );
	void 	allSoundOff( void );

private:
	msgf::Uint8		_lastNote;
	int				_konCount[128];
};

#endif /* defined(__mf_instrument__) */
