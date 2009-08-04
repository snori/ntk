/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: tabbar.cpp,v 1.6 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/tabbar.h"


#include <algorithm>
#include <ntk/interface/utility.h>


namespace ntk {


//########################################################
// public methods

TabBar::TabBar(const Rect& frame, const String& name, Message* message, uint mode, uint flags, const RGBColor& color)
:	Control(frame, name, "", message, mode, flags, color)
{
}

TabBar::~TabBar()
{
}

bool
TabBar::add_tab(const String& label, int index)
{
	for(TabList::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		if(it->name() == label)
			break;

	if(it != m_tabs.end())
		return false;// 2d“o˜^‚Í•s‰Â

	Tab tab(Rect(0, 0, 100, frame().height()), label);

	if(index >= 0)
	{
		TabList::iterator it = m_tabs.begin();
		::std::advance(it, index);

		m_tabs.insert(it, tab);
	}
	else
		m_tabs.push_back(tab);

	if(m_tabs.size() == 1)
		activate_tab(0);

	reposition_();
	invalidate();

	return true;
}

bool
TabBar::remove_tab(const String& label)
{
	for(TabList::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		if(it->name() == label)
			break;

	if(it == m_tabs.end())
		return false;

	bool was_active = it->is_active();

	m_tabs.erase(it);

	if(was_active && m_tabs.size() >= 1)
		activate_tab(0);

	reposition_();
	invalidate();

	return true;
}

bool
TabBar::remove_tab(uint index)
{
	if(index >= m_tabs.size())
		return false;

	TabList::iterator it = m_tabs.begin();
	::std::advance(it, index);

	m_tabs.erase(it);

	reposition_();
	invalidate();

	return true;
}

const String&
TabBar::active_tab() const
{
	for(TabList::const_iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		if(it->is_active())
			return it->name();

	static const String FALSE_STRING = "";
	return FALSE_STRING;
}

void
TabBar::activate_tab(const String& name)
{
	for(TabList::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		if(it->name() == name)
		{
			activate_tab(std::distance(m_tabs.begin(), it));
			return;
		}
}

void
TabBar::activate_tab(uint index)
{
	TabList::iterator it = m_tabs.begin();
	std::advance(it, index);

	for(TabList::iterator jt = m_tabs.begin(); jt != m_tabs.end(); ++jt)
		jt->m_active = false;

	it->m_active = true;
	invalidate();

	Message msg = message() ? *message() : TAB_CHANGED;
	msg.add_int("tab-index", index);
	msg.add_string("tab-name", it->name());

	invoke(msg);
}

void
TabBar::mouse_down(const Point& point, uint buttons, bool dbl)
{
	for(TabList::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		if(it->frame().contains(point))
		{
			activate_tab(std::distance(m_tabs.begin(), it));
			return;
		}
}

void
TabBar::mouse_up(const Point& point, uint buttons)
{
}

void
TabBar::mouse_moved(const Point& point, uint buttons, uint transit, const Message* data)
{
}

void
TabBar::draw(PaintDC& dc)
{
	Rect rect = bounds();
	rect.bottom--;

	dc.push_pen();
	dc.set_pen(ntk::outline_color());
	dc.stroke_line(rect.left_bottom(), rect.right_bottom());
	dc.pop_pen();

	for(TabList::iterator it = m_tabs.begin(); it != m_tabs.end(); ++it)
		it->draw(dc);
}

//********************************************************
// functions

void
TabBar::reposition_()
{
	coord height = frame().height();

	for(int i=0; i<m_tabs.size(); ++i)
	{
		m_tabs[i].m_frame.offset_to(i*100, 0);
		m_tabs[i].m_frame.resize_to(100, height);
	}
}


//########################################################


TabBar::Tab::Tab(const Rect& frame, const String& name)
:	m_frame(frame)
,	m_name(name)
,	m_active(false)
{
}

const Rect&
TabBar::Tab::frame() const
{
	return m_frame;
}

const String&
TabBar::Tab::name() const
{
	return m_name;
}

bool
TabBar::Tab::is_active() const
{
	return m_active;
}

void
TabBar::Tab::draw(DC& dc)
{
	const RGBColor ACTIVE_COLOR = ntk::app_color();// + 30;
	Rect rect = frame();
	rect.top += 1;

	dc.push_state();

	if(is_active())
	{
		dc.set_brush(ACTIVE_COLOR);
		dc.fill_rect(rect);

		dc.set_pen(ntk::outline_color());
		dc.stroke_line(rect.left_top(), rect.left_bottom());
		dc.stroke_line(rect.left_top(), rect.right_top());
		dc.stroke_line(rect.right_top(), rect.right_bottom());

		dc.set_text_color(ntk::font_color());
	}
	else
		dc.set_text_color(ntk::font_color() - 30);

	ntk::draw_string_center(dc, rect, name());

	dc.pop_state();
}


//########################################################
}// namespace ntk
