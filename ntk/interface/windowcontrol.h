/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: windowcontrol.h,v 1.9 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_WINDOWCONTROL_H__
#define __NTK_INTERFACE_WINDOWCONTROL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


class Window;
class Rect;
class Point;
class DC;


class WindowControl {
public:
	//
	// methods
	//
	NtkExport WindowControl();
	NtkExport virtual ~WindowControl();

	NtkExport virtual Rect client_frame() const;
	NtkExport virtual Rect client_bounds() const;

	NtkExport virtual void draw(DC& dc);
	NtkExport virtual uint area_test(const Point& point, uint default_value) const;

	NtkExport virtual void attached_to_window();
	NtkExport virtual void detached_from_window();

	NtkExport virtual uint look() const;
	NtkExport virtual void set_look(uint look);
	NtkExport virtual uint feel() const;
	NtkExport virtual void set_feel(uint feel);
	NtkExport virtual uint flags() const;
	NtkExport virtual void set_flags(uint flags);

	//
	// accessors
	//
	NtkExport Window* window() const;

protected:
	//
	// functions
	//
	NtkExport uint area_test_utility(
		const Point& point,
		coord left_frame_width, coord top_frame_height,
		coord right_frame_width, coord bottom_frame_height,
		coord title_bar_height, uint default_value) const;

private:
	//
	// data
	//
	Window* m_window;
	uint m_look, m_feel, m_flags;

	//
	// friends
	//
	friend class Window;

};// class WindowControl


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef WindowControl window_control_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::WindowControl NWindowControl;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::WindowControl ntk_window_control;
	#endif
#endif


#endif//EOH
