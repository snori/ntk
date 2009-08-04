/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menubar.cpp,v 1.5 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/menubar.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/window.h>


namespace ntk {


//########################################################


const Menu::MenuLayout MenuBar::DEFAULT_LAYOUT = Menu::ITEMS_IN_ROW;


//########################################################
// public methods

MenuBar::MenuBar(const Rect& frame, const String& name, uint mode, uint flags, MenuLayout layout, const RGBColor& color)
:	Menu(frame, name, mode, flags, layout, color)
{
	resize_to(
		frame.width(),
		DC(hwnd()).string_height() + 6);
}

MenuBar::~MenuBar()
{
}


//########################################################
}// namespace ntk
