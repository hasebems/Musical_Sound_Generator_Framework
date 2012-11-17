# Musical Sound Generator Framework (MSGF)

## 概要
MSGFは、ソフトシンセサイザー実装時の基本的なフレームワークとなることを目指しつつ、そのフレームワーク内で利用出来るいくつかのオーディオ処理エンジンを提供することを目的とします。

## 利用環境
Xcodeのプロジェクトに全ファイルを追加することによって、iOSアプリでMIDIベースのデータを奏でるためのソフトシンセとして動作します。

## 接続方法
1. AudioOutput.h をインクルードして、AudioOutput classのインスタンスを生成します。
2. 初期化時に Init をコールします。
3. receiveMidi にMIDIデータを送ります。これで音が鳴ります。

## 将来目指すこと
このFrameworkを汎用Linuxボード上で動作させ、鍵盤などを接続することによってオリジナル電子楽器を作ることを目指しています。
