/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: view.h,v 1.11 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_VIEW_H__
#define __NTK_VIEW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_HANDLER_H__
#include <ntk/application/handler.h>
#endif

#ifndef __NTK_INTERFACE_DEFS_H__
#include <ntk/interface/defs.h>
#endif

#ifndef __NTK_INTERFACE_WND_H__
#include <ntk/interface/wnd.h>
#endif

#ifndef __NTK_INTERFACE_DC_H__
#include <ntk/interface/DC.h>
#endif


namespace ntk {


class Window;
class ScrollBar;


class View : public Handler, public Wnd {
public:
	//
	// constants
	//
	enum {
		FOLLOW_TOP        = 1<<0,
		FOLLOW_LEFT       = 1<<1,
		FOLLOW_RIGHT      = 1<<2,
		FOLLOW_BOTTOM     = 1<<3,

		FOLLOW_H_CENTER   = 1<<4,
		FOLLOW_V_CENTER   = 1<<5,

		FOLLOW_LEFT_RIGHT = FOLLOW_LEFT | FOLLOW_RIGHT,
		FOLLOW_TOP_BOTTOM = FOLLOW_TOP | FOLLOW_BOTTOM,

		FOLLOW_NONE       = FOLLOW_LEFT | FOLLOW_TOP,
		FOLLOW_CENTER     = FOLLOW_H_CENTER | FOLLOW_V_CENTER,

		FOLLOW_ALL_SIDES  = FOLLOW_LEFT_RIGHT | FOLLOW_TOP_BOTTOM
	};// mode

	enum {
		NAVIGABLE             = 1<<0,
		NAVIGABLE_JUMP        = 1<<1,
		WILL_DRAW             = 1<<2,
		FRAME_EVENTS          = 1<<3,
		FULL_UPDATE_ON_RESIZE = 1<<4,

		FLAG_LAST             = FULL_UPDATE_ON_RESIZE << 1,
	};// flags

	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_LEFT | FOLLOW_TOP,
		DEFAULT_FLAGS         = 0,
		DEFAULT_STYLE         = WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
		DEFAULT_EX_STYLE      = 0,
	};

	enum {
		INSIDE_VIEW,
		OUTSIDE_VIEW,
		ENTERED_VIEW,
		EXITED_VIEW
	};

	//
	// class methods
	//
	NtkExport static String window_class();

	//
	// methods
	//
	NtkExport View(
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = ntk::view_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE,
		HMENU hmenu           = NULL);
	NtkExport View(const Message& archive);
	NtkExport virtual ~View();

	NtkExport virtual status_t add_child(View* child);
	NtkExport virtual status_t remove_self();

	NtkExport virtual void move_to(coord x, coord y);
	void move_to(const Point& point) {move_to(point.x, point.y);}
	NtkExport virtual void move_by(coord horizontal, coord vertical);
	void move_by(const Point& point) {move_by(point.x, point.y);}

	NtkExport virtual void resize_to(coord width, coord height);
	NtkExport virtual void resize_by(coord horizontal, coord vertical);

	NtkExport virtual void get_mouse(Point* point, uint* buttons);

	NtkExport bool invalidate(bool erase = true);
	NtkExport bool invalidate_rect(const Rect& rect, bool erase = true);
	NtkExport bool invalidate_region(const Region& update_region, bool erase = true);

	NtkExport bool validate();
	NtkExport bool validate_rect(const Rect& rect);
	NtkExport bool validate_region(const Region& region);

	NtkExport void convert_to_parent(Point* point);
	NtkExport Point convert_to_parent(Point point);
	NtkExport void convert_from_parent(Point* point);
	NtkExport Point convert_from_parent(Point point);

	NtkExport void convert_to_screen(Point* point);
	NtkExport Point convert_to_screen(Point point);
	NtkExport void convert_from_screen(Point* point);
	NtkExport Point convert_from_screen(Point point);

	//
	// accessors and manipulators
	//
	NtkExport virtual DC dc() const;

	NtkExport virtual void set_name(const String& name_);

	NtkExport virtual View* parent() const;
	NtkExport virtual Window* window() const;

	NtkExport virtual uint resizing_mode() const;
	NtkExport virtual void set_resizing_mode(uint mode);

	NtkExport virtual uint flags() const;
	NtkExport virtual void set_flags(uint flags);

	NtkExport virtual const RGBColor& view_color() const;
	NtkExport virtual void set_view_color(const RGBColor& color);

	NtkExport virtual bool is_focus() const;
	NtkExport virtual void make_focus(bool flag = true);

	//
	// message handlers
	//
	NtkExport virtual void draw(PaintDC& dc);

	NtkExport virtual void attached_to_window();
	NtkExport virtual void detached_from_window();

	NtkExport virtual void char_received(uint char_code, uint repea_count);
	NtkExport virtual void key_down(uint key_code, uint repeat_count);
	NtkExport virtual void key_up(uint key_code, uint repeat_count);

	NtkExport virtual void mouse_down(const Point& point, uint buttons, bool double_clicked);
	NtkExport virtual void mouse_up(const Point& point, uint buttons);
	NtkExport virtual void mouse_moved(const Point& point, uint buttons, uint transit, const Message* data);
	NtkExport virtual void mouse_wheel_changed(const Point& point, uint buttons, int delta);

	NtkExport virtual void focus_changed(bool state);

	NtkExport virtual void window_activated(bool state);

	NtkExport virtual void message_received(const Message& message);
	NtkExport virtual LRESULT system_message_received(const Message& message);

protected:
	//
	// methods
	//
	NtkExport View(
		const String& class_name,
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = ntk::view_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE,
		HMENU hmenu           = NULL);

private:
	//
	// data
	//
	Wnd* m_parent;
	Window* m_window;
	uint m_resizing_mode, m_flags;
	RGBColor m_view_color;
	bool m_focus;
	bool m_mouse_inside;

	ScrollBar *m_h_scroll_bar, *m_v_scroll_bar;

	//
	// methods
	//
	void initialize_(uint mode, uint flags, const RGBColor& color);
	void finalize_();

	void attached_to_window_(bool to_dummy_window = false);
	void detached_from_window_(bool from_dummy_window = false);

	void attach_to_dummy_window_();

	//
	// friends
	//
	friend class Application;
	friend class Wnd;
	friend class Window;
	friend class ScrollBar;

};// class View


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef View view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::View NView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::View ntk_view;
	#endif
#endif


#endif//EOH
