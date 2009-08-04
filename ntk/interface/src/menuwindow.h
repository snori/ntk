/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menuwindow.h,v 1.5 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_MENUWINDOW_H__
#define __NTK_INTERFACE_MENUWINDOW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_WINDOW_H__
#include <ntk/interface/window.h>
#endif


namespace ntk {


class Menu;


class MenuWindow : public Window {
public:
	//
	// methods
	//
	MenuWindow(Menu* menu, bool owner = false);
	virtual ~MenuWindow();

	void go(const Point& point);

	//
	// message handler
	//
	bool quit_requested();

private:
	//
	// data
	//
	Menu* m_menu;
	bool m_owner;

};// class MenuWindow


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MenuWindow menu_window_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::MenuWindow NMenuWindow;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::MenuWindow ntk_menu_window;
	#endif
#endif


#endif//EOH
