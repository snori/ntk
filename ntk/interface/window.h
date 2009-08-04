/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: window.h,v 1.15 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_WINDOW_H__
#define __NTK_INTERFACE_WINDOW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_WND_H__
#include <ntk/interface/wnd.h>
#endif

#ifndef __NTK_APPLICATION_LOOPER_H__
#include <ntk/application/looper.h>
#endif

#ifndef __NTK_INTERFACE_WINDOWCONTROL_H__
#include <ntk/interface/windowcontrol.h>
#endif


namespace ntk {


class DC;
class View;
class Panel;
class MenuBar;
class WindowControlButton;


class Window : public Looper, public Wnd {
public:
	//
	// constants
	//
	enum WindowType {
		TITLED_WINDOW = 0x1,
		DOCUMENT_WINDOW,
		MODAL_WINDOW,
		FLOATING_WINDOW,
		BORDERED_WINDOW,
		NOBORDER_WINDOW,

//		STD_TITLED_WINDOW,
//		STD_POPUP_WINDOW,
//		STD_BORDERED_WINDOW,
	};//window type

	enum {
		TITLED_LOOK = 0x1,
		DOCUMENT_LOOK,
		MODAL_LOOK,
		FLOATING_LOOK,
		BORDERED_LOOK,
		NOBORDER_LOOK,

		STD_LOOK_BEGIN = 0xff,
//		STD_TITLED_LOOK,
//		STD_POPUP_LOOK,
//		STD_BORDERED_LOOK,
	};//window look

	enum {
		NORMAL_FEEL,
		MODAL_SUBSET_FEEL,
		MODAL_APP_FEEL,
		MODAL_ALL_FEEL,
		FLOATING_SUBSET_FEEL,
		FLOATING_APP_FEEL,
		FLOATING_ALL_FEEL,
	};//window feel

	enum {
		NOT_MOVABLE      = 1<<0,
		NOT_H_RESIZABLE  = 1<<1,
		NOT_V_RESIZABLE  = 1<<2,
		NOT_RESIZABLE    = NOT_H_RESIZABLE | NOT_V_RESIZABLE,
		NOT_CLOSABLE     = 1<<3,
		NOT_ZOOMABLE     = 1<<4,
		NOT_MINIMIZABLE  = 1<<5,
		ALWAYS_ON_TOP    = 1<<6,

		MULTIPLE_PANEL   = 1<<7,
		CONTAINER_WINDOW = 1<<8,
		CHILD_WINDOW     = 1<<9,
	};//flags

	enum {
		ACTIVATE_WINDOW = '__AW',
	};//messages

	enum {
		DEFAULT_FLAGS    = 0,
		DEFAULT_STYLE    = WS_CLIPCHILDREN,
		DEFAULT_EX_STYLE = 0,
	};

	enum {
		LARGE_ICON,
		SMALL_ICON
	};//icon type

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef WindowType window_type_t;
	#endif

	//
	// methods
	//
	NtkExport Window(
		const Rect& frame,
		const String& title,
		WindowType type = TITLED_WINDOW,
		uint flags      = DEFAULT_FLAGS,
		uint style      = DEFAULT_STYLE,
		uint ex_style   = DEFAULT_EX_STYLE,
		HMENU hmenu     = NULL);
	NtkExport Window(
		Panel* panel,
		const Rect& frame,
		const String& title,
		WindowType type = TITLED_WINDOW,
		uint flags      = DEFAULT_FLAGS,
		uint style      = DEFAULT_STYLE,
		uint ex_style   = DEFAULT_EX_STYLE,
		HMENU hmenu     = NULL);
	NtkExport Window(
		const Rect& frame,
		const String& title,
		uint look,
		uint feel,
		uint flags    = DEFAULT_FLAGS,
		uint style    = DEFAULT_STYLE,
		uint ex_style = DEFAULT_EX_STYLE,
		HMENU hmenu   = NULL);
	NtkExport Window(
		Panel* panel,
		const Rect& frame,
		const String& title,
		uint look,
		uint feel,
		uint flags    = DEFAULT_FLAGS,
		uint style    = DEFAULT_STYLE,
		uint ex_style = DEFAULT_EX_STYLE,
		HMENU hmenu   = NULL);
	NtkExport Window(const Message& archive);
	NtkExport virtual ~Window();

