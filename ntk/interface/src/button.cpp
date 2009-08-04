/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: button.cpp,v 1.5 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/button.h"


namespace ntk {


//########################################################
// public methods


Button::Button(const Rect& frame, const String& name, const String& label, Message* msg, uint mode, uint flags, const RGBColor& color)
:	SystemControl("BUTTON", frame, label, label, msg, mode, flags, color)
{
}

void
Button::set_label(const String& label)
{
	SystemControl::set_label(label);
	SetWindowText(hwnd(), label.c_str());
}


//########################################################
}// namespace ntk
