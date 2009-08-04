/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: application.cpp,v 1.11 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/application.h"


#include <assert.h>
#include <vector>
#include <algorithm>
#include <tchar.h>
#include <ntk/support/debug.h>
#include <ntk/storage/directory.h>
#include <ntk/application/defs.h>
#include <ntk/application/message.h>
#include <ntk/interface/window.h>
#include <ntk/interface/view.h>
#include <ntk/interface/alert.h>


#define for if(false); else for


namespace ntk {


//########################################################


HINSTANCE      Application::m_handle    = NULL;
int            Application::m_show_mode = 0;
int            Application::m_argc      = 0;
const char_t** Application::m_argv      = NULL;

static Application* g_application_instance = NULL;
extern std::vector<Window*> g_modal_window_stack;


Application*
application()
{
	return g_application_instance;
}

Window* dummy_window(bool auto_instantiation = true);


//########################################################
// public methods

Application::Application(const String& name_, const Version& version)
:	Looper(name_, -0xff)// -0xff: 新しくスレッドは作らない。詳しくは Looper::Looper() を参照のこと
,	m_version(version)
,	m_accelerator_handle(NULL)
{
	// Applicationのインスタンスは一つだけしか認めない
	if(application() != NULL)
	{
		abort("Application クラスのインスタンスは一つだけしか認められていません。\n");
		return;
	}

	g_application_instance = this;
}

Application::~Application()
{
	clean_up_windows_();// ココは main() のホントに最後に呼ばれる
}

extern const char_t* CUSTOM_WND_PROP_NAME;

HANDLE
Application::run()
{
	Message argv_message(ARGV_RECEIVED);
	argv_message.add_int("argc", argc());
	argv_message.add_pointer("argv", (void*)argv());

	post_message(argv_message);

	return Looper::run();
}

int
Application::message_loop()// ここは NWidnow/NView に対するメッセージもくる
{
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
		dispatch_msg(msg);

	return msg.wParam;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

HINSTANCE
Application::handle()
{
	assert(application());

	return m_handle;
}

int
Application::show_mode()
{
	assert(application());

	return m_show_mode;
}

int
Application::argc()
{
	return m_argc;
}

const char_t**
Application::argv()
{
	return m_argv;
}

Directory
Application::directory(status_t* status)
{
	enum{BUF_SIZE = 1024};
	char_t path[BUF_SIZE];

	#ifdef NTK_WINDOWS
		GetModuleFileName(GetModuleHandle(NULL), path, sizeof(path));
	#elif 1
	#elif HAVE_LINUX_PROCFS
		readlink("/proc/self/exe", path, sizeof(path));
	#elif HAVE_FREEBSD_PROCFS
		readlink("/proc/curproc/file", path, sizeof(path));
	#endif

	Directory dir;
	status_t sts_, &sts = status ? *status : sts_;

	// 一番後ろの '\\' を見つける
	for(char_t* ch=path + _tcslen(path) -1; ch >= path; --ch)
	{
		if(*ch == '\\')
		{
			// 見つかったら最後の '\\' を残した文字列を返す
			ch[1] = '\0';

			sts = dir.set_to(path);
			return dir;
		}
	}

	sts.reset(st::ERR, "アプリケーションディレクトリの取得に失敗しました");

	return dir;
}

const Version&
Application::version() const
{
	return m_version;
}

uint
Application::count_windows() const
{
	if(m_window_list.size() == 0)// 0 の時は DummyWindow もない
		return 0;
	else
		return m_window_list.size() -1;// -1: DummyWindow の分
}

Window*
Application::window_at(int index) const
{
	index += 1;// index==0 は DummyWindow

	if(0 <= index && index < m_window_list.size())
	{
		WindowList::const_iterator it = m_window_list.begin();
		std::advance(it, index);
		return *it;
	}
	else return NULL;
}

Window*
Application::find_window(const String& title) const
{
	for(WindowList::const_iterator it = m_window_list.begin(); it != m_window_list.end(); ++it)
		if((*it)->title() == title)
			return *it;

	return NULL;
}

HACCEL
Application::accelerator_handle() const
{
	return m_accelerator_handle;
}

void
Application::set_accelerator_handle(HACCEL haccelerator)
{
	m_accelerator_handle = haccelerator;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handler

void
Application::app_activated(bool state)
{
}

void
Application::refs_received(const Message& message)
{
}

void
Application::argv_received(int argc, const char_t** argv)
{
}

void
Application::about_requested()
{
	String text = application()->name() + " v" + application()->version().as_string();
	Alert* about = new Alert("ABOUT", text, "OK");
	about->go();
}

void
Application::message_received(const Message& message)
{
	switch(message.what)
	{
	case APP_ACTIVATED:
		{break;
			bool state = message.find_bool("state");

			for(WindowList::iterator it = m_window_list.begin(); it != m_window_list.end(); ++it)
			{
				if((*it)->is_floating())
				{
					if(state)
						(*it)->set_flags((*it)->flags() | Window::ALWAYS_ON_TOP);
					else
						(*it)->set_flags((*it)->flags() & ~Window::ALWAYS_ON_TOP);
				}
			}

			for(std::vector<Window*>::iterator it = g_modal_window_stack.begin(); it != g_modal_window_stack.end(); ++it)
			{
				if(state)
					(*it)->set_flags((*it)->flags() | Window::ALWAYS_ON_TOP);
				else
					(*it)->set_flags((*it)->flags() & ~Window::ALWAYS_ON_TOP);
			}
		}
		break;

	case REFS_RECEIVED:
		refs_received(message);
		break;

	case SAVE_REQUESTED:
		break;

	case ARGV_RECEIVED:
		argv_received(message.find_int("argc"), (const char_t**)message.find_pointer("argv"));
		break;

	case ABOUT_REQUESTED:
		about_requested();
		break;

	default:
		Looper::message_received(message);
	}
}

//********************************************************
// protected methods

void
Application::dispatch_msg(MSG& msg)
{
	WNDPROC wnd_proc = NULL;

	Message* message = NULL;
	Looper* looper = NULL;
	Handler* target = NULL;

	Wnd* wnd = NULL;
	View* view = NULL;
	Window* window = NULL;

	if(msg.message == NTK_MESSAGE)// ユーザメッセージ。post_message(...) によってポストされる
	{
		looper = reinterpret_cast<Looper*>(msg.wParam);
		message = reinterpret_cast<Message*>(msg.lParam);

		if(! message)
		{
			error("An error occured at Application::dispatch_msg(MSG&).");
			return;
		}

		if(message->target() == NULL)
			message->set_target(looper, message->reply_handler());
	}
	else// システムメッセージ
	{
		wnd_proc = reinterpret_cast<WNDPROC>(GetWindowLong(msg.hwnd, GWL_WNDPROC));
		if(wnd_proc == Wnd::window_proc)// 普通の NWnd オブジェクトだったら
		{
			wnd = reinterpret_cast<Wnd*>(GetWindowLong(msg.hwnd, 0));
		}else if(wnd_proc == Wnd::custom_window_proc){
			wnd = reinterpret_cast<Wnd*>(GetProp(msg.hwnd, CUSTOM_WND_PROP_NAME));
		}
		// else wnd は NULL である。

		// まだ NWnd オブジェクトは初期化されていない、または NTK のオブジェクトではない
		if(! wnd)
		{
			if(
				m_accelerator_handle == NULL ||
				(m_accelerator_handle && TranslateAccelerator(msg.hwnd, m_accelerator_handle, &msg) == 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return;
		}

		message = new Message(msg);

		if(view = dynamic_cast<View*>(wnd))
		{
			looper = view->m_window;//window() は dummy_window() の時 NULL を返すから m_window を直接参照する
			target = view;
		}else if(window = dynamic_cast<Window*>(wnd)){
			looper = window;
			target = window;
		}

		message->set_target(target, NULL);
	}

//	Nlib::dout() << "-> " << *message << "\n";
	looper->dispatch_message(*message, message->target());

	delete message;// new は NLooper::post_message にて行われる
}

//********************************************************
// private functions

void
Application::clean_up_windows_()
{
	Window* dummy = dummy_window(false);

	if(dummy)
	{
		WindowList::iterator it = std::find(m_window_list.begin(), m_window_list.end(), dummy);
		m_window_list.erase(it);
	}

	while(! m_window_list.empty())
	{
		delete m_window_list.back();
		// 解放処理が複雑なので DestroyWindow(m_window_list.back()->hwnd()) はしてはいけない。
		// あと m_window_list.back().quit(); ここではまずい。なぜなら quit() はPostThreadMessage()
		// をつかうのがすでにメッセージループは抜けてしまっているから。(2002.8.27)
		// リストからの削除は NWindow 自身がする。-> NWindow::_finalize()
	}

	// dummy_window は最後に削除する
	delete dummy;
}


//########################################################
}// namespace ntk
