/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stdmenuitem.cpp,v 1.4 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/std/stdmenuitem.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/std/menu.h>


namespace ntk {
namespace standard {


//########################################################
// public methods

MenuItem::MenuItem(const String& label, uint id, uint shortcut, uint modifiers)
:	Handler(label)
//,	Invoker(NULL, NULL)
,	m_id(id)
{
}

MenuItem::~MenuItem()
{
}

//********************************************************
// public accessors

String
MenuItem::label() const
{
	return name();
}

uint
MenuItem::id() const
{
	return m_id;
}


//########################################################
}// namespace standard
}// namespace ntk
