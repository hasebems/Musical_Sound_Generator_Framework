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

//---------------------------------------------------------
class Note;
class TgAudioBuffer;
//---------------------------------------------------------
class Filter {

public:
	Filter( Note* parent );
	~Filter( void ){}
	
	void	process( TgAudioBuffer& buf );

	void	setCoef( double freq, double qValue );
	
	
private:
	Note*	_parentNote;

	double	_a1;
	double	_a2;
	double	_b0;
	double	_b1;
	double	_b2;

	double	_x_m2;
	double	_x_m1;
	double	_y_m2;
	double	_y_m1;
};
}
#endif /* defined(__ToneGenerator__msgf_filter__) */
