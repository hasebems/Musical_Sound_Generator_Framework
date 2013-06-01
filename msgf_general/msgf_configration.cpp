//
//  msgf_configration.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_configration.h"
#include "msgf_instrument_for_check.h"

//	append particular Instrument
#include "vas_instrument.h"

using namespace msgf;

//---------------------------------------------------------
//		get Instrument
//---------------------------------------------------------
Instrument* InstrumentFactory::getInstrument( Part* pt, int instId )
{
	Instrument*	pinst = 0;
	
	switch (instId){
		//	instId = (16384 * Bank MSB) + (128 * Bank LSB) + Program Change Number
		case 0:	pinst = new VasInstrument(pt,instId); break;
		case 127: pinst = new InstrumentForCheck(pt,0); break;
		default: break;
	}

	return pinst;
}

