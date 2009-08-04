/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: layout.h,v 1.3 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_LAYOUT_H__
#define __NTK_INTERFACE_LAYOUT_H__


namespace ntk {

#if 0
class Layout {
public:
	//
	// methods
	//
	NtkExport virtual void frame_resized();

private:
	//
	// friends
	//
	friend class Panel;

};// class Layout
#endif

class Layout {
public:
	//
	// types
	//
	enum Direction {
		HORIZONTAL, VERTICAL
	};

	enum Side {
		LEFT, TOP, RIGHT, BOTTOM
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Direction direction_t;
		typedef Side side_t;
	#endif

	//
	// methods
	//
	Layout(View* container, coord h_space = 0, coord v_space = 0)
	:	m_container(container)
	,	m_h_space(h_space)
	,	m_v_space(v_space)
	{
		if(m_container)
			m_rest_rect = m_container->bounds();
		else
			m_rest_rect.reset(0, 0, 0, 0);
	}
	Layout(View* container, const Rect& rect, coord h_space = 0, coord v_space = 0)
	:	m_container(container)
	,	m_rest_rect(rect)
	,	m_h_space(h_space)
	,	m_v_space(v_space)
	{
	}

	status_t add_child(View* child, coord size, Direction dir = VERTICAL, coord space = -1)
	{
		if(m_container == NULL)
			return false;

		status_t sts = m_container->add_child(child);

		if(sts.is_valid_without_eval() == false)
			return sts;

		child->move_to(m_rest_rect.left_top());

		if(dir == VERTICAL)
		{
			child->resize_to(m_rest_rect.width(), size);
			m_rest_rect.top += size;

			if(space == -1)
				space = m_v_space;

			m_rest_rect.top += space;
		}
		else
		{
			child->resize_to(size, m_rest_rect.height());
			m_rest_rect.left += size;

			if(space == -1)
				space = m_h_space;

			m_rest_rect.left += space;
		}

		return sts;
	}

	Layout split(coord split_point, Side side) const
	{
		Rect rect = m_rest_rect;

		switch(side)
		{
		case LEFT:
			rect.right = rect.left + split_point;
			break;

		case TOP:
			rect.bottom = rect.top + split_point;
			break;

		case RIGHT:
			rect.left = rect.right - split_point;
			break;

		case BOTTOM:
			rect.top += rect.bottom - split_point;
			break;
		}

		return Layout(m_container, rect, m_h_space, m_v_space);
	}
	Layout split(const String& percent, Side side) const
	{
		if(percent[percent.size() -1] != '%')
			return Layout(NULL);

		int split_point;
		sscanf(percent.c_str(), "%d", &split_point);

		if(split_point <= 0 || split_point >= 100)
			return *this;

		coord split_pos;
		if(side == LEFT || side == RIGHT)
			split_pos = m_rest_rect.width() * ((float)split_point / 100.f);
		else
			split_pos = m_rest_rect.height() * ((float)split_point / 100.f);

		return split(split_pos, side);
	}

	Rect& rest_rect() {return m_rest_rect;}
	const Rect& rest_rect() const {return m_rest_rect;}

private:
	//
	// data
	//
	View* m_container;
	Rect m_rest_rect;
	coord m_h_space, m_v_space;

};// class Layout


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Layout layout_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Layout NLayout;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Layout ntk_layout;
	#endif
#endif



#endif//EOH
