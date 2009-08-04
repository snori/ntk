/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: view.cpp,v 1.10 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/view.h"


// WM_MOUSEWHEEL
#define _WIN32_WINNT 0x0500

#include <assert.h>
#include <algorithm>
#include <ntk/windows/windows.h>
#include <ntk/application/application.h>
#include <ntk/interface/window.h>
#include <ntk/interface/panel.h>
#include <ntk/interface/scrollbar.h>
#include <ntk/support/debug.h>


namespace ntk {


//########################################################


enum {
	INVALIDATE_ALL,
	INVALIDATE_RECT,
	INVALIDATE_REGION,
};// invalidation type

Window* dummy_window(bool auto_instantiation = true);// -> <ntk/Window.h>


//########################################################
// public class methods


String
View::window_class()
{
	static const String class_name("ntk-view");

	static bool registered = false;
	if(!registered)
	{
		registered = true;

		WNDCLASSEX wc;
		wc.cbSize        = sizeof wc;
		wc.hInstance     = Application::handle();

		wc.lpszClassName = class_name.c_str();
		wc.lpfnWndProc   = Wnd::window_proc;

		wc.style         = CS_DBLCLKS;
		wc.lpszMenuName  = NULL;
		wc.hIcon         = NULL;
		wc.hIconSm       = NULL;
		wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = background_brush().handle();

		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = sizeof(Wnd*);

		if(RegisterClassEx(&wc) == 0)
		{
			error("ウィンドウクラスの登録に失敗しました。");
			return "";// ウィンドウクラスの名前としては無効な文字列を返す事によって CreateWindow は失敗する
		}
	}

	return class_name;
}


//########################################################
// public methods


View::View(const Rect& frame, const String& name_, uint mode_, uint flags_, const RGBColor& color, uint style, uint ex_style, HMENU hmenu)
:	Handler(name_)
,	Wnd(VIEW_TYPE, window_class(), frame, name_, style, ex_style, dummy_window()->hwnd(), hmenu)
{
	initialize_(mode_, flags_, color);
}

View::View(const String& class_name, const Rect& frame, const String& name_, uint mode_, uint flags_, const RGBColor& color, uint style, uint ex_style, HMENU hmenu)
:	Handler(name_)
,	Wnd(VIEW_TYPE, class_name, frame, name_, style, ex_style, dummy_window()->hwnd(), hmenu)
{
	initialize_(mode_, flags_, color);
}

View::View(const Message& archive)
:	Handler(archive)
,	Wnd(archive)
{
	initialize_(
		archive.find_int("View::resizing_mode"),
		archive.find_int("View::flags"),
		archive.find_color("View::view_color"));
}

View::~View()
{
	finalize_();
}

status_t
View::add_child(View* child)
{
	status_t sts = Wnd::add_child(child);

	if(sts.is_valid_without_eval())
	{
		m_window->add_handler(child);
		child->m_window = m_window;
		child->show();
		child->attached_to_window_();
		return sts;
	}
	else return sts;
}

status_t
View::remove_self()
{
	Window* dummy_win = dummy_window();

	// m_parent は直接 Window に所属するときは Window* である。
	if(m_parent == dummy_win)
		return true;

	HWND prev_hwnd = SetParent(hwnd(), dummy_win->hwnd());

	if(this == window()->current_focus())
		window()->m_focus = NULL;

	if(prev_hwnd)
	{
		// 元の所属情報を削除
		looper()->remove_handler(this);

		Wnd::ChildList::iterator it = std::find(
			m_parent->m_children.begin(),
			m_parent->m_children.end(),
			this);
		assert(it != m_parent->m_children.end());
		m_parent->m_children.erase(it);

		// 新しい所属情報の設定
		attach_to_dummy_window_();

		hide();// 一応
		detached_from_window_();
		attached_to_window_(true);// dummy_window へのアタッチなので通知の必要は無い
		return true;
	}
	else return false;
}

void
View::move_to(coord x, coord y)
{
	Rect frame = this->frame();
	Point new_pos(x, y);

	if(resizing_mode() & FOLLOW_H_CENTER) new_pos.x = frame.left;
	if(resizing_mode() & FOLLOW_V_CENTER) new_pos.y = frame.top;

	if(new_pos == frame.left_top())
		return;

	Wnd::move_to(new_pos.x, new_pos.y);
}

void
View::move_by(coord h, coord v)
{
	move_to(frame().left_top() + Point(h, v));
}

void
View::resize_to(coord w, coord h)
{
	Wnd::resize_to(w, h);

	Rect frame = this->frame();
	Point new_pos = frame.left_top();

	if(parent())
	{
		if(resizing_mode() & FOLLOW_H_CENTER)
			new_pos.x = (parent()->frame().width()  - frame.width())  / 2;
		if(resizing_mode() & FOLLOW_V_CENTER)
			new_pos.y = (parent()->frame().height() - frame.height()) / 2;

		if(new_pos != frame.left_top())
			Wnd::move_to(new_pos.x, new_pos.y);
	}
}

void
View::resize_by(coord h, coord v)
{
	resize_to(frame().width() + h, frame().height() + v);
}

void
View::get_mouse(Point* point, uint* buttons)
{
	::ntk::get_mouse(point, buttons);
	convert_from_screen(point);
}

//#define NTK_INVALIDATE_BY_MESSAGE

bool
View::invalidate(bool erase)
{
#ifdef NTK_INVALIDATE_BY_MESSAGE
	if(! window()) return false;

	Message message(INVALIDATE);
	message.add_bool("_erase", erase);
	message.add_int("_type", INVALIDATE_ALL);

	return window()->post_message(message, this);
#else
	assert(hwnd());
	return to_bool(InvalidateRect(hwnd(), NULL, to_BOOL(erase)));
//	return to_bool(RedrawWindow(hwnd(), NULL, NULL,
//		RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW));
#endif
}

bool
View::invalidate_rect(const Rect& update_rect, bool erase)
{
#ifdef NTK_INVALIDATE_BY_MESSAGE
	if(! window()) return false;

	Message message(INVALIDATE);
	message.add_bool("_erase", erase);
	message.add_int("_type", INVALIDATE_RECT);
	message.add_rect("_rect", update_rect);

	return window()->post_message(message, this);
#else
	assert(hwnd());
	return to_bool(InvalidateRect(hwnd(), &update_rect, to_BOOL(erase)));
//	return to_bool(RedrawWindow(hwnd(), &update_rect, NULL,
//		RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW));
#endif
}

bool
View::invalidate_region(const Region& update_region, bool erase)
{
#if NTK_INVALIDATE_BY_MESSAGE
	if(! window()) return false;

	Message message(INVALIDATE);
	message.add_bool("_erase", erase);
	message.add_int("_type", INVALIDATE_REGION);
	message.add_region("_region", update_region);

	return window()->post_message(message, this);
#else
	assert(hwnd());
	return to_bool(InvalidateRgn(hwnd(), update_region.handle(), to_BOOL(erase)));
//	return to_bool(RedrawWindow(hwnd(), NULL, update_region.handle(),
//		RDW_INVALIDATE | RDW_ERASE | RDW_ALLCHILDREN | RDW_UPDATENOW));
#endif
}

bool
View::validate()
{
	assert(hwnd());
	return to_bool(ValidateRect(hwnd(), NULL));
}

bool
View::validate_rect(const Rect& rect)
{
	assert(hwnd());
	return to_bool(ValidateRect(hwnd(), &rect));
}

bool
View::validate_region(const Region& region)
{
	assert(hwnd());
	return to_bool(ValidateRgn(hwnd(), region.handle()));
}

void
View::convert_to_parent(Point* point)
{
	point->x += frame().left;
	point->y += frame().top;
}

Point
View::convert_to_parent(Point point)
{
	convert_to_parent(&point);
	return point;
}

void
View::convert_from_parent(Point* point)
{
	point->x -= frame().left;
	point->y -= frame().top;
}

Point
View::convert_from_parent(Point point)
{
	convert_from_parent(&point);
	return point;
}

void
View::convert_to_screen(Point* point)
{
	assert(hwnd());

	ClientToScreen(hwnd(), point);
}

Point
View::convert_to_screen(Point point)
{
	convert_to_screen(&point);
	return point;
}

void
View::convert_from_screen(Point* point)
{
	assert(hwnd());

	ScreenToClient(hwnd(), point);
}

Point
View::convert_from_screen(Point point)
{
	convert_from_screen(&point);
	return point;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

DC
View::dc() const
{
	assert(hwnd());
	return DC(hwnd());
}

View*
View::parent() const
{
	// 親が Window* だったときは NULL を返したい
	return dynamic_cast<View*>(m_parent);
}

Window*
View::window() const
{
	return m_window == dummy_window()
		? NULL
		: m_window;
}

void
View::set_name(const String& name_)
{
	assert(hwnd());

	Handler::set_name(name_);
	SetWindowText(hwnd(), name().c_str());
}

uint
View::resizing_mode() const
{
	return m_resizing_mode;
}

void
View::set_resizing_mode(uint mode)
{
	m_resizing_mode = mode;

	if(parent())
	{
		Point pos = frame().left_top();
		if(mode & FOLLOW_H_CENTER) pos.x = (parent()->frame().width() - frame().width()) / 2;
		if(mode & FOLLOW_V_CENTER) pos.y = (parent()->frame().height() - frame().height()) / 2;

		if(pos != frame().left_top())
			Wnd::move_to(pos.x, pos.y);
	}
}

uint
View::flags() const
{
	return m_flags;
}

void
View::set_flags(uint flags_)
{
	if(flags_ == m_flags) return;

	m_flags = flags_;

	uint style = GetWindowLong(hwnd(), GWL_STYLE);
	uint ex_style = GetWindowLong(hwnd(), GWL_EXSTYLE);

	// フラグの初期化
	style &= ~WS_TABSTOP;

	// フラグ設定
	if(m_flags & NAVIGABLE)
		style |= WS_TABSTOP;

	SetWindowLong(hwnd(), GWL_STYLE, style);
	SetWindowLong(hwnd(), GWL_EXSTYLE, ex_style);
}

const RGBColor&
View::view_color() const
{
	return m_view_color;
}

void
View::set_view_color(const RGBColor& color)
{
	m_view_color = color;
	invalidate();
}

bool
View::is_focus() const
{
	return m_focus;
}

void
View::make_focus(bool flag)
{
	if(! (flags() & NAVIGABLE)) return;// フォーカスは持たない
	if(window() == NULL) return;// どこかの Window (dummy_window 以外) に所属していないとフォーカスは持たせない

	if(flag)
		SetFocus(hwnd());
	else if(window() && window()->panel())
		SetFocus(window()->panel()->hwnd());// 一時的処置。要再考
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
View::draw(PaintDC& dc)
{
}

void
View::attached_to_window()
{
}

void
View::detached_from_window()
{
}

void
View::char_received(uint char_code, uint repeat_count)
{
}

void
View::key_down(uint key_code, uint repeat_count)
{
}

void
View::key_up(uint key_code, uint repeat_count)
{
}

void
View::mouse_down(const Point& point, uint buttons, bool double_clicked)
{
}

void
View::mouse_up(const Point& point, uint buttons)
{
}

void
View::mouse_moved(const Point& point, uint buttons, uint transit, const Message* data)
{
#if 0
	Point p;

	DC dc(handle());
	SetBkMode(dc, OPAQUE);

	get_mouse(&p, &buttons);
	dc.draw_string(
		format_string(
			"[%d, %d] left %s, middle %s, right %s",
			p.x,
			p.y,
			buttons & LEFT_MOUSE_BUTTON ? "on" : "off",
			buttons & MIDDLE_MOUSE_BUTTON ? "on" : "off",
			buttons & RIGHT_MOUSE_BUTTON ? "on" : "off"),
		Point(10, 10));
#endif
}

void
View::mouse_wheel_changed(const Point& point, uint buttons, int delta)
{
}

void
View::focus_changed(bool state)
{
}

void
View::window_activated(bool state)
{
}

void
View::message_received(const Message& message)
{
	switch(message.what)
	{
	case INVALIDATE:
		switch(message.find_int("_type"))
		{
		case INVALIDATE_ALL:
			InvalidateRect(
				hwnd(),
				NULL,
				to_BOOL(message.find_bool("_erase")));
			break;

		case INVALIDATE_RECT:
			InvalidateRect(
				hwnd(),
				&(message.find_rect("_rect")),
				to_BOOL(message.find_bool("_erase")));
			break;

		case INVALIDATE_REGION:
			InvalidateRgn(
				hwnd(),
				message.find_region("_region").handle(),
				to_BOOL(message.find_bool("_erase")));
			break;
		}
		break;

	default:
		Handler::message_received(message);
	}
}

LRESULT
View::system_message_received(const Message& message)
{
	struct func {
		static inline LRESULT mouse_up(View* view, const Message& message, uint button)
		{
			LRESULT ret = view->Wnd::system_message_received(message);

			view->mouse_up(
				Point((int16)LOWORD(message.lparam()), (int16)HIWORD(message.lparam())),
				button | message.wparam());

			return ret;
		}
	};

	switch(message.what)
	{
	// process mouse messages
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		{
			LRESULT ret = Wnd::system_message_received(message);

			mouse_down(
				Point((int16)LOWORD(message.lparam()), (int16)HIWORD(message.lparam())),
				message.wparam(),
				false);

			return ret;
		}
		break;

	case WM_LBUTTONUP:
		return func::mouse_up(this, message, LEFT_MOUSE_BUTTON);

	case WM_MBUTTONUP:
		return func::mouse_up(this, message, MIDDLE_MOUSE_BUTTON);

	case WM_RBUTTONUP:
		return func::mouse_up(this, message, RIGHT_MOUSE_BUTTON);

	case WM_MOUSEMOVE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			Point mouse_point((int16)LOWORD(message.lparam()), (int16)HIWORD(message.lparam()));

			uint transit;
			if(m_mouse_inside)
			{
				if(bounds().contains(mouse_point))
					transit = INSIDE_VIEW;
				else
				{
					m_mouse_inside = false;
					transit = EXITED_VIEW;
				}
			}
			else
			{
				if(bounds().contains(mouse_point))
				{
					m_mouse_inside = true;
					transit = ENTERED_VIEW;
				}
				else
					transit = OUTSIDE_VIEW;
			}

			if(transit == ENTERED_VIEW && is_capturing() == false)
			{
				// WM_MOUSELEAVE が呼ばれるようにする
				TRACKMOUSEEVENT mouse_event;
				memset(&mouse_event, 0, sizeof(mouse_event));
				mouse_event.cbSize = sizeof(mouse_event);
				mouse_event.dwFlags = TME_LEAVE;
				mouse_event.hwndTrack = hwnd();
				BOOL ret = TrackMouseEvent(&mouse_event);
				if(ret == FALSE)
					status_t(st::SYSTEM_ERROR).show_error();
			}

			mouse_moved(mouse_point, message.wparam(), transit, NULL);

			return ret;
		}
		break;

	case WM_MOUSELEAVE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			if(is_capturing() == false)
			{
				Point point;
				uint buttons;
				get_mouse(&point, &buttons);

				mouse_moved(convert_from_screen(point), buttons, EXITED_VIEW, NULL);
			}

			m_mouse_inside = false;

			return ret;
		}
		break;

	case WM_LBUTTONDBLCLK:
	case WM_MBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
		{
			LRESULT ret = Wnd::system_message_received(message);

			mouse_down(
				Point((int16)LOWORD(message.lparam()), (int16)HIWORD(message.lparam())),
				message.wparam(),
				true);

			return ret;
		}
		break;

	case WM_MOUSEWHEEL:
		{
			LRESULT ret = Wnd::system_message_received(message);

			mouse_wheel_changed(
				Point((int16)LOWORD(message.lparam()), (int16)HIWORD(message.lparam())),
				LOWORD(message.wparam()),
				(int16)HIWORD(message.wparam()));

			return ret;
		}
		break;

	case WM_CHAR:
		{
			LRESULT ret = Wnd::system_message_received(message);

			char_received((uint32)message.wparam(), message.lparam() & 0xff);

			return ret;
		}
		break;

	case WM_KEYDOWN:
		{
			LRESULT ret = Wnd::system_message_received(message);

			key_down(message.wparam(), message.lparam() & 0xff);

			return ret;
		}
		break;

	case WM_KEYUP:
		{
			LRESULT ret = Wnd::system_message_received(message);

			key_up(message.wparam(), message.lparam() & 0xff);

			return ret;
		}
		break;

	// process window messages
	case WM_MOVE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			if(flags() & FRAME_EVENTS)
				frame_moved(frame().left_top());

			return ret;
		}
		break;

	case WM_SIZE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			if(flags() & FRAME_EVENTS)
				frame_resized(frame().width(), frame().height());

			if(flags() & FULL_UPDATE_ON_RESIZE)
				invalidate();

			if(m_v_scroll_bar)
			{
				coord range = frame().height() - parent()->frame().height();
				m_v_scroll_bar->set_range(0, range >= 0 ? range : 0);
			}

			if(m_h_scroll_bar)
			{
				coord range = frame().width() - parent()->frame().width();
				m_h_scroll_bar->set_range(0, range >= 0 ? range : 0);
			}

			return ret;
		}
		break;

