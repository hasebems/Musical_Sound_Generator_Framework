//
//  AudioOutput.m
//  ToneGenerator
//
//  Created by 長谷部 雅彦 on 2012/11/07.
//  Copyright (c) 2012年 長谷部 雅彦. All rights reserved.
//

#import "AudioOutput.h"
#import	"msgf_audio_buffer.h"
#import "msgf_if.h"

@implementation AudioOutput
//--------------------------------------------------------
#define		SMPL_RATE			44100


//--------------------------------------------------------
//		Initialize
//--------------------------------------------------------
- (id) init
{
    self = [super init];
    if (self != nil) {
        [self setupOutputUnit];
		AudioOutputUnitStart(_outputUnit);
		
		_tg = new msgf::Msgf();
    }
    return self;
}

//--------------------------------------------------------
//
//--------------------------------------------------------
- (void)dispose
{
    AudioUnitUninitialize(_outputUnit);
    AudioComponentInstanceDispose(_outputUnit);
    _outputUnit = NULL;
}

//--------------------------------------------------------
//		End
//--------------------------------------------------------
- (void) dealloc
{
    [self dispose];

	msgf::Msgf*	tg = static_cast<msgf::Msgf*>(_tg);
	delete tg;
}

//--------------------------------------------------------
//		Set Callback Function
//--------------------------------------------------------
static OSStatus OutputCallback(void *inRefCon,
                               AudioUnitRenderActionFlags *ioActionFlags,
                               const AudioTimeStamp *inTimeStamp,
                               UInt32 inBusNumber,
                               UInt32 inNumberFrames,
                               AudioBufferList *ioData)
{
	return [(__bridge AudioOutput*)inRefCon process:ioActionFlags
										  timestamp:inTimeStamp
											 busnum:inBusNumber
										  numframes:inNumberFrames
											   data:ioData];
}

//--------------------------------------------------------
//		Settings
//--------------------------------------------------------
- (void)setupOutputUnit
{
    //	Audio Unit : Audio Component Description の作成
	AudioComponentDescription desc;
    desc.componentType = kAudioUnitType_Output;
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
	
	//	Initialize AU
    AudioComponent component = AudioComponentFindNext(NULL, &desc);
    AudioComponentInstanceNew(component, &_outputUnit);
    AudioUnitInitialize(_outputUnit);
	
	//	Setting call back function
    AURenderCallbackStruct callback;
    callback.inputProc = OutputCallback;
    callback.inputProcRefCon = (__bridge void*)self;
    AudioUnitSetProperty(_outputUnit,
                         kAudioUnitProperty_SetRenderCallback,
                         kAudioUnitScope_Global,
                         0,
                         &callback,
                         sizeof(AURenderCallbackStruct));
	
	//	set ASBD
	AudioStreamBasicDescription outputFormat;
	outputFormat.mSampleRate	= SMPL_RATE;
	outputFormat.mFormatID		= kAudioFormatLinearPCM;
	outputFormat.mFormatFlags	= kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	outputFormat.mChannelsPerFrame = 2;
	outputFormat.mFramesPerPacket	= 1;
	outputFormat.mBitsPerChannel	= 16;
	outputFormat.mBytesPerFrame		= outputFormat.mBitsPerChannel / 8 * outputFormat.mChannelsPerFrame;
	outputFormat.mBytesPerPacket	= outputFormat.mBytesPerFrame * outputFormat.mFramesPerPacket;
	outputFormat.mReserved			= 0;
	AudioUnitSetProperty(_outputUnit,
                         kAudioUnitProperty_StreamFormat,
                         kAudioUnitScope_Input,
                         0,
                         &outputFormat,
                         sizeof(outputFormat));
	
    UInt32 size = sizeof(outputFormat);
    AudioUnitGetProperty(_outputUnit,
                         kAudioUnitProperty_StreamFormat,
                         kAudioUnitScope_Global,
                         0,
                         &outputFormat,
                         &size);
    
    NSLog(@"samplerate = %f", outputFormat.mSampleRate);
    NSLog(@"bits = %u", (unsigned int)outputFormat.mBitsPerChannel);
    NSLog(@"channels = %u", (unsigned int)outputFormat.mChannelsPerFrame);
    NSLog(@"%@",
          (outputFormat.mFormatFlags & kLinearPCMFormatFlagIsNonInterleaved) ?
          @"non interleaved" : @"interleaved");
}

//--------------------------------------------------------
//		Audio Process
//--------------------------------------------------------
- (OSStatus) process: (AudioUnitRenderActionFlags*) ioActionFlags
		   timestamp: (const AudioTimeStamp*) inTimeStamp
			  busnum: (UInt32) inBusNumber
		   numframes: (UInt32) inNumberFrames
				data: (AudioBufferList*) ioData
{
    OSStatus err = noErr;

	msgf::TgAudioBuffer	abuf;						//	MSGF IF
	msgf::Msgf*	tg = static_cast<msgf::Msgf*>(_tg);
	abuf.obtainAudioBuffer(inNumberFrames);			//	MSGF IF
	tg->process( abuf );							//	MSGF IF
	
	for (NSInteger i = 0; i < ioData->mNumberBuffers; i++) {
		//2011/11/19 iOS5.0対応
        SInt16 *ptr = reinterpret_cast<SInt16*>(ioData->mBuffers[i].mData);
		UInt32 channels = ioData->mBuffers[i].mNumberChannels;

        for (NSInteger j = 0; j < inNumberFrames; j++) {
			SInt16 dt = static_cast<SInt16>(abuf.getAudioBuffer(j) * 15000);
            for (NSInteger k = 0; k < channels; k++) {
				ptr[j * channels + k] = dt;
            }
        }
    }

	abuf.releaseAudioBuffer();						//	MSGF IF
	return err;
}

//--------------------------------------------------------
//		Receive MIDI Command
//--------------------------------------------------------
- (void)reveiveMidi:(UInt8)firstByte
			   msg2:(UInt8)secondByte
			   msg3:(UInt8)thirdByte
{
	UInt8	msg[3];
	msgf::Msgf*	tg = static_cast<msgf::Msgf*>(_tg);

	msg[0] = firstByte;
	msg[1] = secondByte;
	msg[2] = thirdByte;
	tg->sendMessage( 3, msg );						//	MSGF IF
}
@end
