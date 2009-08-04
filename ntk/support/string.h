/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: string.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_STRING_H__
#define __NTK_SUPPORT_STRING_H__


#include <string>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


typedef std::string AString;
typedef std::wstring WString;

#ifdef NTK_UNICODE
typedef WString String;
#else
typedef AString String;
#endif


// utility functions //
NtkExport String format_string(const char_t* format, ...);


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef String  string_t;
	typedef AString astring_t;
	typedef WString wstring_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::String  NString;
	typedef ntk::AString NAString;
	typedef ntk::WString NWString;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::String  ntk_string;
	typedef ntk::AString ntk_astring;
	typedef ntk::WString ntk_wstring;
	#endif
#endif


#endif//EOH
