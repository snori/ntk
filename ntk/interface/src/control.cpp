/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: control.cpp,v 1.7 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/control.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/defs.h>
#include <ntk/interface/window.h>


namespace ntk {


//########################################################
// public methods


Control::ClickTracker::ClickTracker(Control* control)
:	m_control(control)
,	m_pushing(false)
{
}

Control::ClickTracker::~ClickTracker()
{
}

Control*
Control::ClickTracker::target() const
{
	return m_control;
}

void
Control::ClickTracker::set_target(Control* target)
{
	m_control = target;
}

bool
Control::ClickTracker::is_clicking() const
{
	return m_pushing;
}

void
Control::ClickTracker::mouse_down(const Point& point, uint buttons, bool dbl)
{
	m_control->begin_capture();

	m_control->set_value(true);
	m_pushing = true;

	m_control->invalidate();
}

void
Control::ClickTracker::mouse_up(const Point& point, uint buttons)
{
	if(m_pushing)
	{
		if(m_control->bounds().contains(point))
			m_control->invoke();

		m_pushing = false;
		m_control->end_capture();
	}
}

void
Control::ClickTracker::mouse_moved(const Point& point, uint buttons, uint transit, const Message* data)
{
	if(m_pushing)
	{
		bool contains = m_control->bounds().contains(point);
		if(
			(m_control->value() && !contains) ||
			(m_control->value() == false && contains))
		{
			m_control->set_value(! m_control->value());
			m_control->invalidate();
		}
	}
}

void
Control::ClickTracker::lost_capture()
{
	m_control->set_value(false);
	m_pushing = false;

	m_control->invalidate();
}


//########################################################
// public methods

Control::Control(const Rect& frame_, const String& name_, const String& label_, Message* msg, uint mode_, uint flags_, const RGBColor& color)
:	View(frame_, name_, mode_, flags_, color),
	Invoker(msg, NULL, NULL),
	m_label(label_),
	m_value(0),
	m_enabled(false)
{
	initialize_();
}

Control::Control(const String& class_name_, const Rect& frame_, const String& name_, const String& label_, Message* msg, uint mode_, uint flags_, const RGBColor& color, uint style, uint ex_style)
:	View(class_name_, frame_, name_, mode_, flags_, color, style, ex_style, reinterpret_cast<HMENU>(SYSTEM_CONTROL)),
	Invoker(msg, NULL, NULL),// check!!
	m_label(label_),
	m_value(0),
	m_enabled(false)
{
	initialize_();
}

Control::~Control()
{
	finalize_();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

String
Control::label() const
{
	return m_label;
}

void
Control::set_label(const String& label_)
{
	m_label = label_;

	invalidate();
}

int
Control::value() const
{
	return m_value;
}

void
Control::set_value(int val)
{
	if(val == m_value)
		return;

	m_value = val;

	invalidate();
}

bool
Control::is_enabled() const
{
	return m_enabled;
}

void
Control::set_enabled(bool flag)
{
	if(flag == m_enabled)
		return;

	m_enabled = flag;

	invalidate();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
Control::attached_to_window()
{
	View::attached_to_window();

	if(target()) set_target(target());
	else set_target(NULL, looper());

	if(window() && window()->current_focus() == NULL)
		make_focus();
}

void
Control::focus_changed(bool state)
{
	invalidate();
}

LRESULT
Control::system_message_received(const Message& message)
{
	switch(message.what)
	{
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		make_focus();
		return View::system_message_received(message);

	default:
		return View::system_message_received(message);
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// private functions

void
Control::initialize_()
{
	SendMessage(hwnd(), WM_SETFONT, (WPARAM)plain_font().handle(), MAKEWPARAM(TRUE, 0));
}

void
Control::finalize_()
{
	SendMessage(hwnd(), WM_SETFONT, NULL, MAKEWPARAM(FALSE, 0));// ïKóvÇ»Ç¢Ç©ÅH
}


//########################################################
}// namespace ntk
