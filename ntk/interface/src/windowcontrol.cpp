/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: windowcontrol.cpp,v 1.10 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/windowcontrol.h"


#include <assert.h>
#include <ntk/windows/windows.h>
#include <ntk/interface/window.h>
#include <ntk/interface/dc.h>


namespace ntk {


//########################################################
// public methods

WindowControl::WindowControl()
:	m_window(NULL)
,	m_look(-1)
,	m_feel(-1)
,	m_flags(-1)
{
}

WindowControl::~WindowControl()
{
}

Rect
WindowControl::client_frame() const
{
	return window()->window_frame();
}

Rect
WindowControl::client_bounds() const
{
	return window()->window_bounds();
}

void
WindowControl::draw(DC& dc)
{
#if 0
	Window* window = this->window();

	if(window == NULL)
	{
		status_t(st::ERR, "WindowControl::draw(NDC& dc): window が設定されていません\n").show_error();
		return;
	}

	// draw window_frame //
	{
		Rect rect = window->window_bounds();

		dc.set_pen(system_pen(OUTLINE_PEN));
		dc.stroke_rect(rect);

		rect.inset_by(1, 1);
		dc.set_pen(system_color(WINDOW_FRAME_COLOR));
		dc.stroke_rect(rect);

		dc.set_pen(system_pen(OUTLINE_PEN));
		rect.inset_by(1, 1);
		dc.stroke_rect(rect);
	}

	// draw title_bar //
	{
		enum{ LEFT_MARGIN = 25 };

		Rect rect = window->bounds();

		dc.set_pen(outline_pen());
		dc.stroke_rect(rect);

		rect.inset_by(1, 1);
		if(window->is_active())
			dc.set_brush(system_color(ACTIVE_WINDOW_TITLE_BAR_COLOR));
		else
			dc.set_brush(system_color(INACTIVE_WINDOW_TITLE_BAR_COLOR));
		dc.fill_rect(rect);

		dc.set_font(bold_font());
		dc.set_text_color(system_color(ACTIVE_WINDOW_TITLE_BAR_FONT_COLOR));
		dc.draw_string(window->title(), Point(rect.left + LEFT_MARGIN, rect.top + (rect.height() - dc.string_height()) /2));
	}
#endif
}

uint
WindowControl::area_test(const Point& point, uint default_value) const
{
	return default_value;
}

void
WindowControl::attached_to_window()
{
}

void
WindowControl::detached_from_window()
{
}

uint
WindowControl::look() const
{
	return m_look;
}

void
WindowControl::set_look(uint look)
{
	m_look = look;
}

uint
WindowControl::feel() const
{
	return m_feel;
}

void
WindowControl::set_feel(uint feel)
{
	m_feel = feel;
}

uint
WindowControl::flags() const
{
	return m_flags;
}

void
WindowControl::set_flags(uint flags)
{
	m_flags = flags;
}

//********************************************************
// protected functions

uint
WindowControl::area_test_utility(const Point& point, coord left_frame_width, coord top_frame_height, coord right_frame_width, coord bottom_frame_height, coord title_bar_height, uint default_value) const
{
	Window* window = this->window();

	const Rect& frame = window->window_frame();
	uint look = this->look();
	uint flag = this->flags();

	coord x = point.x - frame.left;
	coord y = point.y - frame.top;
	coord width = frame.width();
	coord height = frame.height();
	coord menu_bar_height = GetSystemMetrics(SM_CYMENU);

	if((flag & Window::NOT_H_RESIZABLE && flag & Window::NOT_V_RESIZABLE) == false)
	{
		static const coord RESIZER_SIZE = 20;

		// 左ボーダー
		if(0 <= x && x < left_frame_width)
		{
			if(flag & Window::NOT_H_RESIZABLE)
				;// goto top border
			else if(flag & Window::NOT_V_RESIZABLE)
				return HTLEFT;
			else// Window::H_V_RESIZABLE
			{
				if(0 <= y && y < RESIZER_SIZE)// 左上
					return HTTOPLEFT;
				else if(height - RESIZER_SIZE <= y && y < height)// 左下
					return HTBOTTOMLEFT;
				else
					return HTLEFT;
			}
		}

		// 上ボーダー
		if(0 <= y && y < top_frame_height)
		{
			if(flag & Window::NOT_V_RESIZABLE)
				;// goto right border
			else if(flag & Window::NOT_H_RESIZABLE)
				return HTTOP;
			else// Window::H_V_RESIZABLE
			{
				if(0 <= x && x < RESIZER_SIZE)// 左上
					return HTTOPLEFT;
				else if(width - RESIZER_SIZE <= x && x < width)// 右上
					return HTTOPRIGHT;
				else
					return HTTOP;
			}
		}

		//右ボーダー
		if(width - right_frame_width <= x && x < width)
		{
			if(flag & Window::NOT_H_RESIZABLE)
				;// goto bottom border
			else if(flag & Window::NOT_V_RESIZABLE)
				return HTRIGHT;
			else// H_V_RESIZABLE
			{
				if(0 <= y && y < RESIZER_SIZE)// 右上
					return HTTOPRIGHT;
				else if(height - RESIZER_SIZE <= y && y < height)// 右下
					return HTBOTTOMRIGHT;
				else
					return HTRIGHT;
			}
		}

		// 下ボーダー
		if(height - bottom_frame_height <= y && y < height)
		{
			if(flag & Window::NOT_V_RESIZABLE)
				;// do nothing
			else if(flag & Window::NOT_H_RESIZABLE)
				return HTBOTTOM;
			else// H_V_RESIZABLE
			{
				if(0 <= x && x < RESIZER_SIZE)// 左下
					return HTBOTTOMLEFT;
				else if(width - RESIZER_SIZE <= x && x < width)// 右下
					return HTBOTTOMRIGHT;
				else
					return HTBOTTOM;
			}
		}
	}

	return default_value;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

Window*
WindowControl::window() const
{
	return m_window;
}


//########################################################
}// namespace ntk
