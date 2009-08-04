/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: window.cpp,v 1.20 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/window.h"


#include <assert.h>
#include <algorithm>
#include <vector>
#include <ntk/windows/windows.h>
#include <ntk/application/application.h>
#include <ntk/application/defs.h>
#include <ntk/interface/panel.h>
#include <ntk/interface/menubar.h>
#include <ntk/interface/screen.h>
#include <ntk/support/debug.h>
#include "defaultwindowcontrol.h"
//#include "WindowTabControl.h"
//#include "WindowTab.h"


namespace ntk {


//########################################################


typedef std::vector<Window*> WindowStack;
WindowStack g_modal_window_stack;


//========================================================
// functions

Window*
dummy_window(bool auto_instantiation = true)
{
	class DummyWindow : public Window {
	public:
		DummyWindow()
		:	Window(NULL, Rect(-20, -20, -10, -10), "DummyWindow", NOBORDER_LOOK, NORMAL_FEEL)
		{
			show();
			hide();// show() が必要なため。表示してすぐ消す。
		}
		~DummyWindow()
		{
		}

		LRESULT system_message_received(const Message& message)
		{
			switch(message.what)
			{
			case WM_ACTIVATEAPP:
				{
					LRESULT ret = Window::system_message_received(message);

					Message msg(APP_ACTIVATED);
					msg.add_bool("state", message.wparam() == TRUE);

					application()->post_message(msg);

					return ret;
				}
				break;

			default:
				return Window::system_message_received(message);
			}
		}
	};

	static DummyWindow* window = NULL;
	if(window == NULL && auto_instantiation)
		window = new DummyWindow();

	return window;
}

static String
register_window_class(const String& title, status_t* status = NULL)
{
	static uint class_count = 0;

	// ウィンドウクラスの名前の形式は "ntk-window:?" (? は何個目のウィンドウクラスかを表す数)
	String class_name = title + ":" + format_string("%d", ++class_count);//"ntk-window"

	WNDCLASSEX wc;
	wc.cbSize					= sizeof wc;
	wc.hInstance			= Application::handle();

	wc.lpszClassName	= class_name.c_str();
	wc.lpfnWndProc		= Wnd::window_proc;

	wc.style					= 0;
	wc.lpszMenuName		= NULL;
	wc.hIcon					= NULL;
	wc.hIconSm				= NULL;
	wc.hCursor				= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));

	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= sizeof(Wnd*);

	if(RegisterClassEx(&wc) == 0)
	{
		error("ウィンドウクラスの登録に失敗しました。");

		status_t sts_, &sts = status ? *status : sts_;
		sts = st::SYSTEM_ERROR;

		return "";// ウィンドウクラスの名前としては無効な文字列を返す事によって CreateWindow は失敗する
	}

	static bool done = false;
	if(! done)
	{
		done = true;
		dummy_window();// Application::m_window_list の一番最初に登録したい
	}

	return class_name;
}

static void
unregister_window_class(const String& class_name)
{
	if(class_name != "")
		UnregisterClass(class_name.c_str(), Application::handle());
}

static uint
window_type_to_look(Window::WindowType type)
{
	switch(type)
	{
	case Window::TITLED_WINDOW:
		return Window::TITLED_LOOK;

	case Window::DOCUMENT_WINDOW:
		return Window::DOCUMENT_LOOK;

	case Window::MODAL_WINDOW:
		return Window::MODAL_LOOK;

	case Window::FLOATING_WINDOW:
		return Window::FLOATING_LOOK;

	case Window::BORDERED_WINDOW:
		return Window::BORDERED_LOOK;

	case Window::NOBORDER_WINDOW:
		return Window::NOBORDER_LOOK;

	default:
		error("不明なウィンドウタイプが指定されました");
		return Window::TITLED_LOOK;
	}
}

