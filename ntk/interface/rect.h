/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: rect.h,v 1.8 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_RECT_H__
#define __NTK_INTERFACE_RECT_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_POINT_H__
#include <ntk/interface/point.h>
#endif


namespace ntk {


class Rect : public RECT {
public:
	//
	// types
	//
	typedef coord Value;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Value value_t;
	#endif

	//
	// methods
	//
	Rect()
	{
		left = top = right = bottom = 0;
	}
	Rect(Value l, Value t, Value r, Value b)
	{
		left		= l;
		top			= t;
		right		= r;
		bottom	= b;
	}
	Rect(const Point& p1, const Point& p2)
	{
		left		= p1.x;
		top			= p1.y;
		right		= p2.x;
		bottom	= p2.y;
	}
	Rect(const Rect& rhs) {operator=(rhs);}
	Rect(const RECT& rhs) {operator=(rhs);}
	Rect& operator=(const Rect& rhs)
	{
		if(&rhs == this)
			return *this;

		left		= rhs.left;
		top			= rhs.top;
		right		= rhs.right;
		bottom	= rhs.bottom;

		return *this;
	}
	Rect& operator=(const RECT& rhs)
	{
		if(&rhs == this)
			return *this;

		left		= rhs.left;
		top			= rhs.top;
		right		= rhs.right;
		bottom	= rhs.bottom;

		return *this;
	}

	bool contains(const Point& point) const
	{
		return contains(point.x, point.y);
	}
	bool contains(Value x, Value y) const
	{
#if 0
		return
			x >= left &&
			x < right &&
			y >= top &&
			y < bottom;
#else
		if(left <= right)
		{
			if(top <= bottom)
				return
					x >= left &&
					x < right &&
					y >= top &&
					y < bottom;
			else
				return
					x >= left &&
					x < right &&
					y >= bottom &&
					y < top;
		}else{
			if(top <= bottom)
				return
					x >= right &&
					x < left &&
					y >= top &&
					y < bottom;
			else
				return
					x >= right &&
					x < left &&
					y >= bottom &&
					y < top;
		}
#endif
	}
	bool contains(const Rect& rect) const
	{
		return contains(rect.left, rect.top, rect.right, rect.bottom);
	}
	bool contains(Value l, Value t, Value r, Value b) const
	{
#if 0
		return
			l >= left &&
			r <= right &&
			t >= top &&
			b <= bottom;
#else
		if(left <= right)
		{
			if(top <= bottom)
				return
					l >= left &&
					l < right &&
					r >= left &&
					r < right &&
					t >= top &&
					t < bottom &&
					b >= top &&
					b < bottom;
			else
				return
					l >= left &&
					l < right &&
					r >= left &&
					r < right &&
					t >= bottom &&
					t < top &&
					b >= bottom &&
					b < top;
		}else{
			if(top <= bottom)
				return
					l >= right &&
					l < left &&
					r >= right &&
					r < left &&
					t >= top &&
					t < bottom &&
					b >= top &&
					b < bottom;
			else
				return
					l >= right &&
					l < left &&
					r >= right &&
					r < left &&
					t >= bottom &&
					t < top &&
					b >= bottom &&
					b < top;
		}
#endif
	}
	bool intersects(const Rect& rect) const
	{
		return intersects(rect.left, rect.top, rect.right, rect.bottom);
	}
	bool intersects(Value l, Value t, Value r, Value b) const
	{
		return
			r >= left &&
			l <= right &&
			b >= top &&
			t <= bottom;
	}

	void offset_to(Value x, Value y)
	{
		right		+= x - left;
		bottom	+= y - top;
		left		= x;
		top			= y;
	}
	const Rect& offset_to_self(Value x, Value y)
	{
		offset_to(x, y);
		return *this;
	}
	Rect offset_to_copy(Value x, Value y) const
	{
		return Rect(*this).offset_to_self(x, y);
	}
	void offset_to(const Point& point) {offset_to(point.x, point.y);}
	const Rect& offset_to_self(const Point& point) {return offset_to_self(point.x, point.y);}
	Rect offset_to_copy(const Point& point) const {return offset_to_copy(point.x, point.y);}

