/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dialogpanel.cpp,v 1.4 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/dialogpanel.h"


#include <ntk/application/application.h>
#include <ntk/application/messenger.h>
#include <ntk/interface/window.h>


namespace ntk {


//########################################################


namespace {


static const char_t* PROP_NAME = "DialogPanel::PARENT";


BOOL CALLBACK
dialog_panel_proc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp)
{
	switch(message)
	{
	case WM_INITDIALOG:
		SetProp(hwnd, PROP_NAME, reinterpret_cast<void*>(lp));
		break;

	case WM_COMMAND:
		{
			DialogPanel* p = reinterpret_cast<DialogPanel*>(GetProp(hwnd, PROP_NAME));
			Messenger(NULL, p->window()).send_message(Message(message, wp, lp));
		}
		break;

	default:
		return FALSE;
	}

	return FALSE;
}

}// anonymous namespace


//########################################################
// public methods

DialogPanel::DialogPanel(const char_t* resource_id, const Rect& frame, const String& name, uint mode, uint flags, const RGBColor& color)
:	Panel(frame, name, mode, flags, color)
,	m_dialog_handle(NULL)
{
	m_dialog_handle = CreateDialogParam(
		ntk::application()->handle(), resource_id, hwnd(),
		dialog_panel_proc, reinterpret_cast<LPARAM>(this));
	ShowWindow(m_dialog_handle, SW_SHOW);

	if(flags & RESIZE_TO_FIT)
	{
		Rect rect;
		GetWindowRect(m_dialog_handle, &rect);

		resize_to(rect.width(), rect.height());
	}
}


//########################################################
}// namespace ntk
