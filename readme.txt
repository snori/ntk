/******************************************************************************

	The NTK Library : README
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: readme.txt,v 1.2 2003/03/07 01:55:38 nsuzuki Exp $

******************************************************************************/


■ はじめに

Windows 用の C++ クラスライブラリです。MFC の類似品といえばわかりやすいでしょう
か。（とはいっても MFC ほど巨大では無いですが (^^;

まず、このライブラリの最大の特徴は BeOS の API （以下 BeAPI）に似せて作ってある
ということです。ただ開発の方向性として、使いやすい（と思っている）BeAPI を
Windows でも使えるようにする（主に自分のために）というのがありますので、BeOS の
ソフトをソースコンパチでビルドできる、というようなものは目指していません。その互
換性の無さの大部分は、コーディング規則の違い（関数名が小文字アルファベットとアン
ダースコア(_)のみによって書かれている等、詳しくは coding.txt 参照のこと）からき
ていると思いますが、（仕方なく）Windows の作法に合わせたりなんだりで所々、BeAPI
と違ってきてしまっている部分もあります。

# とはいっても、自分では結構いい感じで構築できたなと思っていますので、気に入って
# 使っていただけるとうれしいです。

もちろん意見は歓迎です。今のところは踏ん切りがつかないでいますが、そのうち
SourceForge あたりに移行しようとも思っていますので、開発に参加したいというかたが
もしおられましたらそういうメールも歓迎です。それと、BeOS でプログラミングしていた
方、意見いただけるととても助かります。


■ 概要

以下は NTK ライブラリの概要です。

 * NTK
	BeOS API 的でこのライブラリの中心。Window や View を扱う。

 * MOD(ule)
	SWIG (http://swig-jp.dyndns.org/, http://www.swig.org/) を使用し
	て各スクリプト向け(現在検証用のため Ruby で動作確認しています)の
	NTK ライブラリモジュール。

ドキュメントが無くて申し訳ないのですが、samples/ 以下のサンプルソースが少しは参
考になるかと思います。

まずは samples/sample02.dsw を開きアクティブプロジェクトを sample02 にしビルドし
てみてください。少々時間がかかりますが実行ボタンを押せば簡単なものですがサンプル
プログラムが実行されます。


**そのうちもっと詳しく書く予定**


■ 使用方法

以下の説明は Microsoft Visual C++ 6.0 の使用を前提にして書いてあります。

・プロジェクトの設定
	ヘッダ、ライブラリのディレクトリを設定する。プロジェクトのインクルードパスの
	設定でも、『ツール>オプション』のディレクトリの設定でもどちらでも構いません。

	新規の Win32 プロジェクトで以下の設定をする。
	 - ランタイムをマルチスレッド, DLL にする(デバッグはマルチスレッド, DLL, デ
	   バッグ)
	 - ランタイムタイプ情報（RTTI）を有効にする
	 - 使用するライブラリに ntk(s)(d).lib を追加する (ntkd.lib はダイナミックリ
	   ンク・デバッグ版、ntks.lib はスタティックリンク・リリース版、ntksd.lib は
	   スタティックリンク・デバッグ版のライブラリです)

・プログラムは int main(int argc, const char_t** argv) から始まります。

・main(...) のある .cpp ファイルの先頭では必ず <ntk/main.h> をインクルードします。

・v0.2.x 以降では boost ライブラリを使用するようになっています。コンパイルするには
  インクルードパスに boost を加えておく必要があります。


**そのうちもっと詳しく書く予定**


■ 参考

BeOS 関係の開発資料がたぶん参考になると思います。

	http://www.bebits.com/app/2680
	http://www.bezip.de/
	# 情報提供ありがとうございました。＞ 渋川さん


なお BeBook の日本語版がオライリー・ジャパンより出版されています。探せば
現在でも入手は可能かと思います。

	http://www.oreilly.co.jp/BOOK/be1/
	http://www.oreilly.co.jp/BOOK/be2/


/*****************************************************************************/
                                          Noritaka Suzuki <snori@alles.or.jp>  
                                                           http://ntk.yi.org/  
