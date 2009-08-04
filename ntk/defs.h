/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: defs.h,v 1.6 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_DEFS_H__
#define __NTK_DEFS_H__


#ifndef __NTK_CONFIG_H__
#include <ntk/config.h>
#endif


//
// debug or release
//
#ifdef _DEBUG
	#define NTK_DEBUG
#else
	#define NTK_RELEASE
#endif


//
// platform
//
#ifdef WIN32
	#define NTK_WINDOWS

	#undef NTK_WINNT
	#undef NTK_WIN32

	#if 0//defined(???)
		#define NTK_WINNT
		#define NTK_PLATFORM "WindowsNT"
	#elif defined(_WIN32)
		#define NTK_WIN32
		#define NTK_PLATFORM "Win32"
	#else
		#define NTK_PLATFORM "Windows"
	#endif
#else
	#define NTK_PLATFORM "Unknown"
#endif


//
// compiler (ここら変は適当に...)
//
#if defined(_MSC_VER)

	#define NTK_MICROSOFT_C
	#define NTK_COMPILER "Microsoft C/C++"

#elif defined(__BORLANDC__)// ←適当

	#define NTK_BORLAND_C
	#define NTK_COMPILER "Borland C/C++"

#elif defined(__GCC__)// ←適当

	#define NTK_GCC
	#define NTK_COMPILER "GCC"

#elif defined(__CYGWIN__)// ←適当

	#define NTK_CYGWIN_GCC
	#define NTK_COMPILER "Cygwin GCC"

#elif defined(__WATCOMC__)// ←適当

	#define NTK_WATCOM_C
	#define NTK_COMPILER "Watcom C/C++"

#endif


//
// correct configurations
//

#if defined(NTK_CHECK_MEMORY_LEAKS) && defined(NTK_RELEASE)
#undef NTK_CHECK_MEMORY_LEAKS
#endif

#if defined(NTK_USE_DEBUG_NEW) && !defined(NTK_CHECK_MEMORY_LEAKS)
#undef NTK_USE_DEBUG_NEW
#endif

#if defined(_UNICODE) && !defined(NTK_UNICODE)
#define NTK_UNICODE
#endif

#if defined(NTK_MICROSOFT_C) && defined(_LIB)
#define NTK_STATIC_LINK
#endif


//
// check compile options
//
#if defined(NTK_MICROSOFT_C) && !defined(_CPPRTTI)
	#ifdef NTK_LANG_JP
	#error エラー： NTK をコンパイルするためには、コンパイルオプションとして \
		"実行時型情報(RTTI)" が指定されていなければなりません。
	#else
	#error ERROR: NTK needs "Run Time Type Information(RTTI)" compiler option.
	#endif
#endif

#if defined(NTK_MICROSOFT_C) && (!defined(_MT) || !defined(_DLL))
	#ifdef NTK_LANG_JP
	#error エラー： NTKをコンパイルするためには、コンパイルオプションとして \
		"マルチスレッド, DLL(, デバッグ)" が指定されていなければなりません。
	#else
	#error NTK needs "Multithread, DLL(, Debug)" compiler option.
	#endif
#endif


//
// warnings
//
#ifndef NTK_ENABLE_ALL_WARNINGS
	#ifdef NTK_DISABLE_WARNING_4018
		// signed と unsigned を比較したときに出る警告
		#pragma warning(disable : 4018)
	#endif

	#ifdef NTK_DISABLE_WARNING_4786
		// 識別子が256文字以上になった時にでる警告
		#pragma warning(disable : 4786)
	#endif
#endif

//
// memory check
//
#if defined(NTK_CHECK_MEMORY_LEAKS) && defined(NTK_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <malloc.h>// 本来は必要ないが、#define malloc(...) malloc_dbg(...) 等で問題が出るので必要
	#include <memory>
	#include <new>
	#include <crtdbg.h>

	#define NTK_NEW ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

	#ifdef NTK_USE_DEBUG_NEW
		#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#endif
#else
	#define NTK_NEW new
#endif

//
// import/export definition
//
#if defined(NTK_STATIC_LINK) || defined(SWIG)
	#define NtkExport
#else
	#ifndef NTK_BUILD
		#define NtkExport __declspec(dllimport)
	#else
		#define NtkExport __declspec(dllexport)
	#endif
#endif


namespace ntk {


//--------------------------------------------------------
// basic types

//typedef char                    char;
typedef signed char             schar;
typedef unsigned char           uchar;
//typedef short                   short;
typedef signed short            sshort;
typedef unsigned short          ushort;
//typedef long                    long;
typedef signed long             slong;
typedef unsigned long           ulong;
//typedef int                     int;
typedef signed int              sint;
typedef unsigned int            uint;

typedef NTK_INT8_TYPE           int8;
typedef signed NTK_INT8_TYPE    sint8;
typedef unsigned NTK_INT8_TYPE  uint8;
typedef NTK_INT16_TYPE          int16;
typedef signed NTK_INT16_TYPE   sint16;
typedef unsigned NTK_INT16_TYPE uint16;
typedef NTK_INT32_TYPE          int32;
typedef signed NTK_INT32_TYPE   sint32;
typedef unsigned NTK_INT32_TYPE uint32;
typedef NTK_INT64_TYPE          int64;
typedef signed NTK_INT64_TYPE   sint64;
typedef unsigned NTK_INT64_TYPE uint64;

typedef unsigned NTK_BYTE_TYPE  byte;
typedef signed NTK_BYTE_TYPE    sbyte;
typedef unsigned NTK_BYTE_TYPE  ubyte;
typedef unsigned NTK_WORD_TYPE  word;
typedef signed NTK_WORD_TYPE    sword;
typedef unsigned NTK_WORD_TYPE  uword;
typedef unsigned NTK_DWORD_TYPE dword;
typedef signed NTK_DWORD_TYPE   sdword;
typedef unsigned NTK_DWORD_TYPE udword;
typedef unsigned NTK_QWORD_TYPE qword;
typedef signed NTK_QWORD_TYPE   sqword;
typedef unsigned NTK_QWORD_TYPE uqword;


//--------------------------------------------------------
// system data types


// coordinate value type //
typedef long coord;


// time value type //
typedef int64 bigtime_t;


// character type //
#ifndef NTK_UNICODE
typedef char char_t;
#else
typedef wchar_t char_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_USING_BASIC_TYPES
	using ntk::schar;
	using ntk::uchar;
	using ntk::sshort;
	using ntk::ushort;
	using ntk::slong;
	using ntk::ulong;
	using ntk::sint;
	using ntk::uint;

	using ntk::int8;
	using ntk::sint8;
	using ntk::uint8;
	using ntk::int16;
	using ntk::sint16;
	using ntk::uint16;
	using ntk::int32;
	using ntk::sint32;
	using ntk::uint32;
	using ntk::int64;
	using ntk::sint64;
	using ntk::uint64;

	typedef ntk::byte byte;
	using ntk::sbyte;
	using ntk::ubyte;
	using ntk::word;
	using ntk::sword;
	using ntk::uword;
	using ntk::dword;
	using ntk::sdword;
	using ntk::udword;
	using ntk::qword;
	using ntk::sqword;
	using ntk::uqword;

	using ntk::coord;
	using ntk::bigtime_t;
	using ntk::char_t;
#endif 


#endif//EOH
