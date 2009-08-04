/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: wnd.cpp,v 1.16 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/wnd.h"


#include <assert.h>
#include <vector>
#include <algorithm>
#include <tchar.h>
#include <ntk/windows/windows.h>
#include <ntk/support/autolock.h>
#include <ntk/support/debug.h>
#include <ntk/application/application.h>
#include <ntk/interface/window.h>
#include <ntk/interface/view.h>
#include <ntk/interface/control.h>


namespace ntk {


//########################################################


const char_t* CUSTOM_WND_PROP_NAME = "ntk:wp";

//void _create_window(HWND*, const NString&, const NRect&, const NString&, uint32, uint32, HWND, HMENU, NWnd*);// ntk/tk_main.cpp

Window* dummy_window(bool auto_instantiation = true);// ntk/Window.cpp


//########################################################
// public methods

Wnd::Wnd(uint wnd_type, const String& window_class_, const Rect& frame, const String& name_, uint style, uint ex_style, HWND parent, HMENU menu)
:	m_window_class(window_class_)
,	m_hwnd(NULL)
,	m_wnd_type(wnd_type)
,	m_frame(frame)
,	m_size(m_frame.width(), m_frame.height())
,	m_hide_count(1)// 一度 hide() されているのと同じ
,	m_capturing(false)
,	m_deleting(false)
,	m_original_window_proc(NULL)
{
	m_hwnd = CreateWindowEx(
		ex_style,
		m_window_class.c_str(),
		name_.c_str(),
		style,
		frame.left,
		frame.top,
		frame.width(),
		frame.height(),
		parent,
		menu,
		Application::handle(),
		NULL);

	if(m_hwnd == NULL)
	{
		error("ウィンドウの作成に失敗しました。:" + name_ + "\n");

		status_t(st::SYSTEM_ERROR).show_error();
	}

	initialize_();
}

Wnd::Wnd(const Message& archive)
:	m_hwnd(NULL)
,	m_hide_count(1)// 一度 hide() されているのと同じ
,	m_capturing(false)
,	m_deleting(false)
,	m_original_window_proc(NULL)
{
	m_wnd_type = archive.find_int("Wnd::wnd_type");
	m_window_class = archive.find_string("Wnd::window_class");// window class はまずいよなぁ
	m_frame = archive.find_rect("Wnd::frame");
	m_size.reset(m_frame.width(), m_frame.height());

	m_hwnd = CreateWindowEx(
		archive.find_int("Wnd::ex_style"),
		m_window_class.c_str(),
		archive.find_string("Handler::name").c_str(),
		archive.find_int("Wnd::style"),
		m_frame.left,
		m_frame.top,
		m_frame.width(),
		m_frame.height(),
		NULL,//parent,
		NULL,//menu,
		Application::handle(),
		NULL);

	if(m_hwnd == NULL)
	{
		error("ウィンドウの作成に失敗しました。:" + archive.find_string("Handler::name") + "\n");

		status_t(st::SYSTEM_ERROR).show_error();
	}

	initialize_();
}

Wnd::~Wnd()
{
	// ユーザによって直接 delete が呼ばれたならまだウィンドウは破棄されていない。
	// よってもう delete する必要がないというフラグを立ててからウィンドウを破棄する
	// int Wnd::message_received_(const Message&) 参照のこと
	if(hwnd())
	{
		// 注意！
		// ここにきた時点で ~View() or ~Window() はすでに呼ばれている。

		m_deleting = true;// ←フラグ
		destroy_self();
	}
}

void
Wnd::show()
{
	assert(hwnd());

	// hide() した回数がゼロになったら表示する。
	// 初期状態では一度 hide() した事を示す 1 になっている。コンストラクタ参照
	if(--m_hide_count == 0)
	{
		ShowWindow(hwnd(), SW_SHOW);
		UpdateWindow(hwnd());
	}
}

void
Wnd::hide()
{
	assert(hwnd());

	// 表示されている状態なら非表示に
	if(m_hide_count == 0)
	{
		ShowWindow(hwnd(), SW_HIDE);
		UpdateWindow(hwnd());
	}

	// hide した回数を一回増やす
	++m_hide_count;
}

status_t
Wnd::add_child(View* child)
{
	assert(hwnd());

	if(child == NULL)
		return false;

	// すでにこのウィンドウに所属していたら
	if(child->m_parent == this)
		return true;

	if(child->m_parent != dummy_window())
		return status_t(st::ERR,
			_T("この View(" + child->name() + ") はすでにほかのウィンドウに所属しています。\n"));

	// 親子関係の設定
	HWND hwnd_ = SetParent(child->hwnd(), hwnd());

	if(hwnd_)
	{
		// ※ View::remove_self() 参照 ※

		// 元の所属情報を削除
		child->looper()->remove_handler(child);

		ChildList::iterator it = std::find(
			child->m_parent->m_children.begin(),
			child->m_parent->m_children.end(),
			child);
		child->m_parent->m_children.erase(it);

		child->detached_from_window_(child->m_window == dummy_window());

		// 新しい所属情報を設定
		m_children.push_back(child);

		child->m_parent = this;// Wnd* だから代入できる。View の外から参照するときは parent() を使い、これは dynamic_cast で View* にダウンキャストするので Window* は NULL になる。
//	(Window*)->add_handler(this);// 以下4行は Window/View に任せる
//	child->m_window = ?;
//	child->show();// show も任せるのはここら辺は順番が大切だから
//	child->attached_to_window_();
		return true;
	}
	else return false;
}

status_t
Wnd::remove_child(View* child)
{
	if(child == NULL)
		return false;

	if(child->m_parent != this)// 自分に所属する View じゃなかったら
	{
		char_t buf[256];
		GetWindowText(hwnd(), buf, 256);

		return status_t(st::ERR,
			child->name() + "は" + buf + "の子ではありません。: "
			"Window/View(\"" + buf + "\").remove_child(View(\"" + child->name() + "\"))\n");
	}

	// m_child_listからの削除は child->remove_self() がやってくれる
	return child->remove_self();
}

View*
Wnd::find_view(const String& view_name) const
{
	for(ChildList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
		if((*it)->name() == view_name)
			return *it;

	return NULL;
}

void
Wnd::begin_capture()
{
	assert(hwnd());
	SetCapture(hwnd());
	m_capturing = true;
}

void
Wnd::end_capture()
{
	if(is_capturing())
	{
		m_capturing = false;
		ReleaseCapture();
	}
}

void
Wnd::move_to(coord x, coord y)
{
	assert(hwnd());

	SetWindowPos(hwnd(), NULL, x, y, 0, 0,
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void
Wnd::move_by(coord horizontal, coord vertical)
{
	Rect rect = frame();
	Wnd::move_to(rect.left + horizontal, rect.top + vertical);
}

void
Wnd::resize_to(coord width, coord height)
{
	assert(hwnd());

	SetWindowPos(hwnd(), NULL, 0, 0,
		width >= 0 ? width : 0,
		height >= 0 ? height : 0,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	m_size.x = width;
	m_size.y = height;
}

void
Wnd::resize_by(coord horizontal, coord vertical)
{
	assert(hwnd());

	Rect frame_ = frame();
	coord new_width = 0, new_height = 0;

	m_size.x += horizontal;
	m_size.y += vertical;

	if(m_size.x <= 0)
		new_width = 0;
	else
		new_width = m_size.x;

	if(m_size.y <= 0)
		new_height = 0;
	else
		new_height = m_size.y;

	SetWindowPos(hwnd(), NULL, 0, 0, new_width, new_height,
		SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

LRESULT
Wnd::def_window_proc(const Message& message)
{
	assert(hwnd());

	if(m_original_window_proc)
		return CallWindowProc(m_original_window_proc, hwnd(), message.what, message.wparam(), message.lparam());
	else
		return DefWindowProc(hwnd(), message.what, message.wparam(), message.lparam());
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

const String&
Wnd::window_class() const
{
	return m_window_class;
}

HWND
Wnd::hwnd() const
{
	return m_hwnd;
}

uint
Wnd::wnd_type() const
{
	return m_wnd_type;
}

Rect
Wnd::frame() const
{
	return m_frame;
}

Rect
Wnd::bounds() const
{
	return m_frame.offset_to_copy(0, 0);
}

bool
Wnd::is_hidden() const
{
	return m_hide_count > 0;
}

View*
Wnd::child_at(uint index) const
{
	ChildList::const_iterator it = m_children.begin();
	std::advance(it, index);

	return *it;
}

uint
Wnd::count_children() const
{
	return m_children.size();
}

bool
Wnd::has_child(View* view) const
{
	for(ChildList::const_iterator it = m_children.begin(); it != m_children.end(); ++it)
		if(*it == view || (*it)->has_child(view))
			return true;

	return false;
}

bool
Wnd::is_capturing() const
{
	return m_capturing;
}

/**
 * ウィンドウスタイルの取得
 *
 * @return スタイルを表すフラグ値
 */
uint
Wnd::style() const
{
	return GetWindowLong(hwnd(), GWL_STYLE);
}

/**
 * ウィンドウスタイルの設定
 *
 * @param style 新しいフラグ値
 * @param update フラグの設定後に自動でアップデートするかどうか
 */
void
Wnd::set_style(uint styles, bool update)
{
	SetLastError(0);

	long ret = SetWindowLong(hwnd(), GWL_STYLE, styles);

	if(GetLastError() != 0)
	{
		status_t(st::SYSTEM_ERROR).show_error();
		return;
	}

	if(update)
		update_frame();
}

/**
 * ウィンドウスタイル(EX)の取得
 *
 * @return スタイル(EX)を表すフラグ値
 */
uint
Wnd::ex_style() const
{
	return GetWindowLong(hwnd(), GWL_EXSTYLE);
}

/**
 * ウィンドウスタイル(EX)の設定と取得
 *
 * @param style 新しいフラグ値
 * @param update フラグの設定後に自動でアップデートするかどうか
 */
void
Wnd::set_ex_style(uint styles, bool update)
{
	SetLastError(0);

	SetWindowLong(hwnd(), GWL_EXSTYLE, styles);

	if(GetLastError() != 0)
	{
		status_t(st::SYSTEM_ERROR).show_error();
		return;
	}

	if(update)
		update_frame();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
Wnd::window_activated(bool state)
{
}

void
Wnd::frame_moved(const Point& point)
{
}

void
Wnd::frame_resized(coord width, coord height)
{
}

void
Wnd::lost_capture()
{
}

LRESULT
Wnd::system_message_received(const Message& message)
{
	// 処理するメッセージを増やしたら Window/View ::system_message_received()
	// のそのメッセージ処理している部分で Wnd::system_message_received() を
	// 呼ぶようにすること
	switch(message.what)
	{
	case WM_WINDOWPOSCHANGED:
		{
			WINDOWPOS* pos = reinterpret_cast<WINDOWPOS*>(message.lparam());
			Rect old_frame = m_frame;

			m_frame.reset(pos->x, pos->y, pos->x + pos->cx, pos->y + pos->cy);

			if(m_frame.left != old_frame.left || m_frame.top != old_frame.top)
				offset_client_by(Point(
					m_frame.left - old_frame.left,
					m_frame.top  - old_frame.top));

			if(m_frame.width() != old_frame.width() || m_frame.height() != old_frame.height())
				resize_client_by(
					m_frame.width()  - old_frame.width(),
					m_frame.height() - old_frame.height());

			return def_window_proc(message);
		}
		break;

	case WM_CAPTURECHANGED:
		{
			LRESULT ret = def_window_proc(message);

			m_capturing = false;
			lost_capture();

			return ret;
		}

#if 0
	case WM_SETFOCUS:
		{
			LRESULT ret = def_window_proc(message);

			View* old_focus = dynamic_cast<View*>(get_wnd_from_hwnd(reinterpret_cast<HWND>(message.wparam())));
			if(old_focus)
				old_focus->m_focus = false;

			return ret;
		}
		break;
#endif

	case WM_PAINT:
		{
			Rect update_rect;
			GetUpdateRect(hwnd(), &update_rect, FALSE);

			LRESULT ret = def_window_proc(message);

			invalidate_children_(update_rect);

			return ret;
		}
		break;

	default:
		return def_window_proc(message);
	}

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// [private] message handlers used by system 

void
Wnd::window_activated_(bool state)
{
	window_activated(state);

	ChildList& list = m_children;
	for(ChildList::iterator it = list.begin(); it != list.end(); ++it)
	{
		View* view = *it;
		view->window_activated_(state);
	}
}

//********************************************************
// protected methods

void
Wnd::destroy_self()
{
	DestroyWindow(hwnd());

	// デストラクタにすでにこのウィンドウは
	// 破棄する必要がないことを示す
	m_hwnd = NULL;

	// デストラクタの中から呼ばれたのでなければ (NWindow の場合は QUIT_REQUESTED を受けた時)
	// 自分を解放する。m_deleting はデストラクタの中からしか変更されない。
	// NWindow* のインスタンスは NWindow::dispatch_message() の中で解放される。
	if(m_deleting == false && wnd_type() == VIEW_TYPE)
		delete this;
}

void
Wnd::update_frame()
{
	SetWindowPos(hwnd(), NULL, 0, 0, 0, 0,
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSENDCHANGING);
}

//********************************************************
// private methods

void
Wnd::offset_client_by(const Point& d)
{
#if 0
	ChildList& list = m_children;
	for(ChildList::iterator it = list.begin(); it != list.end(); ++it)
	{
		View* view = *it;
		uint mode = view->resizing_mode();

		if(mode == View::FOLLOW_NONE)
		{
			view->Wnd::move_by(-d.x, -d.y);
		}
	}
#endif
}

void
Wnd::resize_client_by(coord dwidth, coord dheight)
{
	int dx, dy, dw, dh;

	ChildList::iterator it, end = m_children.end();
	for(it = m_children.begin(); it != end; ++it)
	{
		dx = dy = dw = dh = 0;

		View* view = *it;
		uint mode = view->resizing_mode();

		if(mode == View::FOLLOW_ALL_SIDES)
		{
			dw = dwidth;
			dh = dheight;
		}
		else
		{
			// horizontal
			if(mode & View::FOLLOW_H_CENTER)
				dx = (frame().width() - view->frame().width()) / 2 - view->frame().left;
			else if(mode & View::FOLLOW_LEFT && mode & View::FOLLOW_RIGHT)
				dw = dwidth;
			else if(mode & View::FOLLOW_RIGHT)
				dx = dwidth;

			// vertical
			if(mode & View::FOLLOW_V_CENTER)
				dy = (frame().height() - view->frame().height()) / 2 - view->frame().top;
			else if(mode & View::FOLLOW_TOP && mode & View::FOLLOW_BOTTOM)
				dh = dheight;
			else if(mode & View::FOLLOW_BOTTOM)
				dy = dheight;
		}

		// 変更の必要がある時だけ更新する
		if(dx || dy) view->Wnd::move_by(dx, dy);
		if(dw || dh) view->Wnd::resize_by(dw, dh);
	}
}

void
Wnd::invalidate_children_(const Rect& update_rect)
{
#if 0
	if(update_rect.width() <= 0 || update_rect.height() <= 0)
		return;

	for(ChildList::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		const Rect& child_frame = (*it)->frame();
		Rect and_rect = update_rect & child_frame;

		if(and_rect.width() > 0 && and_rect.height() > 0 && (*it)->is_hidden() == false)
			(*it)->invalidate_rect(and_rect.offset_by_self(-child_frame.left_top()));
	}
#else
	for(ChildList::iterator it = m_children.begin(); it != m_children.end(); ++it)
		(*it)->invalidate();
#endif
}


extern std::vector<Window*> g_modal_window_stack;

enum {
	SKIP_MESSAGE,
	PROCESS_MESSAGE
};


LRESULT
Wnd::message_received_(const Message& message)
{
	if(process_message_(message) == SKIP_MESSAGE) return 0;

	LRESULT result = system_message_received(message);

	if(message.what == WM_DESTROY)
	{
		// 子ウィンドウを削除
		while(! m_children.empty())
			delete m_children.back();// リストからの削除は子が自分でする
	}
	else if(message.what == WM_NCDESTROY)
		finalize_();

	return result;
}

void
Wnd::initialize_()
{
	// this と HWND と関連付ける
	if(reinterpret_cast<WNDPROC>(GetWindowLong(m_hwnd, GWL_WNDPROC)) == Wnd::window_proc)
	{
		SetWindowLong(m_hwnd, 0, reinterpret_cast<long>(this));
	}else{
		SetProp(m_hwnd, CUSTOM_WND_PROP_NAME, static_cast<HANDLE>(this));

		m_original_window_proc = reinterpret_cast<WNDPROC>(GetWindowLong(m_hwnd, GWL_WNDPROC));
		SetWindowLong(m_hwnd, GWL_WNDPROC, reinterpret_cast<LONG>(custom_window_proc));
	}
}

void
Wnd::finalize_()
{
	if(m_original_window_proc)
	{
		SetWindowLong(m_hwnd, GWL_WNDPROC, reinterpret_cast<LONG>(m_original_window_proc));
		m_original_window_proc = NULL;

		RemoveProp(m_hwnd, CUSTOM_WND_PROP_NAME);
	}
	else
		SetWindowLong(m_hwnd, 0, NULL);
}

uint
Wnd::process_message_(const Message& message)// WM_* しかこない
{
	if(g_modal_window_stack.size())// modal_window がある時
	{
		if(message.what == WM_ACTIVATE)
			return SKIP_MESSAGE;

		if(// マウスに反応するメッセージなら
			(WM_MOUSEFIRST <= message.what && message.what <= WM_MOUSELAST) ||
			message.what == WM_NCHITTEST)
		{
			Window* current_window = g_modal_window_stack.back();
			if(
				(wnd_type() == WINDOW_TYPE && current_window != dynamic_cast<Window*>(this)) ||     // 自分はウィンドウだが、最前面モーダルウィンドウではない、又は
				(wnd_type() == VIEW_TYPE && current_window != dynamic_cast<View*>(this)->window())) // 自分はビューだが、最前面モーダルウィンドウに所属していない
				return SKIP_MESSAGE; // ならメッセージに反応しない
		}
	}

	// NTK のシステム標準コントロールからのメッセージ。
	// もう一度 invoke() で所属ウィンドウ(target)にメッセージを送りなおす。効率悪いね
	if(message.what == WM_COMMAND && LOWORD(message.wparam()) == SYSTEM_CONTROL)
	{
		Wnd* wnd = get_wnd_from_hwnd(reinterpret_cast<HWND>(message.lparam()));

		Control* control;
		if(wnd && wnd->wnd_type() == Wnd::VIEW_TYPE && (control = dynamic_cast<Control*>(wnd)))
		{
			control->invoke();
			return SKIP_MESSAGE;
		}
	}

	return PROCESS_MESSAGE;
}

//********************************************************
// class methods

LRESULT CALLBACK
Wnd::window_proc(HWND hwnd, UINT sys_what, WPARAM wparam, LPARAM lparam)
{
	Wnd* wnd = reinterpret_cast<Wnd*>(GetWindowLong(hwnd, 0));

	if(wnd)
		return wnd->message_received_(Message(sys_what, wparam, lparam));
	else
		return DefWindowProc(hwnd, sys_what, wparam, lparam);
}

LRESULT CALLBACK
Wnd::custom_window_proc(HWND hwnd, UINT sys_what, WPARAM wparam, LPARAM lparam)
{
	Wnd* wnd = reinterpret_cast<Wnd*>(GetProp(hwnd, CUSTOM_WND_PROP_NAME));

	// wnd のチェックをする必要はないが、WNDCLASS に custom_window_proc を渡しても一応動くように
	if(wnd)
		return wnd->message_received_(Message(sys_what, wparam, lparam));
	else
		return DefWindowProc(hwnd, sys_what, wparam, lparam);
}


//########################################################


Wnd*
get_wnd_from_hwnd(HWND hwnd)
{
	WNDPROC wnd_proc = reinterpret_cast<WNDPROC>(GetWindowLong(hwnd, GWL_WNDPROC));

	if(wnd_proc == Wnd::window_proc)// 普通の NWnd オブジェクトだったら
		return reinterpret_cast<Wnd*>(GetWindowLong(hwnd, 0));
	else if(wnd_proc == Wnd::custom_window_proc)
		return reinterpret_cast<Wnd*>(GetProp(hwnd, CUSTOM_WND_PROP_NAME));
	else
		return NULL;
}


//########################################################
}// namespace ntk
