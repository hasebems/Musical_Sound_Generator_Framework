//
//  msgf_configration.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_configration__
#define __msgf_configration__

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
#endif /* defined(__ToneGenerator__msgf_configration__) */
