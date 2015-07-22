//
//  tmporg_note.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2015/07/18.
//  Copyright (c) 2015年 長谷部 雅彦. All rights reserved.
//

#ifndef __tmporg_note__
#define __tmporg_note__

#include <iostream>
#include "msgf_note.h"
#include "msgf_iirfilter.h"
#include "msgf_osc_tmpr.h"
#include "msgf_amp2seg.h"
#include "msgf_event_info.h"

//---------------------------------------------------------
class TmporgNote : public msgf::Note {
	
public:
	TmporgNote( msgf::Instrument* inst );
	~TmporgNote( void );
	bool	keyOn( msgf::EventInfo* ei );
	void	keyOff( void );
	//	void	damp( void );

	//void	keyOnAlternate( msgf::EventInfo* ei );
	void	changeNote( msgf::EventInfo* ei );
	bool	noteSoundProcess( msgf::TgAudioBuffer& buf );

	msgf::OscillatorTmpr*	getOscTmpr( void ) const { return _osc;}
	
private:
	//	Audio Engines
	msgf::OscillatorTmpr*	_osc;
	msgf::IirFilter*		_flt;
	msgf::Amp2seg*			_amp;
};

#endif /* defined(__tmporg_note__) */
