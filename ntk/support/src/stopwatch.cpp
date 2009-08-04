/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stopwatch.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/stopwatch.h"


#include <ntk/windows/windows.h>
#include <ntk/support/debug.h>
#include <ntk/support/utility.h>


namespace ntk {


//########################################################
// public methods

StopWatch::StopWatch(const String& name)
:	m_name(name),
	m_start_time(current_time())
{
}

StopWatch::~StopWatch()
{
	String tmp = "";

	if(name() != String())
		tmp = name() + ": ";

	print(tmp + format_string("%d", current_time() - start_time()) + "ms\n");
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

const String&
StopWatch::name() const
{
	return m_name;
}

void
StopWatch::set_name(const String& name_)
{
	m_name = name_;
}

dword
StopWatch::start_time() const
{
	return m_start_time;
}

dword
StopWatch::current_time() const
{
	return GetTickCount();//timeGetTime();
}


//########################################################
}// namespace ntk
