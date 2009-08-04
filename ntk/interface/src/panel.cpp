/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: panel.cpp,v 1.5 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/panel.h"


#include <ntk/interface/window.h>
#include <ntk/interface/menubar.h>


namespace ntk {


//########################################################
// public methods

Panel::Panel(const Rect& frame, const String& name, uint mode, uint flags, const RGBColor& color, uint style, uint ex_style, HMENU hmenu)
:	View(frame, name, mode, flags, color, style, ex_style, hmenu)
,	m_key_menu_bar(NULL)
{
}

Panel::~Panel()
{
}

status_t
Panel::add_child(View* child)
{
	status_t sts = View::add_child(child);

	if(sts.is_valid_without_eval() == false)
		return sts;

	MenuBar* menu_bar = dynamic_cast<MenuBar*>(child);
	if(menu_bar)
		set_key_menu_bar(menu_bar);

	return true;
}

status_t
Panel::remove_child(View* child)
{
	if(child == m_key_menu_bar)
		set_key_menu_bar(NULL);

	return Wnd::remove_child(child);
}

status_t
Panel::add_child(Window* window)
{
	if(window == NULL)
		return st::BAD_VALUE_ERROR;

	window->set_ex_style(window->ex_style() | WS_EX_MDICHILD);
	window->m_parent = this;

	HWND ret = SetParent(window->hwnd(), hwnd());
	if(ret == NULL)
		return st::SYSTEM_ERROR;

	return true;
}

status_t
Panel::remove_child(Window* window)
{
	if(window == NULL)
		return st::BAD_VALUE_ERROR;
	if(window->m_parent == NULL || window->m_parent != this)
		return status_t(st::ERR,
			"Panel::remove_child(Window*): このウィンドウはこのパネルに所属していません\n");

	HWND ret = SetParent(window->hwnd(), NULL);
	if(ret == NULL)
		return st::SYSTEM_ERROR;

	window->m_parent = NULL;
	window->set_ex_style(window->ex_style() & ~WS_EX_MDICHILD);

	return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

MenuBar*
Panel::key_menu_bar() const
{
	return m_key_menu_bar;
}

void
Panel::set_key_menu_bar(MenuBar* menu_bar)
{
	m_key_menu_bar = menu_bar;

	if(m_key_menu_bar)
	{
		m_key_menu_bar->move_to(0, 0);
		m_key_menu_bar->resize_to(frame().width(), m_key_menu_bar->frame().height());
	}
}


//########################################################
}// namespace ntk