static uint
window_type_to_feel(Window::WindowType type)
{
	switch(type)
	{
	case Window::TITLED_WINDOW:
	case Window::DOCUMENT_WINDOW:
		return Window::NORMAL_FEEL;

	case Window::MODAL_WINDOW:
		return Window::MODAL_APP_FEEL;

	case Window::FLOATING_WINDOW:
		return Window::FLOATING_APP_FEEL;

	case Window::BORDERED_WINDOW:
	case Window::NOBORDER_WINDOW:
		return Window::NORMAL_FEEL;

	default:
		error("不明なウィンドウタイプが指定されました");
		return Window::NORMAL_FEEL;
	}
}

//========================================================
// macros

static inline void
convert_to_window(Point* point, const coord& frame_width, const coord& title_bar_height)
{
	point->x += frame_width;
	point->y += frame_width + title_bar_height;
}


//########################################################
// public methods


#ifdef NTK_WINDOW_OWN_THREAD
	uint WINDOW_THREAD_PRIORITY = NORMAL_PRIORITY;
#else
	uint WINDOW_THREAD_PRIORITY = -0xff;// -0xff: 新しくスレッドは作らない。詳しくは Looper::Looper() を参照のこと
#endif

Window::Window(const Rect& frame_, const String& title_, WindowType type_, uint flags_, uint style, uint ex_style, HMENU hmenu)
:	Looper(title_, WINDOW_THREAD_PRIORITY)
,	Wnd(
		WINDOW_TYPE, register_window_class(title_),
		frame_, title_, style, ex_style, NULL, hmenu)
,	m_window_control(NULL)
,	m_parent(NULL)
,	m_panel(NULL)
{
	initialize_(window_type_to_look(type_), window_type_to_feel(type_), flags_);
	set_panel(new Panel(bounds(), "DefaultPanel"), true);

	move_to(frame_.left, frame_.top);
	resize_to(frame_.width(), frame_.height());
}

Window::Window(Panel* panel_, const Rect& frame_, const String& title_, WindowType type_, uint flags_, uint style, uint ex_style, HMENU hmenu)
:	Looper(title_, WINDOW_THREAD_PRIORITY)
,	Wnd(
		WINDOW_TYPE, register_window_class(title_),
		frame_, title_, style, ex_style, NULL, hmenu)
,	m_window_control(NULL)
,	m_parent(NULL)
,	m_panel(NULL)
{
	initialize_(window_type_to_look(type_), window_type_to_feel(type_), flags_);
	set_panel(panel_, true);

	move_to(frame_.left, frame_.top);
	resize_to(frame_.width(), frame_.height());
}

Window::Window(const Rect& frame_, const String& title_, uint look_, uint feel_, uint flags_, uint style, uint ex_style, HMENU hmenu)
:	Looper(title_, WINDOW_THREAD_PRIORITY)
,	Wnd(
		WINDOW_TYPE, register_window_class(title_),
		frame_, title_, style, ex_style, NULL, hmenu)
,	m_window_control(NULL)
,	m_parent(NULL)
,	m_panel(NULL)
{
	initialize_(look_, feel_, flags_);
	set_panel(new Panel(bounds(), "DefaultPanel"), true);

	move_to(frame_.left, frame_.top);
	resize_to(frame_.width(), frame_.height());
}

Window::Window(Panel* panel_, const Rect& frame_, const String& title_, uint look_, uint feel_, uint flags_, uint style, uint ex_style, HMENU hmenu)
:	Looper(title_, WINDOW_THREAD_PRIORITY)
,	Wnd(
		WINDOW_TYPE, register_window_class(title_),
		frame_, title_, style, ex_style, NULL, hmenu)
,	m_window_control(NULL)
,	m_parent(NULL)
,	m_panel(NULL)
{
	initialize_(look_, feel_, flags_);
	set_panel(panel_, true);

	move_to(frame_.left, frame_.top);
	resize_to(frame_.width(), frame_.height());
}

