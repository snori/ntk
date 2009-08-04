/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: windows.h,v 1.2 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STD_WINDOWS_H__
#define __NTK_STD_WINDOWS_H__


#ifndef __NTK_CONFIG_H__
#include <ntk/config.h>
#endif

#if defined(NTK_DEFINE_LnM) || defined(NTK_DEFINE_WINVER)
	#ifdef NTK_LANG_JP
	#error 有り得ないエラーです。
	#else
	#error It is the error which is not possible.
	#endif
#endif


//------------------


#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
	#define NTK_DEFINE_LnM
#endif

#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400
	#define NTK_DEFINE_WINVER
#endif


#include <windows.h>


#ifdef NTK_DEFINE_LnM
	#undef WIN32_LEAN_AND_MEAN
#endif

#ifdef NTK_DEFINE_WINVER
	#undef _WIN32_WINNT
#endif


#endif//EOH
