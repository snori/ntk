/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: debug.h,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_DEBUG_H__
#define __NTK_SUPPORT_DEBUG_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif


namespace ntk {


//
// debug utilities
//

typedef void (*ErrorFunc)(const char_t* str);
NtkExport void error(const char_t* str);
NtkExport void error(const String& message);
NtkExport void set_error_func(ErrorFunc func = NULL);


NtkExport void abort(const char_t* str = NULL);
NtkExport void abort(const String& message);


NtkExport String last_error_message(dword error_no = 0);// 0: GetLastError()


//
// logger
//
#if 0
// データロガー
class Logger : public ofstream {
	public:
		//
		// methods
		//
		Logger(const String& filename);

	private:
		String m_filename;

};// class Logger
#endif


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ErrorFunc error_func_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::ErrorFunc NErrorFunc;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::ErrorFunc ntk_error_func;
	#endif
#endif


#endif//EOH
