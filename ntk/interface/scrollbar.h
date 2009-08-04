/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: scrollbar.h,v 1.7 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_SCROLLBAR_H__
#define __NTK_INTERFACE_SCROLLBAR_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif


namespace ntk {


class ScrollBar : public View {
public:
	//
	// constants
	//
	enum {
		V_SCROLL_BAR_WIDTH = 16,
		H_SCROLL_BAR_HEIGHT = 16,
	};

	enum Orientation {
		VERTICAL,
		HORIZONTAL,
	};

	enum {
		DEFAULT_SCROLL_BAR_SIZE = View::FLAG_LAST << 0,
		LEFT_ALIGN              = View::FLAG_LAST << 1,
		TOP_ALIGN               = View::FLAG_LAST << 2,
		RIGHT_ALIGN             = View::FLAG_LAST << 3,
		BOTTOM_ALIGN            = View::FLAG_LAST << 4,
		FLAG_LAST               = View::FLAG_LAST << 5,
	};

	enum {
		VALUE_CHANGED = '__VC',
	};//messages

	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_RIGHT | FOLLOW_TOP_BOTTOM,
		DEFAULT_FLAGS         = DEFAULT_SCROLL_BAR_SIZE | RIGHT_ALIGN,
	};//default flags

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Orientation orientation_t;
	#endif

	//
	// methods
	//
	NtkExport ScrollBar(
		const Rect& frame,
		const String& name,
		View* target,
		int min_val,
		int max_val,
		Orientation orient = VERTICAL,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = app_color());
	NtkExport virtual ~ScrollBar();

	//
	// accessors
	//
	NtkExport virtual View* target() const;
	NtkExport virtual void set_target(View* target);

	NtkExport virtual int value() const;
	NtkExport virtual void set_value(int val);

	NtkExport virtual int min_value() const;
	NtkExport virtual int max_value() const;
	NtkExport virtual void set_range(int min_val, int max_val);

	NtkExport virtual int small_step() const;
	NtkExport virtual int large_step() const;
	NtkExport virtual void set_step(int small_, int large_);

	NtkExport virtual Orientation orientation() const;

	NtkExport virtual const RGBColor& view_color() const;
	NtkExport virtual void set_view_color(const RGBColor& color);

	//
	// message handlers
	//
	NtkExport virtual void value_changed(int new_value);

	NtkExport virtual void draw(PaintDC& dc);
	NtkExport virtual void attached_to_window();
	NtkExport virtual void frame_resized(coord width, coord height);

	NtkExport virtual void mouse_down(const Point& point, uint buttons, bool dbl);
	NtkExport virtual void mouse_up(const Point& point, uint bttons);
	NtkExport virtual void mouse_moved(const Point& point, uint buttons, uint transit, const Message* data);

	NtkExport virtual void message_received(const Message& message);

private:
	//
	// data
	//
	int m_value;
	View* m_target;
	int m_min_value, m_max_value, m_small_step, m_large_step;
	Orientation m_orientation;
	RGBColor m_view_color;

	bool m_moving_knob;
	Point m_prev_point;
	Rect m_knob_frame, m_minus_arrow_frame, m_plus_arrow_frame;

	void calculate_frames_();

};// class ScrollBar


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ScrollBar scroll_bar_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::ScrollBar NScrollBar;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::ScrollBar ntk_scroll_bar;
	#endif
#endif


#endif//EOH
