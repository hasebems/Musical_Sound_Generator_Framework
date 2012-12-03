//
//  msgf_filter.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/18.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#ifndef __ToneGenerator__msgf_filter__
#define __ToneGenerator__msgf_filter__

#include <iostream>
#include "msgf_type.h"
#include "msgf_signal_process_core.h"

namespace msgf {
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
class Note;
//---------------------------------------------------------
class Filter : public SignalProcessCore {

public:
	Filter( Note* parent );
	~Filter( void ){}

	void	setCoef( double freq, double qValue );
	void	setOneCoef( double fc, double qValue, Coef& cf );
	
	void	init( void );
	void	process( TgAudioBuffer& buf );

	static const int FEG_MAX = 32;
	static const int FEG_DEPTH_MAX = 16; // *Fc[Hz]
	
private:
	//	override
	void	toAttack( void );
	void	toSteady( void );
	void	toRelease( void );
	Coef*	getFegCoef( void );

	//	FEG
	Coef	_center;
	Coef	_upper[FEG_MAX];
	Coef	_lower[FEG_MAX];
	int		_fegStartLevel;
	int		_fegCrntLevel;
	int		_fegLevel;

	double	_x_m2;
	double	_x_m1;
	double	_y_m2;
	double	_y_m1;
};
}
#endif /* defined(__ToneGenerator__msgf_filter__) */
