/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: screen.cpp,v 1.4 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/screen.h"


#include <ntk/windows/windows.h>


namespace ntk {


//########################################################
// public methods

Screen::Screen()
{
}

Screen::~Screen()
{
}

Rect
Screen::frame()
{
	Rect screen_frame(
		0,
		0,
		GetSystemMetrics(SM_CXSCREEN),
		GetSystemMetrics(SM_CYSCREEN));

	return screen_frame;
}


//########################################################
}// namespace ntk
