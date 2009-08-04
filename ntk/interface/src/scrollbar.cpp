/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: scrollbar.cpp,v 1.8 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/scrollbar.h"


#include <assert.h>
#include <ntk/windows/windows.h>
#include <ntk/interface/window.h>
#include <ntk/interface/utility.h>


namespace ntk {


//########################################################


namespace {


static uint
flags2style(ScrollBar::Orientation orient, uint flags)
{return 0;
	uint style = 0;

	if(orient == ScrollBar::VERTICAL)
	{
		style |= SBS_VERT;

		if(flags & ScrollBar::DEFAULT_SCROLL_BAR_SIZE)
		{
			if(flags & ScrollBar::LEFT_ALIGN)
				style |= SBS_LEFTALIGN;
			else if(flags & ScrollBar::RIGHT_ALIGN)
				style |= SBS_RIGHTALIGN;
		}
	}else{
		style |= SBS_HORZ;

		if(flags & ScrollBar::DEFAULT_SCROLL_BAR_SIZE)
		{
			if(flags & ScrollBar::TOP_ALIGN)
				style |= SBS_TOPALIGN;
			else if(flags & ScrollBar::BOTTOM_ALIGN)
				style |= SBS_BOTTOMALIGN;
		}
	}

	return style;
}


}// anonymous namespace


//########################################################
// public methods

ScrollBar::ScrollBar(const Rect& frame, const String& name, View* target, int min_val, int max_val, Orientation orient, uint mode, uint flags, const RGBColor& color)
:	View(frame, name, mode, flags | WILL_DRAW | FRAME_EVENTS, transparent_color(), DEFAULT_FLAGS | flags2style(orient, flags))
,	m_value(0)
,	m_target(target)
,	m_min_value(0)
,	m_max_value(0)
,	m_small_step(3)
,	m_large_step(-1)
,	m_orientation(orient)
,	m_view_color(color)
,	m_moving_knob(false)
{
	set_range(min_val, max_val);
}

ScrollBar::~ScrollBar()
{
}

//********************************************************
// public accessors

View*
ScrollBar::target() const
{
	return m_target;
}

void
ScrollBar::set_target(View* target)
{
	if(m_target)
	{
		if(m_orientation == VERTICAL)
		{
			assert(m_target->m_v_scroll_bar == this);
			m_target->m_v_scroll_bar = NULL;
		} else {
			assert(m_target->m_h_scroll_bar == this);
			m_target->m_h_scroll_bar = NULL;
		}
	}

	m_target = target;

	if(m_target)
	{
		if(m_orientation == VERTICAL)
		{
			assert(m_target->m_v_scroll_bar == NULL);
			m_target->m_v_scroll_bar = this;
		} else {
			assert(m_target->m_h_scroll_bar == NULL);
			m_target->m_h_scroll_bar = this;
		}
	}
}

int
ScrollBar::value() const
{
	return m_value;
}

void
ScrollBar::set_value(int val)
{
	if(val > m_max_value)
		val = m_max_value;
	else if(val < m_min_value)
		val = m_min_value;

	if(m_value == val) return;

	m_value = val;

	window()->post_message(VALUE_CHANGED, this);
}

int
ScrollBar::min_value() const
{
	return m_min_value;
}

int
ScrollBar::max_value() const
{
	return m_max_value;
}

void
ScrollBar::set_range(int min_val, int max_val)
{
	if(min_val <= max_val)
	{
		m_min_value = min_val;
		m_max_value = max_val;
	} else {
		m_min_value = max_val;
		m_max_value = min_val;
	}

	invalidate();
}

int
ScrollBar::small_step() const
{
	return m_small_step;
}

int
ScrollBar::large_step() const
{
	return m_large_step;
}

void
ScrollBar::set_step(int small_, int large_)
{
	m_small_step = small_;
	m_large_step = large_;
}

ScrollBar::Orientation
ScrollBar::orientation() const
{
	return m_orientation;
}

const RGBColor&
ScrollBar::view_color() const
{
	return m_view_color;
}

void
ScrollBar::set_view_color(const RGBColor& color)
{
	m_view_color = color;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
ScrollBar::value_changed(int new_value)
{
	if(m_target)
	{
		if(m_orientation == VERTICAL)
			m_target->move_to(m_target->frame().left, -new_value);
		else
			m_target->move_to(-new_value, m_target->frame().top);
	}
}

void
ScrollBar::draw(PaintDC& dc)
{
	Rect rect = bounds();

	MemoryDC mem_dc(dc);
	mem_dc.set_bitmap(DDB(dc, rect.width(), rect.height()));

	mem_dc.set_pen(outline_pen());
	mem_dc.stroke_rect(rect);

	Pen
		light_pen(view_color() + light_edge_color_difference()),
		light_pen2(view_color() + light_edge_color_difference(1)),
		dark_pen(view_color() - dark_edge_color_difference()),
		dark_pen2(view_color() - dark_edge_color_difference(1));
	Brush
		view_color_brush(view_color()),
		light_brush(view_color() + light_edge_color_difference()),
		dark_brush(view_color() - dark_edge_color_difference());

	enum{ SIDE_BAND_WIDTH = 3 };

	rect.inset_by(1, 1);
	if(m_orientation == VERTICAL)
	{
		rect.top = m_minus_arrow_frame.bottom;
		rect.bottom = m_plus_arrow_frame.top;

		mem_dc.stroke_line(rect.left_top(), rect.right_top());
		mem_dc.stroke_line(rect.left_bottom(), rect.right_bottom());

		rect.inset_by(0, 1);
		Rect band_rect = rect;
		band_rect.right = band_rect.left + SIDE_BAND_WIDTH;

		mem_dc.set_brush(dark_brush);
		mem_dc.fill_rect(band_rect);

		band_rect = rect;
		band_rect.left = band_rect.right - SIDE_BAND_WIDTH;

		mem_dc.set_brush(light_brush);
		mem_dc.fill_rect(band_rect);

		rect.left += SIDE_BAND_WIDTH;
		rect.right -= SIDE_BAND_WIDTH;
		mem_dc.set_brush(view_color_brush);
		mem_dc.fill_rect(rect);
	} else {
		rect.left = m_minus_arrow_frame.right;
		rect.right = m_plus_arrow_frame.left;

		mem_dc.stroke_line(rect.left_top(), rect.left_bottom());
		mem_dc.stroke_line(rect.right_top(), rect.right_bottom());

		rect.inset_by(1, 0);
		Rect band_rect = rect;
		band_rect.bottom = band_rect.top + SIDE_BAND_WIDTH;

		mem_dc.set_brush(dark_brush);
		mem_dc.fill_rect(band_rect);

		band_rect = rect;
		band_rect.top = band_rect.bottom - SIDE_BAND_WIDTH;

		mem_dc.set_brush(light_brush);
		mem_dc.fill_rect(band_rect);

		rect.top += SIDE_BAND_WIDTH;
		rect.bottom -= SIDE_BAND_WIDTH;
		mem_dc.set_brush(view_color());
		mem_dc.fill_rect(rect);
	}

	// draw knob
	if(m_max_value - m_min_value > 0)
	{
		rect = m_knob_frame;

		rect.inset_by(-1, -1);// m_knob_frame はアウトラインの分は含まない
		mem_dc.set_pen(outline_pen());
		mem_dc.stroke_rect(rect);

		rect.inset_by(1, 1);
		stroke_edge(mem_dc, rect, light_pen, dark_pen);

		rect.inset_by(1, 1);
		stroke_edge(mem_dc, rect, light_pen2, dark_pen2);

		rect.inset_by(1, 1);
		mem_dc.set_brush(view_color_brush);
		mem_dc.fill_rect(rect);
	}

	// draw arrow buttons
	dc.set_brush(view_color_brush);

	// minus arrow
	rect = m_minus_arrow_frame;
	stroke_edge(mem_dc, rect, light_pen, dark_pen);

	rect.inset_by(1, 1);
	stroke_edge(mem_dc, rect, light_pen2, dark_pen2);

	rect.inset_by(1, 1);
	mem_dc.fill_rect(rect);

	// plus arrow
	rect = m_plus_arrow_frame;
	stroke_edge(mem_dc, rect, light_pen, dark_pen);

	rect.inset_by(1, 1);
	stroke_edge(mem_dc, rect, light_pen2, dark_pen2);

	rect.inset_by(1, 1);
	mem_dc.fill_rect(rect);

	// swap buffer
	dc.bit_blt(bounds(), mem_dc, Point(0, 0));
}

void
ScrollBar::attached_to_window()
{
}

void
ScrollBar::frame_resized(coord w, coord h)
{
	calculate_frames_();
}

void
ScrollBar::mouse_down(const Point& point, uint buttons, bool dbl)
{
	if(buttons == LEFT_MOUSE_BUTTON)
	{
		if(m_knob_frame.contains(point) && dbl == false)
		{
			begin_capture();
			m_moving_knob = true;
			m_prev_point = point;
		}
	}
}

void
ScrollBar::mouse_up(const Point& point, uint buttons)
{
	if(buttons == LEFT_MOUSE_BUTTON || buttons == RIGHT_MOUSE_BUTTON)
	{
		if(m_moving_knob)
		{
			m_moving_knob = false;
			end_capture();
		}
	}
}

void
ScrollBar::mouse_moved(const Point& point, uint buttons, uint transit, const Message* data)
{
	if(m_moving_knob)
	{
		if(m_orientation == VERTICAL)
			set_value(value() + (point.y - m_prev_point.y));
		else
			set_value(value() + (point.x - m_prev_point.x));

		m_prev_point = point;
	}
}

void
ScrollBar::message_received(const Message& message)
{
	switch(message.what)
	{
	case VALUE_CHANGED:
		calculate_frames_();
		value_changed(m_value);
		break;

	default:
		View::message_received(message);
	}
}

//****************************************************
// private functions

void
ScrollBar::calculate_frames_()
{
	m_minus_arrow_frame = bounds();
	m_minus_arrow_frame.inset_by(1, 1);
	m_knob_frame = m_plus_arrow_frame = m_minus_arrow_frame;

	if(m_orientation == VERTICAL)
	{
		m_minus_arrow_frame.bottom = m_minus_arrow_frame.top + m_minus_arrow_frame.width();
		m_plus_arrow_frame.top = m_plus_arrow_frame.bottom - m_plus_arrow_frame.width();

		m_knob_frame.top = m_minus_arrow_frame.bottom + value() +1;
		m_knob_frame.bottom = m_plus_arrow_frame.top - ((m_max_value - m_min_value) - value()) -1;// 1 は線の分
	} else {
		m_minus_arrow_frame.right = m_minus_arrow_frame.left + m_minus_arrow_frame.height();
		m_plus_arrow_frame.left = m_plus_arrow_frame.right - m_plus_arrow_frame.height();

		m_knob_frame.left = m_minus_arrow_frame.right + value() +1;
		m_knob_frame.right = m_plus_arrow_frame.left - ((m_max_value - m_min_value) - value()) -1;// 1 は線の分
	}

	invalidate();
}


//########################################################
}// namespace ntk
