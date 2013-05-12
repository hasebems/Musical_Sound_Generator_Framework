//
//  msgf_configration.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_configration.h"

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
		case 0:	pinst = new VasInstrument(pt,instId); break;
		default: break;
	}

	return pinst;
}

