/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: point.h,v 1.7 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_POINT_H__
#define __NTK_INTERFACE_POINT_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


class Point : public POINT {
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
	Point() {}
	Point(Value x_, Value y_) {x=x_; y=y_;}
	Point(const Point& rhs) {operator=(rhs);}
	Point(const POINT& rhs) {operator=(rhs);}
	Point(const SIZE& rhs) {operator=(rhs);}
	Point& operator=(const Point& rhs)
	{
		x=rhs.x;
		y=rhs.y;
		return *this;
	}
	Point& operator=(const POINT& rhs)
	{
		x=rhs.x;
		y=rhs.y;
		return *this;
	}
	Point& operator=(const SIZE& rhs)
	{
		x=rhs.cx;
		y=rhs.cy;
		return *this;
	}

	void reset(Value x_, Value y_)
	{
		x = x_;
		y = y_;
	}

	//
	// operators
	//
	Value& operator[](int index) {return ((Value*)this)[index];}
	const Value& operator[](int index) const {return ((Value*)this)[index];}

	friend Point& operator+=(Point& lhs, Value rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		return lhs;
	}
	friend const Point operator+(Point lhs, Value rhs)
	{
		return lhs += rhs;
	}

	friend Point& operator+=(Point& lhs, const Point& rhs)
	{
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}
	friend const Point operator+(Point lhs, const Point& rhs)
	{
		return lhs += rhs;
	}

	friend Point& operator-=(Point& lhs, Value rhs)
	{
		lhs.x -= rhs;
		lhs.y -= rhs;
		return lhs;
	}
	friend const Point operator-(Point lhs, Value rhs)
	{
		return lhs -= rhs;
	}

	friend Point& operator-=(Point& lhs, const Point& rhs)
	{
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}
	friend const Point operator-(Point lhs, const Point& rhs)
	{
		return lhs -= rhs;
	}

	// íPçÄââéZéq
	friend const Point operator-(Point point)
	{
		point.x = -point.x;
		point.y = -point.y;
		return point;
	}

	// î‰ärââéZéq
	friend bool operator==(const Point& lhs, const Point& rhs)
	{
		return lhs.x == rhs.x && lhs.y == rhs.y;
	}
	friend bool operator!=(const Point& lhs, const Point& rhs)
	{
		return !(lhs == rhs);
	}

};// class Point


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Point point_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Point NPoint;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Point ntk_point;
	#endif
#endif


#endif//EOH
