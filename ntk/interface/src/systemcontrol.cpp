/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: systemcontrol.cpp,v 1.1 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/systemcontrol.h"


namespace ntk {


//########################################################
// public methods

SystemControl::SystemControl(const String& class_name, const Rect& frame, const String& name, const String& label, Message* message, uint mode, uint flags, const RGBColor& color, uint style, uint ex_style)
:	Control(class_name, frame, name, label, message, mode, flags, color, style, ex_style)
{
}

SystemControl::~SystemControl()
{
}


//########################################################
}// namespace ntk
