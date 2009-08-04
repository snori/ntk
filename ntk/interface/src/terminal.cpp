/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: terminal.cpp,v 1.3 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/terminal.h"


#include <ntk/interface/scrollbar.h>
#include <ntk/interface/terminalview.h>


namespace ntk {


//########################################################


namespace {


Rect
modify_rect(Rect rect, bool v, bool h)
{
	if(v) rect.right -= ScrollBar::V_SCROLL_BAR_WIDTH;
	if(h) rect.bottom -= ScrollBar::H_SCROLL_BAR_HEIGHT;

	return rect;
}


class TerminalControl : public TerminalView {
public:
	TerminalControl(
		const Rect& frame_,
		const String& name,
		const RGBColor& color = ntk::view_color())
	:	TerminalView(
			frame_,
			name,
			FOLLOW_TOP | FOLLOW_LEFT_RIGHT,
			WILL_DRAW | NAVIGABLE /* | CHARA_FLAG*/,
			color)
	{
//		resize_to(frame().width(), dc().string_height());
	}

	void put_string(const char_t* buf, int32 size)
	{
		TerminalView::put_string(buf, size);
//		resize_to(frame().width(), line_height(dc()) * num_lines());
	}

	void put_char(char_t ch)
	{
		TerminalView::put_char(ch);

//		if(ch == '\n')
//			resize_by(frame().width(), line_height(dc()) * num_lines());
	}

};


}// anonymous namespace


//########################################################
// public methods

Terminal::Terminal(const Rect& frame, const String& name, uint mode, uint flags, bool v, bool h, const RGBColor& color)
:	ScrollView(
		name,
		new TerminalControl(modify_rect(frame, v, h), name, color),
		mode, flags, v, h, color)
{
	m_terminal_view = (TerminalView*)target();
//	resize_by(NScrollBar::V_SCROLL_BAR_WIDTH, 0);
}

Terminal::Terminal(const String& name, TerminalView* terminal_view, uint mode, uint flags, bool v, bool h, const RGBColor& color)
:	ScrollView(
		name,
		terminal_view,
		mode, flags, v, h, color)
{
	m_terminal_view = (TerminalView*)target();
//	resize_by(-NScrollBar::V_SCROLL_BAR_WIDTH, 0);
}

Terminal::~Terminal()
{
}


//########################################################
}// namespace ntk