	void offset_by(Value horizontal, Value vertical)
	{
		left		+= horizontal;
		top			+= vertical;
		right		+= horizontal;
		bottom	+= vertical;
	}
	const Rect& offset_by_self(Value horizontal, Value vertical)
	{
		offset_by(horizontal, vertical);
		return *this;
	}
	Rect offset_by_copy(Value horizontal, Value vertical) const
	{
		return Rect(*this).offset_by_self(horizontal, vertical);
	}
	void offset_by(const Point& point) {offset_by(point.x, point.y);}
	const Rect& offset_by_self(const Point& point) {return offset_by_self(point.x, point.y);}
	Rect offset_by_copy(const Point& point) const {return offset_by_copy(point.x, point.y);}

	void resize_to(Value width, Value height)
	{
		right		= left + width;
		bottom	= top + height;
	}
	const Rect& resize_to_self(Value width, Value height)
	{
		resize_to(width, height);
		return *this;
	}
	Rect resize_to_copy(Value width, Value height) const
	{
		return Rect(*this).resize_to_self(width, height);
	}

	void resize_by(Value horizontal, Value vertical)
	{
		right		+= horizontal;
		bottom	+= vertical;
	}
	const Rect& resize_by_self(Value horizontal, Value vertical)
	{
		resize_by(horizontal, vertical);
		return *this;
	}
	Rect resize_by_copy(Value horizontal, Value vertical) const
	{
		return Rect(*this).resize_by_self(horizontal, vertical);
	}

	void inset_by(Value horizontal, Value vertical)
	{
		left		+= horizontal;
		right		-= horizontal;
		top			+= vertical;
		bottom	-= vertical;
	}
	const Rect& inset_by_self(Value horizontal, Value vertical)
	{
		inset_by(horizontal, vertical);
		return *this;
	}
	Rect inset_by_copy(Value horizontal, Value vertical) const
	{
		return Rect(*this).inset_by_self(horizontal, vertical);
	}

	//
	// accessors and manipulators
	//
	void reset(Value l, Value t, Value r, Value b)
	{
		left		= l;
		top			= t;
		right		= r;
		bottom	= b;
	}

	Value width() const {return right - left;}
	Value height() const {return bottom - top;}

	Point left_top() const {return Point(left, top);}
	Point right_top() const {return Point(right, top);}
	Point left_bottom() const {return Point(left, bottom);}
	Point right_bottom() const {return Point(right, bottom);}

	void set_left_top(const Point& p) {left = p.x; top = p.y;}
	void set_right_top(const Point& p) {right = p.x; top = p.y;}
	void set_left_bottom(const Point& p) {left = p.x; bottom = p.y;}
	void set_right_bottom(const Point& p) {right = p.x; bottom = p.y;}

	bool is_valid() const
	{
		return (left <= right) && (top <= bottom);
	}
	void validate()
	{
		if(left > right) swap_(left, right);
		if(top > bottom) swap_(top, bottom);
	}

	//
	// operators
	//
	Value& operator[](int index) {return ((Value*)this)[index];}
	const Value& operator[](int index) const {return ((Value*)this)[index];}

	friend const Rect& operator&=(Rect& lhs, const Rect& rhs)
	{
		lhs.reset(
			lhs.left   > rhs.left   ? lhs.left   : rhs.left,
			lhs.top    > rhs.top    ? lhs.top    : rhs.top,
			lhs.right  < rhs.right  ? lhs.right  : rhs.right,
			lhs.bottom < rhs.bottom ? lhs.bottom : rhs.bottom);

		return lhs;
	}
	friend Rect operator&(Rect lhs, const Rect& rhs)
	{
		lhs &= rhs;
		return lhs;
	}
	friend const Rect& operator|=(Rect& lhs, const Rect& rhs)
	{
		lhs.reset(
			lhs.left   < rhs.left   ? lhs.left   : rhs.left,
			lhs.top    < rhs.top    ? lhs.top    : rhs.top,
			lhs.right  > rhs.right  ? lhs.right  : rhs.right,
			lhs.bottom > rhs.bottom ? lhs.bottom : rhs.bottom);
		return lhs;
	}
	friend Rect operator|(Rect lhs, const Rect& rhs)
	{
		lhs |= rhs;
		return lhs;
	}

private:
	//
	// utilities
	//
	void swap_(coord& lhs, coord& rhs)
	{
		coord tmp = lhs;
		lhs = rhs;
		rhs = tmp;
	}

};// class Rect


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Rect rect_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Rect NRect;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Rect ntk_rect;
	#endif
#endif


#endif//EOH