Window::Window(const Message& archive)
:	Looper(archive)
,	Wnd(archive)
,	m_window_control(NULL)
,	m_parent(NULL)
,	m_panel(NULL)
{
	initialize_(archive.find_int("Window::look"), archive.find_int("Window::feel"), archive.find_int("Window::flags"));
	set_panel(new Panel(bounds(), "DefaultPanel"), true);

	move_to(window_frame().left, window_frame().top);
	resize_to(window_frame().width(), window_frame().height());
}

Window::~Window()
{
	// Window::destroy_self() を呼ぶ為にここで destroy_self() する。（苦肉の策
	// Wnd::~Wnd() で呼ぶと（この関数がデストラクタのため）Wnd::destroy_self() を呼んでしまう。
	destroy_self();

	// delete はさせない
//	if(hwnd())
//		abort("Window* は delete することは出来ません。\n");

	finalize_();

	unregister_window_class(window_class());
}

void
Window::show()
{
	if(is_running() == false)
		run();

	bool hidden = is_hidden();

	Wnd::show();

	if(
		(hidden != is_hidden()) && // 非表示状態から表示状態に変わったか
		is_modal())
	{
		g_modal_window_stack.push_back(this);
	}
}

void
Window::close()
{
	quit();
}

void
Window::zoom()
{
	post_message(ZOOM);
}

void
Window::zoom(const Point& origin, coord width, coord height)
{
	#if 0
		Rect rect = Screen::frame();

		move_to(0, 0);
		resize_to(rect.width(), rect.height());
	#else
		if(! m_zoom)
		{
			ShowWindow(hwnd(), SW_MAXIMIZE);
			m_zoom = true;
		}else{
			ShowWindow(hwnd(), SW_RESTORE);
			m_zoom = false;
		}
	#endif
}

void
Window::minimize()
{
	post_message(MINIMIZE);
}

status_t
Window::add_child(View* child)
{
	status_t sts;

	if(m_panel)
	{
		return m_panel->add_child(child);
	}
	else if(sts = Wnd::add_child(child))
	{
		add_handler(child);
		child->m_window = this;
		child->show();
		child->attached_to_window_();

		MenuBar* menu_bar = dynamic_cast<MenuBar*>(child);
		if(menu_bar) set_key_menu_bar(menu_bar);

		return sts;
	}
	else return sts;
}

status_t
Window::add_child_directly(View* child)
{
	if(child == NULL)
		return false;

	// add_child 時に panel が有るとそこに add_child してしまう。
	// ここでは Window に直接 add_child したい。
	Panel* p = m_panel;
	m_panel = NULL;// Window::add_child() 参照のこと
	{
		add_child(child);
	}
	m_panel = p;

	return true;
}

status_t
Window::remove_child(View* child)
{
	if(child == NULL)
		return false;

	if(child == m_panel)
	{
		status_t sts = Wnd::remove_child(child);

		m_panel = NULL;

		return sts;
	}
	else if(child->m_parent == this || m_panel == NULL)
	{
		if(child == m_key_menu_bar)
			set_key_menu_bar(NULL);

		return Wnd::remove_child(child);
	}
	else return m_panel->remove_child(child);
}

View*
Window::find_view(const String& view_name) const
{
	Panel* panel_ = panel();
	if(panel_)
		return panel_->find_view(view_name);
	else
		return Wnd::find_view(view_name);
}

void
Window::move_to(coord x, coord y)
{
	coord h_diff = 0, v_diff = 0;

	if(m_window_control)
	{
		Rect client_frame = m_window_control->client_frame();
		Rect win_frame = window_frame();

		h_diff = client_frame.left - win_frame.left;
		v_diff = client_frame.top - win_frame.top;
	}

	Wnd::move_to(x - h_diff, y - v_diff);
}

