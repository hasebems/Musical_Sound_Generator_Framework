//
//  msgf_instrument_for_check.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/05/25.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_instrument_for_check__
#define __msgf_instrument_for_check__

#include <iostream>
#include "msgf_instrument.h"

namespace msgf {
	//---------------------------------------------------------
	class InstrumentForCheck : public Instrument {
	
	public:
		InstrumentForCheck( Part* pt, int vid ):
			Instrument(pt,vid),
			_soundOn(false),
			_crntPhase(0),
			_diff(0){}
		virtual ~InstrumentForCheck( void ){}
	
		void	keyOn( Uint8 note, Uint8 velocity );
		void	keyOff( Uint8 note, Uint8 velocity );
		
		//	process thread
		void	process( TgAudioBuffer& buf );

	private:
		bool	_soundOn;
		double	_crntPhase;
		double	_diff;
	};
}
#endif /* defined(__msgf_instrument_for_check__) */
