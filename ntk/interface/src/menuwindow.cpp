/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menuwindow.cpp,v 1.7 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "menuwindow.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/menu.h>
#include <ntk/interface/panel.h>
#include <ntk/interface/utility.h>


namespace ntk {


//########################################################


namespace {

static const coord MENU_WINDOW_FRAME_WIDTH = 2;

}// anonymous namespace


class MenuWindowPanel : public Panel {
public:
	MenuWindowPanel()
	:	Panel(Rect(0, 0, 10, 10), "panel", FOLLOW_ALL_SIDES, DEFAULT_FLAGS | WILL_DRAW)
	{}
	virtual ~MenuWindowPanel()
	{}

	void draw(PaintDC& dc)
	{
		Rect rect = bounds();
		stroke_edge(dc, rect, Pen(view_color() + 15), Pen(view_color() - 15));

		rect.inset_by(1, 1);
		stroke_edge(dc, rect, Pen(view_color() + 5), Pen(view_color() - 5));
	}
};


//########################################################
// public methods

MenuWindow::MenuWindow(Menu* menu_, bool owner)
:	Window(new MenuWindowPanel(), Rect(), "menu", BORDERED_LOOK, FLOATING_APP_FEEL, 0, 0, WS_EX_MDICHILD)
,	m_menu(menu_)
,	m_owner(owner)
{
	if(m_menu)
	{
		add_child(m_menu);
		m_menu->move_to(MENU_WINDOW_FRAME_WIDTH, MENU_WINDOW_FRAME_WIDTH);

		Rect rect = m_menu->frame();
		rect.inset_by(-MENU_WINDOW_FRAME_WIDTH, -MENU_WINDOW_FRAME_WIDTH);
		resize_to(rect.width(), rect.height());
	}
}

MenuWindow::~MenuWindow()
{
}

void
MenuWindow::go(const Point& point)
{
	move_to(point);
	show();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// message handlers

bool
MenuWindow::quit_requested()
{
	if(m_menu->m_opened_submenu)
		m_menu->m_opened_submenu->window()->quit();

	if(! m_owner)
		remove_child(m_menu);

	return true;
}


//########################################################
}// namespace ntk