void
Window::resize_to(coord width, coord height)
{
	coord h_diff = 0, v_diff = 0;

	if(m_window_control)
	{
		Rect client_frame = m_window_control->client_frame();
		Rect win_frame = window_frame();

		h_diff = win_frame.width() - client_frame.width();
		v_diff = win_frame.height() - client_frame.height();
	}

	Wnd::resize_to(width + h_diff, height + v_diff);
}

bool
Window::post_message(const Message& msg, Handler* target, Handler* reply_handler)
{
	if(! thread_id()) return false;

	Message* message = new Message(msg);
	message->set_target(target, reply_handler);

	// Window に対するメッセージのポストで、且つメッセージが WM_xxx の時は hwnd を設定し
	// ::DispatchMessage() が正しく動作するようにする。
	if(target == NULL && message->sys_what() != NTK_MESSAGE)
		message->m_msg.hwnd = hwnd();

	return to_bool(PostThreadMessage(
		thread_id(),
		NTK_MESSAGE,
		reinterpret_cast<WPARAM>(this),
		reinterpret_cast<LPARAM>(message)));
}

void
Window::dispatch_message(Message& message, Handler* target_)
{
	Handler* target = target_ ? target_ : this;

	FilterResult ret = filter_message(message, &target);

	// フィルタのどれかがメッセージをスキップするよう要求したらメッセージは dispatch しない
	if(ret == MessageFilter::SKIP_MESSAGE) return;

	lock();
	push_current_message(&message);

	// user message
	if(message.sys_what() == NTK_MESSAGE)
		target->message_received(message);
	// system message
	else{
		HACCEL haccel = application()->accelerator_handle();
		if(
			haccel == NULL ||
			(haccel && TranslateAccelerator(hwnd(), haccel, &message.m_msg) == 0))
		{
			TranslateMessage(&message.msg());
			DispatchMessage(&message.msg());
		}
	}

	pop_current_message();
	unlock();

	// Window* の場合は delete_self() で自分を解放してしまうと上の pop_current_message() 以降が
	// 呼べなくなってしまうので（自分はすでに解放されているから）、ココで解放する。
	// See also: Window::delete_self()
	if(
		message.sys_what() == NTK_MESSAGE &&
		message.what == QUIT_REQUESTED &&
		target == this)
	{
		delete this;
	}
}

