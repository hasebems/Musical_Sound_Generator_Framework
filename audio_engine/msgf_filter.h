//
//  msgf_filter.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/18.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_filter__
#define __msgf_filter__

#include <iostream>
#include "msgf_type.h"
#include "msgf_signal_process_with_eg.h"
#include "msgf_note.h"

namespace msgf {

	const int VP_FILTER_ID = 100;
	//---------------------------------------------------------
	typedef enum {
		
		VP_FILTER_TYPE,			//	0:LPF,
		VP_FILTER_CUTOFF,		//	[Hz]
		VP_FILTER_RESO,			//	1 - 100 ?
		
		VP_FEG_ATTACK_TIME,		//	0 - 1000 (*10ms)
		VP_FEG_ATTACK_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
		VP_FEG_RELEASE_TIME,	//	0 - 1000 (*10ms)
		VP_FEG_RELEASE_LEVEL,	//	-100 - 0 - +100 (1/4 - 4[times*Hz])
		
		VP_FILTER_MAX
		
	} VoiceParameter_Filter;
	//---------------------------------------------------------
	struct Coef {
		
		Coef( void ):_a1(0),_a2(0),_b0(0),_b1(0),_b2(0) {}
		double	_a1;
		double	_a2;
		double	_b0;
		double	_b1;
		double	_b2;
	};
	//---------------------------------------------------------
	class Filter : public SignalProcessWithEG {
		
	public:
		Filter( Note* parent );
		~Filter( void );
		
		void	init( void );
		void	process( TgAudioBuffer& buf );
		
		static const int FEG_MAX = 100;
		static const int FEG_DEPTH_MAX = 16; // *Fc[Hz]
		
	private:
		//	override
		void	toAttack( void );
		void	toKeyOnSteady( void );
		void	toRelease( void );
		void	getFegCoef( Coef& cf );

		//	Original
		int		getVoicePrm( int id ){ return _parentNote->getVoiceContext()->getParameter( VP_FILTER_ID, id ); }
		int		getAttackDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_FEG_ATTACK_TIME)); }
		int		getReleaseDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_FEG_RELEASE_TIME)); }

		double	calcFreq( double fc, int prm );
		void	setOneCoef( double fc, double qValue, Coef& cf );

		//	FEG
		Coef	_center;
		int		_fegStartLevel;
		int		_fegCrntLevel;
		int		_fegTargetLevel;
		
		//	Basic Value
		double	_frqRatio;
		double	_baseFc;
		double	_baseQ;

		double	_x_m2;
		double	_x_m1;
		double	_y_m2;
		double	_y_m1;
	};
}
#endif /* defined(__msgf_filter__) */
