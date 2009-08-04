/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menuitem.cpp,v 1.4 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/menuitem.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/dc.h>
#include <ntk/interface/menu.h>
#include <ntk/interface/utility.h>
#include "menuwindow.h"


namespace ntk {


//########################################################
// public methods

MenuItem::MenuItem(const String& label_, Message* message, char_t shortcut_, uint modifiers_)
:	Invoker(message, NULL, NULL),
	Archivable(),
	m_menu(NULL),
	m_label(label_),
	m_submenu(NULL),
	m_selected(false)
{
	set_shortcut(shortcut_);
	set_modifiers(modifiers_);
}

MenuItem::MenuItem(Menu* submenu_, Message* message, char_t shortcut_, uint modifiers_)
:	Invoker(message, NULL, NULL),
	Archivable(),
	m_menu(NULL),
	m_submenu(NULL),
	m_selected(false)
{
	set_shortcut(shortcut_);
	set_modifiers(modifiers_);

	m_submenu = submenu_;
	m_label = m_submenu ? m_submenu->name() : "no name";
}

MenuItem::~MenuItem()
{
	delete m_submenu;
}

void
MenuItem::invalidate()
{
	if(menu())
		menu()->invalidate_rect(frame(), false);// ?
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

Menu*
MenuItem::menu() const
{
	return m_menu;
}

Rect
MenuItem::frame() const
{
	return m_frame;
}

String
MenuItem::label() const
{
	return m_label;
}

void
MenuItem::set_label(const String& label_)
{
	m_label = label_;

	invalidate();
}

char_t
MenuItem::shortcut() const
{
	return m_shortcut;
}

void
MenuItem::set_shortcut(char_t shortcut_)
{
	m_shortcut = shortcut_;

	invalidate();
}

uint
MenuItem::modifiers() const
{
	return m_modifiers;
}

void
MenuItem::set_modifiers(uint modifiers_)
{
	m_modifiers = modifiers_;

	invalidate();
}

Menu*
MenuItem::submenu() const
{
	return m_submenu;
}

bool
MenuItem::is_selected() const
{
	return m_selected;
}

status_t
MenuItem::set_target(Handler* target_, Looper* looper_)
{
	// サブメニュー用の NMenuItem ならば新しいターゲットを子のメニューにも設定する
	if(submenu())
	{
		submenu()->set_target(target_, looper_);
	}

	if(target() == NULL)// item 個別のターゲット設定がなければ(あればそれを優先)
	{
		return Invoker::set_target(target_, looper_);
	}
	else return st::OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
MenuItem::attached_to_window()
{
}

void
MenuItem::detached_from_window()
{
}

void
MenuItem::draw(DC& dc)
{
	draw_content(dc);
	highlight(dc, m_selected);
}

void
MenuItem::draw_content(DC& dc)
{
	Brush brush;
	if(is_selected())
		brush = Brush(menu()->view_color() - 50);
	else
		brush = Brush(menu()->view_color());

	dc.set_brush(brush);
	dc.fill_rect(frame());

	Rect rect = frame();
	coord string_height = dc.string_height();

/*	if(is_selected())
		dc.set_text_color(font_color());
	else
		dc.set_text_color(font_color());
*/

	dc.draw_string(m_label, Point(rect.left + H_SPACE, rect.top + (rect.height() - string_height) /2));
}

void
MenuItem::highlight(DC& dc, bool state)
{
	dc.set_drawing_mode(DC::OP_INVERT);
	dc.set_brush(Brush(RGBColor(255, 255, 110)));

	if(state)
		dc.fill_rect(frame());
//	draw_edge();
}

void
MenuItem::get_content_size(coord* width, coord* height) const
{
	DC& dc = menu()->dc();
	*width = dc.string_width(m_label) + H_SPACE *2;
	*height = dc.string_height() + V_SPACE *2;
}


//########################################################
// public methods

SeparatorItem::SeparatorItem()
:	MenuItem("Separator", NULL)
{
}

SeparatorItem::~SeparatorItem()
{
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
SeparatorItem::draw_content(DC& dc)
{
	Rect rect = frame();

	dc.set_brush(Brush(menu()->view_color()));
	dc.fill_rect(rect);

	coord center = (rect.top + rect.bottom) /2 -1;

	dc.set_pen(Pen(menu()->view_color() - dark_edge_color_difference()));
	dc.stroke_line(rect.left, center, rect.right, center);

	dc.set_pen(Pen(menu()->view_color() + light_edge_color_difference()));
	dc.stroke_line(rect.left, center +1, rect.right, center +1);
}

void
SeparatorItem::highlight(DC& dc, bool state)
{
}

void
SeparatorItem::get_content_size(coord* width, coord* height) const
{
	DC& dc = menu()->dc();
	*width = 1;//dc.string_width(m_label) + H_SPACE *2;
	*height = 10;//dc.string_height() + V_SPACE *2;
}


//########################################################
}// namespace ntk
