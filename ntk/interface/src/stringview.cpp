/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stringview.cpp,v 1.3 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/stringview.h"


#include <assert.h>


namespace ntk {


//########################################################
// public methods

StringView::StringView(const Rect& frame_, const String& name_, const String& text, uint mode_, uint flags_, const RGBColor& color)
:	Control(frame_, name_, text, NULL, mode_, flags_ | WILL_DRAW, color)
,	m_alignment(ALIGN_LEFT | ALIGN_TOP)
{
	calc_text_height_();
}

StringView::~StringView()
{
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulator

String
StringView::text() const
{
	return label();
}

void
StringView::set_text(const String& text_)
{
	set_label(text_);
	calc_text_height_();
}

uint
StringView::alignment() const
{
	return m_alignment;
}

void
StringView::set_alignment(uint alignment_)
{
	m_alignment = alignment_;
	if(! is_hidden()) invalidate();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
StringView::draw(PaintDC& dc)
{
	uint flag = 0;

	switch(m_alignment & HORIZONTAL_ALIGN_MASK)
	{
	case ALIGN_LEFT:
		flag |= DT_LEFT;
		break;

	case ALIGN_RIGHT:
		flag |= DT_RIGHT;
		break;

	case ALIGN_CENTER:
		flag |= DT_CENTER;
		break;
	}

	Rect rect = bounds();
	rect.bottom = rect.top + m_text_height;

	switch(m_alignment & VERTICAL_ALIGN_MASK)
	{
	case ALIGN_TOP:
		break;

	case ALIGN_BOTTOM:
		rect.offset_to(0, frame().height() - rect.height());
		break;

	case ALIGN_VCENTER:
		rect.offset_to(0, (frame().height() - rect.height()) /2);
		break;
	}

	DrawText(dc.hdc(), text().c_str(), -1, &rect, flag);
}


//********************************************************
// private functions

void
StringView::calc_text_height_()
{
	const String& text = this->text();
	coord string_height = dc().string_height();
	m_text_height = string_height;// ïKÇ∏àÍçsï™ÇÃçÇÇ≥ÇÕÇ†ÇÈ

	for(String::const_iterator it = text.begin(); it != text.end(); ++it)
		if(*it == '\n')
			m_text_height += string_height;
}


//########################################################
}// namespace ntk
