/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: config.h,v 1.9 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_CONFIG_H__
#define __NTK_CONFIG_H__


//--------------------------------------------------------
// global configurations


#define NTK_LANG_JP
/*
 * 言語別のコード部分がある場合、日本語を選択します。
 */


//#define NTK_STATIC_LINK
/*
 * このマクロを定義するとエクスポート用マクロ (NtkExport 等)
 * を無効化します。
 */


//#define NTK_UNICODE
/*
 * このマクロが定義されていると以下点が変更されます
 * ・string : std::basic_string<char> --> std::basic_string<wchar_t>
 * ・char_t :                    char --> wchar_t
 * ・その他もろもろ
 */


//#define NTK_DISABLE_WARNING_4018 // signed と unsigned を比較したときに出る警告
#define NTK_DISABLE_WARNING_4786 // 識別子が256文字以上になった時にでる警告
/*
 * （冗長な）警告を、無効にします。
 */


//#define NTK_ENABLE_ALL_WARNINGS
/*
 * このマクロを定義すると、上の個別の警告無効化を無視しすべて有効にします。
 */


//#define NTK_CHECK_MEMORY_LEAKS
/*
 * 実装ファイルの先頭部分で new の代わりに NTK_NEW を使用することでメモリリークの
 * 検出機構を利用できるようになります。
 */


//#define NTK_USE_DEBUG_NEW
/*
 * このマクロが定義されていると、
 *
 *	#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
 *
 * が定義され、標準の new でも上のメモリリーク検出機構が働くようになります。
 */


#define NTK_USING_BASIC_TYPES
/*
 * int32 等の基本型をグローバル名前空間に using する。他のライブラリと被る等、
 * 問題がある場合はコメントアウトしてください。using しないようになります。
 */


//#define NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
/*
 * したの2項目と組み合わせ、NTK の公開されている型の識別子名をグローバルに
 * typedef します。
 */


#define NTK_TYPEDEF_GLOBAL_NCLASS
/*
 * ::ntk::Class -> ::NClass の typedef (template class に限り #define)
 * を有効にします。
 * （注） この項目は先の NTK_TYPEDEF_TYPES_AS_GLOBAL が有効な時のみ有効になります
 */


#define NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
/*
 * ::ntk::Class -> ::ntk_class の typedef (template class に限り #define)
 * を有効にします。
 * （注） この指定は先の NTK_TYPEDEF_TYPES_AS_GLOBAL が有効な時のみ有効になります
 *
 * またトップレベルスコープのクラス中にある公開されている型も
 * ::ntk::DataIO::Stat -> ::ntk::DataIO::stat_t のように (...)_t として
 * typedef されます。
 * （注） これは NTK_TYPEDEF_TYPES_AS_GLOBAL とは関係なく有効です。
 */


#define NTK_DEF_MAIN
/*
 * このマクロが定義されていると、
 *
 * 	#define main ntk_main
 *
 * が定義されます。
 */


//#define NTK_WINDOW_OWN_THREAD
/*
 * *OBSOLETE*
 * この設定を有効にすると、Window クラスがそれぞれスレッドを持つようになります。
 * 現在は、この手法とウィンドウズOSとの相性が悪いことがわかったため、インプリメ
 * ントされていません。
 */


#define NTK_NUM_BITS_OF_FLAG 32
/*
 * 整数型をフラグとして利用する時のビット数
 */


#define NTK_ENABLE_SCINTILLA
/*
 * この設定が有効の場合 ntk::Scintilla コントロールが使用可能になります。
 * その場合、ヘッダパスの設定と、Scintilla.dll/SciLexer.dll が利用可能になって
 * いる必要があります。
 */


//--------------------------------------------------------
// basic type configurations


#ifndef NTK_INT8_TYPE
#define NTK_INT8_TYPE char
//#define NTK_INT8_TYPE _int8
#endif

#ifndef NTK_INT16_TYPE
#define NTK_INT16_TYPE short
//#define NTK_INT16_TYPE _int16
#endif

#ifndef NTK_INT32_TYPE
#define NTK_INT32_TYPE long
//#define NTK_INT32_TYPE _int32
#endif

#ifndef NTK_INT64_TYPE
//#define NTK_INT64_TYPE long long
#define NTK_INT64_TYPE _int64
#endif

#ifndef NTK_BYTE_TYPE
#define NTK_BYTE_TYPE char
#endif

#ifndef NTK_WORD_TYPE
#define NTK_WORD_TYPE short
#endif

#ifndef NTK_DWORD_TYPE
#define NTK_DWORD_TYPE long
#endif

#ifndef NTK_QWORD_TYPE
#define NTK_QWORD_TYPE _int64
#endif


#endif//EOH