	case WM_ERASEBKGND:
		{
			if(m_view_color.alpha > 0)
			{
				DC dc((HDC)message.wparam());

				dc.set_brush(m_view_color);
				dc.fill_rect(bounds());
			}

			return 1;
		}
		break;

	case WM_PAINT:
		{
			if(flags() & WILL_DRAW && window() && window()->is_update_enabled())
			{
				Rect update_rect;

				{
					PaintDC dc(hwnd());
					update_rect = dc.update_rect();

					draw(dc);
				}

				invalidate_children_(update_rect);
			}
			else
				return Wnd::system_message_received(message);
		}
		break;

	case WM_SETFOCUS:
		{
			LRESULT ret = Wnd::system_message_received(message);

			if(flags() & NAVIGABLE)
			{
				m_focus = true;
				if(window()) window()->m_focus = this;

				focus_changed(is_focus());
			}else{
//				View* old_focus = dynamic_cast<View*>(Ntk::get_wnd_from_hwnd((HWND)message.wparam()));
			}

			return ret;
		}
		break;

	case WM_KILLFOCUS:
		{
			LRESULT ret = Wnd::system_message_received(message);

			m_focus = false;

			focus_changed(is_focus());

			return ret;
		}
		break;

	default:
		return Wnd::system_message_received(message);
	}

	return 0;
}

