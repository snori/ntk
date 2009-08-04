/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: interfaceutility.cpp,v 1.3 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/interfaceutility.h"


#include <ntk/interface/dc.h>
#include <ntk/interface/gdiobject.h>
#include <ntk/interface/wnd.h>
#include <ntk/interface/window.h>
#include <ntk/interface/view.h>


namespace ntk {


//########################################################
// drawing utilities

static const int g_color_difference_table[] = {
	20,
	15,
	10
};

int
light_edge_color_difference(int level)
{
	assert(0 <= level && level < sizeof(g_color_difference_table) / sizeof(g_color_difference_table[0]));
	return g_color_difference_table[level];
}

int
dark_edge_color_difference(int level)
{
	assert(0 <= level && level < sizeof(g_color_difference_table) / sizeof(g_color_difference_table[0]));
	return g_color_difference_table[level];
}

void
stroke_edge(DC& dc, const Rect& rect, const Pen& light_pen, const Pen& dark_pen)
{
	Pen default_pen = dc.pen();

	dc.set_pen(light_pen);
	dc.move_to(rect.left, rect.bottom -1);
	dc.stroke_line(rect.left, rect.top);
	dc.stroke_line(rect.right -1, rect.top);

	dc.set_pen(dark_pen);
	dc.stroke_line(rect.right -1, rect.bottom -1);
	dc.stroke_line(rect.left, rect.bottom -1);

	dc.set_pen(default_pen);
}

void
draw_string_center(DC& dc, const Rect& rect, const String& str)
{
	uint prev_align = dc.set_text_align(TA_CENTER | TA_TOP);

	Point str_pos(
		rect.left + rect.width() /2,
		rect.top + (rect.height() - dc.string_height()) /2);
	dc.draw_string(str, str_pos);

	dc.set_text_align(prev_align);
}


//########################################################
}// namespace ntk
