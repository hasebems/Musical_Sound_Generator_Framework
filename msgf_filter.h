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

namespace msgf {
//---------------------------------------------------------
typedef enum {
	
	FEG_NOT_YET,
	FEG_ATTACK,
	KEY_ON_STEADY,
	FEG_RELEASE,
	FEG_STATE_MAX

} FEG_STATE;
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
class TgAudioBuffer;
//---------------------------------------------------------
class Filter {

public:
	Filter( Note* parent );
	~Filter( void ){}

	void	setCoef( double freq, double qValue );
	void	setOneCoef( double fc, double qValue, Coef& cf );
	
	void	process( TgAudioBuffer& buf );

	static const int FEG_MAX = 32;
	static const int FEG_DEPTH_MAX = 8; // *Fc[Hz]
	
private:
	void	toAttack( void );
	void	toSteady( void );
	void	toRelease( void );
	int		getTotalDacCount( int time );
	Coef*	getFegCoef( void );
	
	Note*	_parentNote;

	Coef	_center;
	Coef	_upper[FEG_MAX];
	Coef	_lower[FEG_MAX];

	double	_x_m2;
	double	_x_m1;
	double	_y_m2;
	double	_y_m1;

	long	_dacCounter;
	long	_startDac;
	long	_targetDac;

	int		_fegStartLevel;
	int		_fegCrntLevel;
	int		_fegLevel;
	FEG_STATE	_state;
};
}
#endif /* defined(__ToneGenerator__msgf_filter__) */