	NtkExport virtual void show();
	NtkExport virtual void close();

	NtkExport void zoom();
	NtkExport virtual void zoom(const Point& origin, coord width, coord height);
	NtkExport virtual void minimize();

	NtkExport virtual status_t add_child(View* child);
	NtkExport virtual status_t add_child_directly(View* child);
	NtkExport virtual status_t remove_child(View* child);
	NtkExport virtual View* find_view(const String& view_name) const;

	NtkExport virtual void move_to(coord x, coord y);
	void move_to(const Point& point) {move_to(point.x, point.y);}
	NtkExport virtual void resize_to(coord width, coord height);

	NtkExport virtual bool post_message(const Message& message, Handler* target = NULL, Handler* reply_handler = NULL);
	bool post_message(uint command, Handler* target = NULL, Handler* reply_handler = NULL)
		{return post_message(Message(command), target, reply_handler);}

	NtkExport virtual void dispatch_message(Message& message, Handler* target);
	NtkExport virtual int message_loop();

	//
	// accessors and manipulators
	//
	NtkExport virtual Rect frame() const;
	NtkExport virtual Rect bounds() const;
	NtkExport Rect window_frame() const;
	NtkExport Rect window_bounds() const;

	NtkExport virtual void set_name(const String& name_);

	NtkExport virtual String title() const;
	NtkExport virtual void set_title(const String& title);

	NtkExport virtual uint look() const;
	NtkExport virtual uint feel() const;
	NtkExport virtual uint flags() const;
	NtkExport virtual void set_look(uint look);
	NtkExport virtual void set_feel(uint feel);
	NtkExport virtual void set_flags(uint flags);

	NtkExport virtual Panel* panel() const;
	NtkExport virtual Panel* set_panel(Panel* panel, bool owner = true);

	NtkExport virtual bool is_active() const;
	NtkExport virtual void activate(bool flag = true);

	NtkExport virtual bool is_zoomed() const;
	NtkExport virtual bool is_minimized() const;

	NtkExport virtual View* current_focus() const;

	NtkExport virtual MenuBar* key_menu_bar() const;
	NtkExport virtual void set_key_menu_bar(MenuBar* menu_bar);

	NtkExport virtual void enable_updates();
	NtkExport virtual void disable_updates();
	NtkExport virtual bool is_update_enabled() const;

	NtkExport virtual HICON icon_handle(uint type = LARGE_ICON) const;
	NtkExport virtual void set_icon_handle(HICON icon, uint type = LARGE_ICON);

	NtkExport virtual HMENU menu_handle() const;
	NtkExport virtual void set_menu_handle(HMENU hmenu);
	NtkExport virtual coord menu_height() const;

	NtkExport virtual WindowControl* window_control() const;
	NtkExport virtual void set_window_control(WindowControl* window_control);

	NtkExport virtual bool is_modal() const;
	NtkExport virtual bool is_floating() const;

	//
	// message handlers
	//
	NtkExport virtual void window_activated(bool state);
	NtkExport virtual void message_received(const Message& message);

	NtkExport virtual LRESULT system_command_received(uint command_id);
	NtkExport virtual LRESULT system_message_received(const Message& message);

	//
	// [private] message handlers
	//
	NtkExport virtual void window_activated_(bool state);

protected:
	//
	// methods
	//
	NtkExport virtual void destroy_self();

private:
	//
	// data
	//
	WindowControl* m_window_control;
	bool m_active, m_zoom, m_minimize;
	View* m_focus;
	Panel *m_parent, *m_panel;
	bool m_panel_owner;
	MenuBar* m_key_menu_bar;
	bool m_update;
	coord m_prev_menu_height;

	//
	// methods
	//
	void initialize_(uint look, uint feel, uint flags);
	void finalize_();

	void check_menu_height();

	//
	//
	//
	friend class View;// to change 'm_focus'
	friend class Panel;// to change 'm_parent'

};// class Window


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Window window_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Window NWindow;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Window ntk_window;
	#endif
#endif


#endif//EOH
