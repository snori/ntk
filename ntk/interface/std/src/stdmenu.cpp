/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stdmenu.cpp,v 1.4 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/std/stdmenu.h"


#include <algorithm>
#include <ntk/windows/windows.h>
#include <ntk/interface/window.h>
#include <ntk/interface/std/menuitem.h>


namespace ntk {
namespace standard {


//########################################################
// public methods


MenuOperator::MenuOperator(HMENU hmenu)
:	m_handle(hmenu)
{
}

MenuOperator::~MenuOperator()
{
}

bool
MenuOperator::add_item(const Menu& submenu)
{
	MENUITEMINFO menu_item_info;
	menu_item_info.cbSize			= sizeof(MENUITEMINFO);

	menu_item_info.fMask			= MIIM_SUBMENU | MIIM_TYPE;
	menu_item_info.fType			= MFT_STRING;

	menu_item_info.hSubMenu		= submenu.handle();
	menu_item_info.dwTypeData	= const_cast<char*>(submenu.label().c_str());
	menu_item_info.cch				= submenu.label().length();

	InsertMenuItem(handle(), 0xffff, TRUE, &menu_item_info);

	return true;
}

bool
MenuOperator::add_item(const MenuItem& item)
{
	MENUITEMINFO menu_item_info;
	menu_item_info.cbSize			= sizeof(MENUITEMINFO);

	menu_item_info.fMask			= MIIM_ID | MIIM_TYPE;
	menu_item_info.fType			= MFT_STRING;

	menu_item_info.wID				= item.id();
	menu_item_info.dwTypeData	= const_cast<char*>(item.label().c_str());
	menu_item_info.cch				= item.label().length();

	InsertMenuItem(handle(), 0xffff, TRUE, &menu_item_info);

	return true;
}

//--------------------------------------------------------
// public accessors and manipulators

HMENU
MenuOperator::handle() const
{
	return m_handle;
}


//########################################################
// public methods


Menu::Menu(const String& label)
:	MenuOperator(CreateMenu())
,	m_label(label)
{
}

Menu::~Menu()
{
}

const String&
Menu::label() const
{
	return m_label;
}


//########################################################
}// namespace standard
}// namespace std
