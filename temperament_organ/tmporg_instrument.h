//
//  tmporg_instrument.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2015/07/18.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#ifndef __tmporg_instrument__
#define __tmporg_instrument__

#include <iostream>
#include "msgf_instrument.h"
#include "msgf_osc_tmpr.h"

//---------------------------------------------------------
class TmporgInstrument : public msgf::Instrument, msgf::CallBackForTemperament {
	
public:
	TmporgInstrument( msgf::Part* pt, int vid );
	~TmporgInstrument( void );
	
	void	keyOn( msgf::Uint8 note, msgf::Uint8 velocity );
	void	keyOff( msgf::Uint8 note, msgf::Uint8 velocity );
//	void 	allSoundOff( void );
	void	specialCommand( msgf::Uint8 statusByte, msgf::Uint8 parameter1, msgf::Uint8 parameter2 );

	double	getCent( int doremi, int velVari );
	
private:
	msgf::Uint8		_lastNote;
	int				_konCount[128];

	//	Pitch Command
	double			_tmperamentPitch[4][12];	// resolution: 0.01[cent] difference from equal.temperament
	int				_setPitchCounter;
	
};
#endif /* defined(__tmporg_instrument__) */
