# Musical Sound Generator Framework (MSGF)

## Outline
MSGF is being developed to provide as a software framework of musical synthesizer and an audio engine that works with it.

## Environment
If you make Xcode project and add these files to it, it will work as a MIDI based software synthesizer as an iOS app.

## How to use
1. Include "AudioOutput.h", create an instance of AudioOutput class.
2. Call Init() when it initializes.
3. Send MIDI data to receiveMidi(). And then you can get sound.

## Future Plan
I will try to make this framework work on a general ARM-Linux board like Raspberry pi.
Finally I want to make an original electric music keyboard.


## 概要
MSGFは、ソフトシンセサイザー実装時の基本的なフレームワークとなることを目指しつつ、そのフレームワーク内で利用出来るいくつかのオーディオ処理エンジンを提供することを目的とします。

## 利用環境
Xcodeのプロジェクトに全ファイルを追加することによって、iOSアプリでMIDIベースのデータを奏でるためのソフトシンセとして動作します。

## 接続方法
1. AudioOutput.h をインクルードして、AudioOutput classのインスタンスを生成します。
2. 初期化時に Init をコールします。
3. receiveMidi にMIDIデータを送ります。これで音が鳴ります。

## 将来目指すこと
このFrameworkを汎用ARM-Linuxボード(Raspberry piなど)上で動作させ、鍵盤などを接続することによってオリジナル電子楽器を作ることを目指しています。
