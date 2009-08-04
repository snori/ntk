/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: debug.cpp,v 1.5 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/debug.h"


#include <ntk/windows/windows.h>
#include <ntk/support/utility.h>


namespace ntk {


//########################################################
// debug utilities

static void
error_func(const char_t* str)
{
#ifdef NTK_DEBUG
	OutputDebugString(str);
#else
	message_box("ÉGÉâÅ[", str, MB_OK | MB_ICONEXCLAMATION);
#endif
}

static ErrorFunc g_error_func = error_func;

void
error(const char_t* str)
{
	g_error_func(str);
}

void
error(const String& message)
{
	error(message.c_str());
}

#if 0
void
error(const char_t* format, ...)
{
	enum{ BUF_SIZE = 1024 };
	char_t buf[BUF_SIZE];

	va_list args;
	va_start(args, format);
	n_vsprintf(buf, format, args);
	va_end(args);

	error(buf);
}
#endif

void
set_error_func(ErrorFunc func)
{
	if(func)
		g_error_func = func;
	else
		g_error_func = error_func;
}

void
abort(const char_t* str)
{
	if(str)
		error(str);

	::abort();
}

void
abort(const String& message)
{
	abort(message.c_str());
}

#if 0
void
abort(const char_t* format, ...)
{
	if(format == NULL) return;

	enum{ BUF_SIZE = 1024 };
	char_t buf[BUF_SIZE];

	va_list args;
	va_start(args, format);
	n_vsprintf(buf, format, args);
	va_end(args);

	abort(buf);
}
#endif

String
last_error_message(dword error_no)
{
	error_no = error_no == 0 ? GetLastError() : error_no;
	void* buf;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error_no, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&buf, 0, NULL);

	String str(reinterpret_cast<const char_t*>(buf));

	LocalFree(buf);

	return str;
}


//########################################################
}// namespace ntk