//********************************************************
// private methods

void
View::initialize_(uint mode, uint flags, const RGBColor& color)
{
	m_parent       = NULL;
	m_window       = NULL;
	m_view_color   = color;
	m_focus        = false;
	m_mouse_inside = false;
	m_h_scroll_bar = NULL;
	m_v_scroll_bar = NULL;

	attach_to_dummy_window_();

	set_resizing_mode(mode);
	set_flags(flags);
}

void
View::finalize_()
{
	looper()->remove_handler(this);

	ChildList::iterator it = std::find(
		m_parent->m_children.begin(),
		m_parent->m_children.end(),
		this);
	m_parent->m_children.erase(it);
}

void
View::attached_to_window_(bool to_dummy_window)
{
	View* view;
	for(ChildList::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		view = *it;

		view->m_window->remove_handler(view);
		view->m_window = m_window;
		view->m_window->add_handler(view);

		view->attached_to_window_(to_dummy_window);
	}

	if(! to_dummy_window)
	{
		Rect rect = frame();
		Point pos = rect.left_top();

		if(resizing_mode() & FOLLOW_H_CENTER)
			pos.x = (parent()->frame().width() - rect.width()) / 2;
		if(resizing_mode() & FOLLOW_V_CENTER)
			pos.y = (parent()->frame().height() - rect.height()) / 2;

		if(pos != rect.left_top())
			Wnd::move_to(pos.x, pos.y);

		attached_to_window();
	}
}

void
View::detached_from_window_(bool from_dummy_window)
{
	View* view;
	for(ChildList::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		view = *it;
		view->detached_from_window_(from_dummy_window);
	}

	if(is_focus())
		make_focus(false);

	if(! from_dummy_window)
		detached_from_window();
}

void
View::attach_to_dummy_window_()
{
	Window* dummy_win = dummy_window();

	m_parent = dummy_win;
	m_window = dummy_win;

	dummy_win->add_handler(this);
	dummy_win->Wnd::m_children.push_back(this);
}


//########################################################
}// namespace ntk
