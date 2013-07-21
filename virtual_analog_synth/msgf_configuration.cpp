//
//  msgf_configuration.cpp
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/06.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#include "msgf_configuration.h"
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

		//	VAS (virtual analog Synth)
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
		case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15:{
			pinst = new VasInstrument(pt,instId);
			break;
		}

		case 127: pinst = new InstrumentForCheck(pt,0); break;
		default: break;
	}

	return pinst;
}

