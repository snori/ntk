/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: wnd.h,v 1.12 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_WND_H__
#define __NTK_INTERFACE_WND_H__


#include <list>
#include <boost/utility.hpp>

#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_POINT_H__
#include <ntk/interface/point.h>
#endif

#ifndef __NTK_INTERFACE_RECT_H__
#include <ntk/interface/rect.h>
#endif

#ifndef __NTK_APPLICATION_MESSAGE_H__
#include <ntk/application/message.h>
#endif


namespace ntk {


class View;


class Wnd : public boost::noncopyable {
public:
	//
	// constants
	//
	enum {
		//UNKNOWN_TYPE,
		WINDOW_TYPE, VIEW_TYPE,
	};

	//
	// methods
	//
	NtkExport virtual ~Wnd() = 0;

	NtkExport virtual void show();
	NtkExport virtual void hide();

	NtkExport virtual status_t add_child(View* child);
	NtkExport virtual status_t remove_child(View* child);
	NtkExport virtual View* find_view(const String& view_name) const;

	NtkExport virtual void begin_capture();
	NtkExport virtual void end_capture();

	NtkExport virtual void move_to(coord x, coord y);
	void move_to(const Point& point) {move_to(point.x, point.y);}
	NtkExport virtual void move_by(coord horizontal, coord vertical);
	void move_by(const Point& point) {move_by(point.x, point.y);}

	NtkExport virtual void resize_to(coord width, coord height);
	NtkExport virtual void resize_by(coord horizontal, coord vertical);

	NtkExport virtual LRESULT def_window_proc(const Message& message);

	//
	// accessors and manipulators
	//
	NtkExport const String& window_class() const;
	NtkExport HWND hwnd() const;

	NtkExport uint wnd_type() const;

	NtkExport virtual Rect frame() const;
	NtkExport virtual Rect bounds() const;

	NtkExport virtual bool is_hidden() const;

	NtkExport virtual View* child_at(uint index) const;
	NtkExport virtual uint count_children() const;
	NtkExport virtual bool has_child(View* view) const;

	NtkExport virtual bool is_capturing() const;

	NtkExport virtual uint style() const;
	NtkExport virtual uint ex_style() const;
	NtkExport virtual void set_style(uint styles, bool update = false);
	NtkExport virtual void set_ex_style(uint styles, bool update = false);

	//
	// message handlers
	//
	NtkExport virtual void window_activated(bool state);

	NtkExport virtual void frame_moved(const Point& point);
	NtkExport virtual void frame_resized(coord width, coord height);

	NtkExport virtual void lost_capture();

	NtkExport virtual LRESULT system_message_received(const Message& message);

	//
	// [private] message handlers
	//
	NtkExport virtual void window_activated_(bool state);

	//
	// class methods
	//
	static LRESULT CALLBACK window_proc(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK custom_window_proc(HWND, UINT, WPARAM, LPARAM);

protected:
	//
	// methods
	//
	NtkExport Wnd(
		uint wnd_type,
		const String& window_class,
		const Rect& frame,
		const String& name,
		uint style,
		uint ex_style,
		HWND parent,
		HMENU menu);
	NtkExport Wnd(const Message& archive);

	//
	// functions
	//
	NtkExport virtual void destroy_self();
	NtkExport void update_frame();

private:
	//
	// types
	//
	typedef std::list<View*> ChildList;

	//
	// data
	//
	String m_window_class;
	HWND m_hwnd;
	uint m_wnd_type;
	Rect m_frame;
	Point m_size;
	int m_hide_count;

	bool m_capturing;
	bool m_deleting;
	ChildList m_children;

	WNDPROC m_original_window_proc;

	//
	// functions
	//
	void offset_client_by(const Point& d);
	void resize_client_by(coord dw, coord dh);

	void invalidate_children_(const Rect& update_rect);

	LRESULT message_received_(const Message& message);

	void initialize_();
	void finalize_();

	uint process_message_(const Message& message);

	//
	// friends
	//
	friend class Window;
	friend class View;// for m_child_list

};// class Wnd


NtkExport Wnd* get_wnd_from_hwnd(HWND hwnd);


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Wnd wnd_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Wnd NWnd;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Wnd ntk_wnd;
	#endif
#endif


#endif//EOH
