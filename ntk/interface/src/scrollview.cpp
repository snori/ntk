/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: scrollview.cpp,v 1.2 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/scrollview.h"


#include <assert.h>
#include <ntk/interface/scrollbar.h>


namespace ntk {


//########################################################
// public methods

ScrollView::ScrollView(const String& name, View* target, uint mode, uint flags, bool v, bool h, const RGBColor& color)
:	View(Rect(0, 0, 100, 100), name, mode, flags, color)
,	m_target(NULL)
,	m_v_scroll_bar(NULL)
,	m_h_scroll_bar(NULL)
{
	assert(target);

	Rect rect = target->frame();

	target->move_to(0, 0);// ‹Ù‹}”ð“ï‘[’u

	if(v)
	{
		m_v_scroll_bar = new ScrollBar(
			Rect(0, 0, ScrollBar::V_SCROLL_BAR_WIDTH, 10),
			"VScrollBar",
			NULL,
			0, 0,
			ScrollBar::VERTICAL,
			FOLLOW_RIGHT | FOLLOW_TOP_BOTTOM);
		add_child(m_v_scroll_bar);

		rect.right += m_v_scroll_bar->frame().width();
	}

	if(h)
	{
		m_h_scroll_bar = new ScrollBar(
			Rect(0, 0, 10, ScrollBar::H_SCROLL_BAR_HEIGHT),
			"HScrollBar",
			NULL,
			0, 0,
			ScrollBar::HORIZONTAL,
			FOLLOW_BOTTOM | FOLLOW_LEFT_RIGHT);
		add_child(m_h_scroll_bar);

		rect.bottom += m_h_scroll_bar->frame().height();
	}

	move_to(rect.left_top());
	resize_to(rect.width(), rect.height());

	set_target(target);
}

ScrollView::~ScrollView()
{
}

View*
ScrollView::target() const
{
	return m_target;
}

void
ScrollView::set_target(View* target)
{
	if(m_target) remove_child(m_target);

	m_target = target;

	if(m_target)
		add_child(m_target);

	if(m_v_scroll_bar)
	{
		Rect rect = bounds();
		rect.left = rect.right - ScrollBar::V_SCROLL_BAR_WIDTH;

		if(m_h_scroll_bar) rect.bottom -= ScrollBar::H_SCROLL_BAR_HEIGHT;

		m_v_scroll_bar->move_to(rect.left_top());
		m_v_scroll_bar->resize_to(rect.width(), rect.height());

		m_v_scroll_bar->set_target(m_target);
	}

	if(m_h_scroll_bar)
	{
		Rect rect = bounds();
		rect.top = rect.bottom - ScrollBar::H_SCROLL_BAR_HEIGHT;

		if(m_v_scroll_bar) rect.right -= ScrollBar::V_SCROLL_BAR_WIDTH;

		m_h_scroll_bar->move_to(rect.left_top());
		m_h_scroll_bar->resize_to(rect.width(), rect.height());

		m_h_scroll_bar->set_target(m_target);
	}
}


//########################################################
}// namespace ntk
