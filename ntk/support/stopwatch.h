/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stopwatch.h,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_STOPWATCH_H__
#define __NTK_SUPPORT_STOPWATCH_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif


namespace ntk {


class StopWatch {
public:
	//
	// methods
	//
	NtkExport StopWatch(const String& name = String());
	NtkExport ~StopWatch();

	//
	// accessors
	//
	NtkExport const String& name() const;
	NtkExport void set_name(const String& name);

	NtkExport dword start_time() const;
	NtkExport dword current_time() const;

private:
	//
	// data
	//
	String m_name;
	dword m_start_time;

};// class StopWatch


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef StopWatch stop_watch_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::StopWatch NStopWatch;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::StopWatch ntk_stop_watch;
	#endif
#endif


#endif//EOH
