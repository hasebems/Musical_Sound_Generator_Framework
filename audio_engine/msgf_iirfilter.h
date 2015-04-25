//
//  msgf_iirfilter.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2013/01/20.
//  Copyright (c) 2013年 長谷部 雅彦. All rights reserved.
//

#ifndef __msgf_iirfilter__
#define __msgf_iirfilter__

#include <iostream>
#include "msgf_type.h"
#include "msgf_signal_process_core.h"
#include "msgf_eg2seg.h"
#include "msgf_note.h"

namespace msgf {

	//	to be defined in every Audio Engine that demands an original parameter set
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
	class FegCallBack;
	//---------------------------------------------------------
	class IirFilter : public SignalProcessCore {

		friend class FegCallBack;

	public:
		IirFilter( Note& parent );
		~IirFilter( void );
		
		void	init( void );
		void	release( void ){ _eg->moveToRelease(); }

		//	process thread
		void	process( TgAudioBuffer& buf );
		
		static const int FEG_MAX = 100;
		static const int FEG_DEPTH_MAX = 16; // *Fc[Hz]
		
	private:
		double	calcFreq( double fc, int prm );
		void	setOneCoef( double fc, double qValue );

		//	Original
		int		getVoicePrm( int id ){ return _parentNote.getVoiceContext()->getParameter( VP_FILTER_ID, id ); }
		int		getAttackDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_FEG_ATTACK_TIME)); }
		int		getReleaseDacCount( void ){ return getTotalDacCount(getVoicePrm(VP_FEG_RELEASE_TIME)); }
		
		void	getFegCoef( Coef& cf );
		
		//	Basic Reference
		Note&	_parentNote;
		
		//	FEG
		FegCallBack*	_cbInst;
		Eg2segment*		_eg;
		
		//	Basic Value
		Coef	_crntCf;
		
		double	_frqRatio;
		double	_baseFc;
		double	_baseQ;
		double	_fcOld;	//	to omit same result
		double	_qOld;
		
		double	_x_m2;
		double	_x_m1;
		double	_y_m2;
		double	_y_m1;
	};
	//---------------------------------------------------------
	class FegCallBack : public CallBack {
		
	public:
		FegCallBack( IirFilter* ifltr ):
		_ifltr(ifltr){}
		virtual double	getEgLvl( EG_STATE prm )
		{
			switch (prm){
				case EG_ATTACK:	 return static_cast<double>(_ifltr->getVoicePrm( VP_FEG_ATTACK_LEVEL ))/100;
				case EG_RELEASE: return static_cast<double>(_ifltr->getVoicePrm( VP_FEG_RELEASE_LEVEL ))/100;
				default: return 0;
			}
		}
		virtual int		getEgTime( EG_STATE prm )
		{
			switch (prm){
				case EG_ATTACK:	 return _ifltr->getAttackDacCount();
				case EG_RELEASE: return _ifltr->getReleaseDacCount();
				default: return 0;
			}
		}
	private:
		IirFilter*	_ifltr;
	};
}
#endif /* defined(__msgf_iirfilter__) */
