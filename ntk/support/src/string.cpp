/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: string.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/string.h"


#include <stdio.h>
#include <stdarg.h>
#include <tchar.h>
#include <boost/scoped_array.hpp>


namespace ntk {


//########################################################
// functions

String
format_string(const char_t* format, ...)
{
	boost::scoped_array<char_t> buf(new char_t[2048]);

	va_list args;
	va_start(args, format);
	_vstprintf(&buf[0], format, args);
	va_end(args);

	return &buf[0];
}


//########################################################
}// namespace ntk
