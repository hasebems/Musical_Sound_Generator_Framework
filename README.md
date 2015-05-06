# Musical Sound Generator Framework (MSGF)

This software is released under the MIT License, see LICENSE.txt.

## Outline
MSGF is being developed to provide as a software framework of musical synthesizer and an audio engine that works with it.

## Environment
If you make Xcode project and add these files to it, it will work as a MIDI based software synthesizer as an iOS app.
I confirmed it also works on Raspberry Pi with ALSA.

## How to use ( case of iOS )
1. Include "AudioOutput.h", create an instance of AudioOutput class.
2. Call Init() when it initializes.
3. Send MIDI data to receiveMidi(). And then you can get sound.


## 概要
MSGFは、ソフトシンセサイザー実装時の基本的なフレームワークとなることを目指しつつ、そのフレームワーク内で利用出来るいくつかのオーディオ処理エンジンを提供することを目的とします。

## 利用環境
Xcodeのプロジェクトに全ファイルを追加することによって、iOSアプリでMIDIベースのデータを奏でるためのソフトシンセとして動作します。ラズベリーパイ(ALSA使用)上での動作も確認しました。

## 接続方法 ( iOSの場合 )
1. AudioOutput.h をインクルードして、AudioOutput classのインスタンスを生成します。
2. 初期化時に Init をコールします。
3. receiveMidi にMIDIデータを送ります。これで音が鳴ります。