int
Window::message_loop()
{
	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

Rect
Window::frame() const
{
	if(m_window_control)
		return m_window_control->client_frame();
	else
		return window_frame();
}

Rect
Window::bounds() const
{
	if(m_panel)
		return m_panel->bounds();
	else if(m_window_control)
		return m_window_control->client_bounds();
	else
		return window_bounds();
}

Rect
Window::window_frame() const
{
	return Wnd::frame();
}

Rect
Window::window_bounds() const
{
	return Wnd::bounds();
}

void
Window::set_name(const String& name_)
{
	assert(hwnd());

	Handler::set_name(name_);
	SetWindowText(hwnd(), name().c_str());
}

String
Window::title() const
{
	return name();
}

void
Window::set_title(const String& title)
{
	set_name(title);
	InvalidateRect(hwnd(), NULL, to_BOOL(false));
}

uint
Window::look() const
{
	if(m_window_control)
		return m_window_control->look();
	else
		return 0;
}

void
Window::set_look(uint look_)
{
	if(m_window_control)
		m_window_control->set_look(look_);
}

uint
Window::feel() const
{
	if(m_window_control)
		return m_window_control->feel();
	else
		return 0;
}

void
Window::set_feel(uint feel_)
{
	if(m_window_control)
		m_window_control->set_feel(feel_);
}

uint
Window::flags() const
{
	if(m_window_control)
		return m_window_control->flags();
	else
		return 0;
}

void
Window::set_flags(uint flags_)
{
	if(m_window_control)
		m_window_control->set_flags(flags_);
}

Panel*
Window::panel() const
{
	return m_panel;
}

Panel*
Window::set_panel(Panel* new_panel, bool owner)
{
	Panel* old_panel = m_panel;
	m_panel = NULL;// add_child()/bounds() のため。Window::add_child()/bounds()参照

	if(old_panel)
		remove_child(old_panel);

	// 位置を調整し子に加える
	if(new_panel)
	{
		add_child(new_panel);

		if(new_panel->flags() == Panel::COVER_WINDOW)
		{
			Rect rect = bounds();
			new_panel->move_to(rect.left_top());
			new_panel->resize_to(rect.width(), rect.height());
		}

		if(m_focus == NULL)
			m_focus = new_panel;
	}

	// old panel の管理権が自分にあった時は解放も行う
	if(m_panel_owner && old_panel)
	{
		delete old_panel;
		old_panel = NULL;
	}

	m_panel = new_panel;
	m_panel_owner = owner;

	return old_panel;
}

bool
Window::is_active() const
{
	return m_active;
}

void
Window::activate(bool flag)
{
	assert(flag == true);// どうにかすべし

	if(flag == m_active) return;

	if(thread_id() == GetCurrentThreadId())
		SetActiveWindow(hwnd());
	else
		post_message(ACTIVATE_WINDOW);
}

bool
Window::is_zoomed() const
{
	return m_zoom;
}

bool
Window::is_minimized() const
{
	return m_minimize;
}

View*
Window::current_focus() const
{
	return m_focus;
}

MenuBar*
Window::key_menu_bar() const
{
	if(m_key_menu_bar)
		return m_key_menu_bar;
	else if(m_panel)
		return m_panel->key_menu_bar();
	else
		return NULL;
}

void
Window::set_key_menu_bar(MenuBar* menu_bar)
{
	m_key_menu_bar = menu_bar;

	if(m_key_menu_bar)
	{
		m_key_menu_bar->move_to(0, 0);
		m_key_menu_bar->resize_to(frame().width(), m_key_menu_bar->frame().height());
	}
}

void
Window::enable_updates()
{
	m_update = true;
}

void
Window::disable_updates()
{
	m_update = false;
}

bool
Window::is_update_enabled() const
{
	return m_update;
}

static const uint ICON_TYPE[] = {ICON_BIG, ICON_SMALL};

HICON
Window::icon_handle(uint type) const
{
	return (HICON)SendMessage(hwnd(), WM_GETICON, ICON_TYPE[type], 0);
}

void
Window::set_icon_handle(HICON icon, uint type)
{
	SendMessage(hwnd(), WM_SETICON, ICON_TYPE[type], (LPARAM)icon);
}

HMENU
Window::menu_handle() const
{
	return GetMenu(hwnd());
}

void
Window::set_menu_handle(HMENU hmenu)
{
	SetMenu(hwnd(), hmenu);
	check_menu_height();
}

coord
Window::menu_height() const
{
	if(GetMenu(hwnd()) == NULL)
		return 0;

	coord border_width   = GetSystemMetrics(SM_CXFRAME);
	coord border_height  = GetSystemMetrics(SM_CYFRAME);
	coord caption_height = GetSystemMetrics(SM_CYCAPTION);

	Rect window_r, client_r;
	GetWindowRect(hwnd(), &window_r);
	GetClientRect(hwnd(), &client_r);

	return window_r.height() - (client_r.height() + border_height * 2 + caption_height);
}

WindowControl*
Window::window_control() const
{
	return m_window_control;
}

void
Window::set_window_control(WindowControl* window_control)
{
	if(window_control == NULL)
	{
		status_t(st::BAD_VALUE_ERROR).show_error();
		return;
	}

	if(m_window_control)
	{
		m_window_control->detached_from_window();
		m_window_control->m_window = NULL;
	}

	delete m_window_control;
	m_window_control = window_control;

	if(m_window_control)
	{
		m_window_control->m_window = this;
		m_window_control->attached_to_window();
	}
}

bool
Window::is_modal() const
{
	uint val = feel();
	return
		val == MODAL_SUBSET_FEEL ||
		val == MODAL_APP_FEEL ||
		val == MODAL_ALL_FEEL;
}

bool
Window::is_floating() const
{
	uint val = feel();
	return
		val == FLOATING_SUBSET_FEEL ||
		val == FLOATING_APP_FEEL ||
		val == FLOATING_ALL_FEEL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
Window::window_activated(bool state)
{
}

void
Window::message_received(const Message& message)
{
	switch(message.what)
	{
	case ACTIVATE_WINDOW:
		SetActiveWindow(hwnd());
		break;

	case ZOOM:
		zoom(Point(0, 0), Screen::frame().width(), Screen::frame().height());
		break;

	case MINIMIZE:
		if(! m_minimize)
		{
			ShowWindow(hwnd(), SW_MINIMIZE);
			m_minimize = true;
		}else{
			ShowWindow(hwnd(), SW_RESTORE);
			m_minimize = false;
		}
		break;

	case QUIT_REQUESTED:
		SendMessage(hwnd(), WM_CLOSE, 0, 0);
		break;

	default:
		Looper::message_received(message);
	}
}

LRESULT
Window::system_command_received(uint command_id)
{
	return 1;
}

LRESULT
Window::system_message_received(const Message& message)
{
	switch(message.what)
	{
	case WM_COMMAND:
		return system_command_received(LOWORD(message.wparam()));

	case WM_SYSCOMMAND:
		switch(message.wparam() & 0xfff0)
		{
		case SC_MAXIMIZE:
			zoom();
			break;

		case SC_MINIMIZE:
			minimize();
			break;

		case SC_RESTORE:
			if(is_minimized())
				minimize();
			else if(is_zoomed())
				zoom();
			break;

		default:
			return Wnd::system_message_received(message);
		}
		break;

	case WM_ACTIVATEAPP:break;
		if(is_modal() || is_floating())
		{
			if(message.wparam() == TRUE)
				set_flags(flags() | ALWAYS_ON_TOP);
			else
				set_flags(flags() & ~ALWAYS_ON_TOP);
		}
		break;

	case WM_ACTIVATE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			window_activated_(LOWORD(message.wparam()) != WA_INACTIVE);

			return ret;
		}
		break;

	case WM_CLOSE:
		if(quit_requested())
			destroy_self();

		return 0;

	case WM_MOVE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			frame_moved(frame().left_top());

			return ret;
		}
		break;

	case WM_SIZE:
		{
			LRESULT ret = Wnd::system_message_received(message);

			switch(message.wparam())
			{
			case SIZE_MAXIMIZED:
				m_zoom = true;
				break;

			case SIZE_MINIMIZED:
				m_minimize = true;
				break;

			case SIZE_RESTORED:
				m_zoom = m_minimize = false;
				break;
			}

			switch(look())
			{
			case TITLED_LOOK:
			case DOCUMENT_LOOK:
			case MODAL_LOOK:
			case FLOATING_LOOK:
			case BORDERED_LOOK:
break;
				Rect win_bounds = window_bounds();
				Region region(win_bounds);

				region -= Region(m_window_control->client_bounds());

				InvalidateRgn(hwnd(), region.handle(), FALSE);// ここら辺そのうち直そう
				break;
			}

			check_menu_height();

			Rect rect = frame();
			frame_resized(rect.width(), rect.height());

			return ret;
		}
		break;

	case WM_CHAR:
		{
			LRESULT ret = Wnd::system_message_received(message);

			View* focus = current_focus();
			if(focus)
			{
				SendMessage(focus->hwnd(), message.msg().message, message.wparam(), message.lparam());
				return 0;
			}
			else return ret;
		}
		break;

	case WM_KEYDOWN:
		{
			LRESULT ret = Wnd::system_message_received(message);

			View* focus = current_focus();
			if(focus)
			{
				SendMessage(focus->hwnd(), message.msg().message, message.wparam(), message.lparam());
				return 0;
			}
			else return ret;
		}
		break;

	case WM_KEYUP:
		{
			LRESULT ret = Wnd::system_message_received(message);

			View* focus = current_focus();
			if(focus)
			{
				SendMessage(focus->hwnd(), message.msg().message, message.wparam(), message.lparam());
				return 0;
			}
			else return ret;
		}
		break;

	case WM_NCHITTEST:
		{
			LRESULT default_val = Wnd::system_message_received(message);

			if(m_window_control)
				return m_window_control->area_test(
					Point(LOWORD(message.lparam()), HIWORD(message.lparam())), default_val);
			else
				return default_val;
		}
		break;

	case WM_ERASEBKGND:
		{
			if(m_panel)
				return 1;
			else
				return Wnd::system_message_received(message);
		}
		break;

	case WM_PAINT:
		{
			if(m_window_control)
			{
				Rect update_rect;

				{
					PaintDC dc(hwnd());
					update_rect = dc.update_rect();

					m_window_control->draw(dc);
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

			if(m_focus)
				m_focus->make_focus();

			return ret;
		}
		break;

	case WM_DESTROY:
		{
			if(feel() == MODAL_SUBSET_FEEL ||
				feel() == MODAL_APP_FEEL ||
				feel() == MODAL_ALL_FEEL)
			{
				assert(g_modal_window_stack.back() == this);
				g_modal_window_stack.pop_back();
			}

			// 最後まで必要な場合があるため。(DerivedWindow::~DerivedWindow() とかで frame() を呼ぶとか。)
			// 解放自体は finalize_() でやる。
//			remove_child(m_title_bar);

			LRESULT ret = Wnd::system_message_received(message);
		}
		break;

	default:
		return Wnd::system_message_received(message);
	}

	return 0;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// [private] message handlers used by system

void
Window::window_activated_(bool state)
{
	m_active = state;

	Wnd::window_activated_(state);
	InvalidateRect(hwnd(), NULL, FALSE);
}

//********************************************************
// protected methods

void
Window::destroy_self()
{
	if(! m_panel_owner && m_panel)
		remove_child(m_panel);

	Wnd::destroy_self();
}

//********************************************************
// private methods

void
Window::initialize_(uint look_, uint feel_, uint flags_)
{
	application()->m_window_list.push_back(this);

	m_window_control   = NULL;
	m_active           = false;
	m_zoom             = false;// OK?
	m_minimize         = false;
	m_focus            = NULL;
	m_parent           = NULL;
	m_panel            = NULL;
	m_panel_owner      = true;
	m_key_menu_bar     = NULL;
	m_update           = true;
	m_prev_menu_height = menu_height();

	set_window_control(new DefaultWindowControl);
	set_flags(flags_);
	set_look(look_);
	set_feel(feel_);
}

void
Window::finalize_()
{
	delete m_window_control;

	Application::WindowList::iterator it = std::find(
		application()->m_window_list.begin(),
		application()->m_window_list.end(),
		this);
	if(it != application()->m_window_list.end())// DummyWindow の場合はリストから削除する必要はない
		application()->m_window_list.erase(it);
}

void
Window::check_menu_height()
{
	coord new_menu_height = menu_height();
	if(new_menu_height != m_prev_menu_height)
	{
		coord dheight = new_menu_height - m_prev_menu_height;
		if(dheight)
			resize_client_by(0, -dheight);

		m_prev_menu_height = new_menu_height;
	}
}

//########################################################
}// namespace ntk


#if 0
// メモ

■ Window の死に様について（ひどい題だ･･･

	new Window(..);

		.
		.
		.

	window->quit();
	|
	+-> Looper::quit(); -> post_message(QUIT_REQUESTED);
	    |
	    Window::message_received(...);
	    |
	    +-> case QUIT_REQUESTED:
	        |
					destroy_self();
					|
					DestroyWindow()

#endif
