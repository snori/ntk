/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menubar.h,v 1.6 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_MENUBAR_H__
#define __NTK_INTERFACE_MENUBAR_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_MENU_H__
#include <ntk/interface/menu.h>
#endif


namespace ntk {


class MenuBar : public Menu {
public:
	//
	// constants
	//
	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_LEFT_RIGHT | FOLLOW_TOP,
	};

	NtkExport static const MenuLayout DEFAULT_LAYOUT;

	//
	// methods
	//
	NtkExport MenuBar(
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		MenuLayout layout     = DEFAULT_LAYOUT,
		const RGBColor& color = app_color());
	NtkExport virtual ~MenuBar();

//	NtkExport virtual void resize_to(coord width, coord height);

};// class MenuBar


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MenuBar menu_bar_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::MenuBar NMenuBar;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::MenuBar ntk_menu_bar;
	#endif
#endif


#endif//EOH
