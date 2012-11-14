//
//  AudioOutput.h
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/07.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>

@interface AudioOutput : NSObject {

	AudioUnit	_outputUnit;
	void*		_tg;
}

- (id) init;
- (void)dispose;
- (void)dealloc;
- (void)setupOutputUnit;
- (OSStatus) process: (AudioUnitRenderActionFlags*) ioActionFlags
		   timestamp: (const AudioTimeStamp*) inTimeStamp
			  busnum: (UInt32) inBusNumber
		   numframes: (UInt32) inNumberFrames
				data: (AudioBufferList*) ioData;
- (void)reveiveMidi:	(UInt8)firstByte
			   msg2:	(UInt8)secondByte
			   msg3:	(UInt8)thirdByte;

@end
