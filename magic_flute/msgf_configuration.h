//
//  msgf_configuration.h
//  ToneGenerator
//
//  Created by Hasebe Masahiko on 2013/07/15.
//  Copyright (c) 2013 Hasebe Masahiko. All rights reserved.
//

#ifndef __msgf_configuration__
#define __msgf_configuration__

#include <iostream>
#include "msgf_type.h"

namespace msgf {
	
	//---------------------------------------------------------
	class Instrument;
	class Part;
	//---------------------------------------------------------
	class InstrumentFactory {
		
	public:
		Instrument* getInstrument( Part* pt, int instId );
	};
}
#endif /* defined(__ToneGenerator__msgf_configuration__) */
