/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: screen.h,v 1.5 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_SCREEN_H__
#define __NTK_INTERFACE_SCREEN_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_RECT_H__
#include <ntk/interface/rect.h>
#endif


namespace ntk {


class Screen {
public:
	//
	// methods
	//
	NtkExport Screen();
	NtkExport ~Screen();

	//
	// accessors
	//
	NtkExport static Rect frame();

};// class Screen


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Screen screen_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Screen NScreen;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Screen ntk_screen;
	#endif
#endif


#endif//EOH
