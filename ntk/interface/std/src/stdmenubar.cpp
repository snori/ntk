/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stdmenubar.cpp,v 1.8 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/std/stdmenubar.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/window.h>


namespace ntk {
namespace standard {


//########################################################


MenuBar::MenuBar(HMENU hmenu, const Rect& frame_, const String& name, uint mode, uint flags, const RGBColor& color, uint style, uint ex_style)
:	Super(window_class(), frame_, name, mode, flags, color, style, ex_style)
,	MenuOperator(hmenu)
{
	SetMenu(hwnd(), handle());
}

MenuBar::~MenuBar()
{
}

//--------------------------------------------------------
// public message handlers

void
MenuBar::attached_to_window()
{
	if(flags() & RESIZE_TO_FIT)
	{
		Rect rect;
		if(parent())
			rect = parent()->bounds();
		else if(window())
			rect = window()->bounds();

		move_to(rect.left_top());
		resize_to(rect.width(), GetSystemMetrics(SM_CYMENU) -1);// -1: ‚Ç‚¤‚àˆê–{ü‚ª“ü‚é‚æ‚¤‚È‚Ì‚Å
	}
}

void
MenuBar::detached_from_window()
{
}

LRESULT
MenuBar::system_message_received(const Message& message)
{
	switch(message.what)
	{
	case WM_COMMAND:
		window()->post_message(message);
		break;

	case WM_NCACTIVATE:
		{
//			if(message.wparam() != FALSE)
//				return FALSE;

//			Message msg(message.what, message.wparam() | (TRUE & 0xffff), message.lparam());
			return View::system_message_received(message);
		}
		break;

	default:
		return View::system_message_received(message);// ntk::Menu ‚ÌŽÀ‘•‚ð–³Œø‚É
	}

	return 0;
}


//########################################################
}// namespace standard
}// namespace ntk
