/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: supportutility.h,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_SUPPORTUTILITY_H__
#define __NTK_SUPPORT_SUPPORTUTILITY_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_DEBUG_H__
#include <ntk/support/debug.h>
#endif


#define NTK_BIT(n) (1<<n)


namespace ntk {


//
// utility functions
//

// random //
NtkExport void init_random(int seed = -1);// -1: 時間で初期化
NtkExport float random();


// print/message_box //
NtkExport void print(const char_t* str);
NtkExport void print(const String& message);
NtkExport int message_box(
	const String& title, const String& message, uint flags = 0);// 0 == MB_OK


template<typename T>
inline const T&
bit(const T& n)
{
	assert(0 <= n && n < NTK_NUM_BITS_OF_FLAG);
	return 1 << n;
}

template<typename T>
inline const T&
maximum(const T& a, const T& b)
{
	return a > b ? a : b;
}

template<typename T>
inline const T&
minimum(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
inline const T&
minmax(const T& val, const T& min_val, const T& max_val)
{
	return val < min_val ? min_val : (val > max_val ? max_val : val);
}

template<typename REAL, typename INTEGER>
inline INTEGER
round(const REAL& val)
{
	return (INTEGER)(val + 0.5);
}

template<typename T>
inline void
swap(T& a, T& b)
{
	T tmp = a;
	a = b;
	b = tmp;
}

//
// bool/BOOL
//
inline bool
to_bool(int val)
{
	return val != 0;
}

template<typename T>
inline int
to_BOOL(const T& val)
{
	return val ? 1 : 0;
}

//
// endian convertion utilities
//
template<typename T>
T
convert_endian(T src)
{
	error("エンディアン変換ルーチンがありません。");
	return 0;
}

inline int
convert_endian<int>(int src)
{
	uint& tmp = *(uint*)&src;

	return
		(tmp & 0xff000000) >> 24 |
		(tmp & 0x00ff0000) >> 8 |
		(tmp & 0x0000ff00) << 8 |
		(tmp & 0x000000ff) << 24;
}

inline int16
convert_endian<int16>(int16 src)
{
	uint16& tmp = *(uint16*)&src;

	return
		(tmp & 0xff00) >> 8 |
		(tmp & 0x00ff) << 8;
}

inline uint16
convert_endian<uint16>(uint16 src)
{
	return
		(src & (uint16)0xff00) >> 8 |
		(src & (uint16)0x00ff) << 8;
}

inline int32
convert_endian<int32>(int32 src)
{
	uint32& tmp = *(uint32*)&src;

	return
		(tmp & 0xff000000) >> 24 |
		(tmp & 0x00ff0000) >> 8 |
		(tmp & 0x0000ff00) << 8 |
		(tmp & 0x000000ff) << 24;
}

inline uint32
convert_endian<uint32>(uint32 src)
{
	return
		(src & 0xff000000) >> 24 |
		(src & 0x00ff0000) >> 8 |
		(src & 0x0000ff00) << 8 |
		(src & 0x000000ff) << 24;
}

inline float
convert_endian<float>(float src)
{
	uint32& tmp = *(uint32*)&src;

	tmp =
		(tmp & 0xff000000) >> 24 |
		(tmp & 0x00ff0000) >> 8 |
		(tmp & 0x0000ff00) << 8 |
		(tmp & 0x000000ff) << 24;

	return src;
}

inline double
convert_endian<double>(double src)
{
	uint64& tmp = *(uint64*)&src;

	tmp =
		tmp >> 56 |
		(tmp & 0x00ff000000000000) >> 40 |
		(tmp & 0x0000ff0000000000) >> 24 |
		(tmp & 0x000000ff00000000) >> 8 |
		(tmp & 0x00000000ff000000) << 8 |
		(tmp & 0x0000000000ff0000) << 24 |
		(tmp & 0x000000000000ff00) << 40 |
		tmp << 56;

	return src;
}


//--------------------------------------------------------
// wide/multi-byte character conversion


NtkExport WString mb2wc(const char* str);
NtkExport WString mb2wc(const AString& str);
NtkExport AString wc2mb(const wchar_t* str);
NtkExport AString wc2mb(const WString& str);


} namespace Ntk = ntk;


#endif//EOH
