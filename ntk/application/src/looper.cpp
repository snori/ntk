/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: looper.cpp,v 1.8 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/looper.h"


#include <assert.h>
#include <algorithm>
#include <tchar.h>
#include <ntk/windows/windows.h>
#include <ntk/application/applicationdefs.h>
#include <ntk/application/application.h>
#include <ntk/application/messagefilter.h>
#include <ntk/support/debug.h>


namespace ntk {


//########################################################
// public methods

Looper::Looper(const String& name_, int priority)
:	Handler(name_)
,	m_thread_id(0)
,	m_thread_handle(NULL)
,	m_priority(priority)
,	m_running(false)
,	m_own_thread(priority != -0xff)
{
	Handler::m_looper = this;

	// 新しいスレッドを持つ時
	if(m_own_thread)
	{
		status_t status;
		m_thread_handle = spawn_thread(
			_thread_proc,
			name(),
			m_priority,
			this,
			&status,
			&m_thread_id,
			false);
	}
}

Looper::Looper(const Message& archive)
:	Handler(archive)
,	m_thread_id(0)
,	m_thread_handle(NULL)
,	m_running(false)
{
	Handler::m_looper = this;

	m_priority = archive.find_int("Looper::priority");
	m_own_thread = m_priority != -0xff;

	// 新しいスレッドを持つ時
	if(m_own_thread)
	{
		status_t status;
		m_thread_handle = spawn_thread(
			_thread_proc,
			name(),
			m_priority,
			this,
			&status,
			&m_thread_id,
			false);
	}
}

Looper::~Looper()
{
	// スレッドを持たない時は run() でハンドルを複製している
	if(! m_own_thread)
		CloseHandle(m_thread_handle);
}

int
Looper::message_loop()
{
	MSG msg;
	Message* message;
	Looper* looper;
	Handler* target;

	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);

		if(msg.message == NTK_MESSAGE)
		{
			looper = reinterpret_cast<Looper*>(msg.wParam);
			message = reinterpret_cast<Message*>(msg.lParam);
			target = message->target() ? message->target() : this;

			looper->dispatch_message(*message, target);

			// new は NLooper::post_message にて行われる
			delete message;
		}
		else assert(false);// ここには NWindow/NView 関係のメッセージはこないと思う...
	}

	return msg.wParam;
}

void
Looper::dispatch_message(Message& message, Handler* target_)
{
	Handler* target = target_ ? target_ : this;

	FilterResult ret = filter_message(message, &target);
	// フィルタのどれかがメッセージをスキップするよう要求したらメッセージは dispatch しない
	if(ret == MessageFilter::SKIP_MESSAGE)
		return;

	lock();
	push_current_message(&message);

	target->message_received(message);

	pop_current_message();
	unlock();
}

HANDLE
Looper::run()
{
	if(is_running())
	{
		error(_T("Looper::run(): already running...\n"));
		return thread_handle();
	}

	if(m_own_thread)
	{
		resume_thread(thread_handle());
	}else{
		m_thread_id = GetCurrentThreadId();

		bool dup_ret = to_bool(DuplicateHandle(
			GetCurrentProcess(),
			GetCurrentThread(),
			GetCurrentProcess(),
			&m_thread_handle,
			0,
			TRUE,
			DUPLICATE_SAME_ACCESS));
		assert(dup_ret);

		message_loop_();
	}

	return thread_handle();
}

void
Looper::quit()
{
	post_message(QUIT_REQUESTED);
}

bool
Looper::post_message(const Message& msg, Handler* target, Handler* reply_handler)
{
	if(! thread_id()) return false;

	Message* message = new Message(msg);
	message->set_target(target, reply_handler);

	return to_bool(PostThreadMessage(
		thread_id(),
		NTK_MESSAGE,
		reinterpret_cast<WPARAM>(this),
		reinterpret_cast<LPARAM>(message)));
}

bool
Looper::add_handler(Handler* handler)
{
	if(handler == NULL) return false;

	if(handler->looper())
	{
		error("Handler(" + handler->name() + ") は既にほかの Looper に所属しています");
		return false;
	}

#ifdef NTK_DEBUG
	HandlerList::iterator it = std::find(m_handler_list.begin(), m_handler_list.end(), handler);
	if(it != m_handler_list.end()) return false;
#endif

	handler->m_looper = this;
	m_handler_list.push_back(handler);

	return true;
}

bool
Looper::remove_handler(Handler* handler)
{
	if(
		handler == NULL ||
		handler->m_looper == NULL)// どこのルーパーにも所属していない。当然 this にも。
		return false;

	HandlerList::iterator it = std::find(m_handler_list.begin(), m_handler_list.end(), handler);
	if(it == m_handler_list.end()) return false;

	m_handler_list.erase(it);
	handler->m_looper = NULL;

	return true;
}

bool
Looper::lock()
{
	return m_locker.lock();
}

void
Looper::unlock()
{
	m_locker.unlock();
}

void
Looper::push_current_message(const Message* message)
{
	m_current_message_stack.push_back(message);
}

void
Looper::pop_current_message()
{
	if(! m_current_message_stack.empty())
		m_current_message_stack.pop_back();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

HANDLE
Looper::thread_handle() const
{
	return m_thread_handle;
}

ThreadID
Looper::thread_id() const
{
	return m_thread_id;
}

uint
Looper::count_handlers() const
{
	return m_handler_list.size() +1;// +1 は this の分
}

Handler*
Looper::handler_at(int index) const
{
	if(index == 0)
		return looper();// this

	if(index < 0 || index >= count_handlers())
		return NULL;
	else return m_handler_list[index -1];
}

int
Looper::index_of(Handler* handler) const
{
	if(handler == static_cast<const Handler*>(this))
		return 0;

	HandlerList::const_iterator it = std::find(m_handler_list.begin(), m_handler_list.end(), handler);
	if(it == m_handler_list.end())
		return -1;
	else
		return it - m_handler_list.begin() +1;// this の分、一つずらす
}

bool
Looper::is_running() const
{
	return m_running;
}

bool
Looper::is_locked() const
{
	return m_locker.is_locked();
}

const Message*
Looper::current_message() const
{
	return !m_current_message_stack.empty()
		? m_current_message_stack.back()
		: NULL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
Looper::ready_to_run()
{
}

bool
Looper::quit_requested()
{
	return true;
}

void
Looper::message_received(const Message& message)
{
	switch(message.what)
	{
	case QUIT_REQUESTED:
		if(quit_requested())
			PostQuitMessage(0);
		break;

	default:
		Handler::message_received(message);
	}
}

//********************************************************
// protected methods

FilterResult
Looper::filter_message(Message& message, Handler** target)
{
	bool skip = false;

	Handler::FilterList::iterator it;
	for(it = (*target)->m_filter_list.begin(); it != (*target)->m_filter_list.end(); ++it)
	{
		FilterResult ret = (*it)->filter_(message, target);
		if(ret == MessageFilter::DISPATCH_MESSAGE)
			;
		else if(ret == MessageFilter::SKIP_MESSAGE)
			skip = true;
	}

	return skip
		? MessageFilter::SKIP_MESSAGE
		: MessageFilter::DISPATCH_MESSAGE;
}

//********************************************************
// private methods

int
Looper::message_loop_()
{
	ready_to_run();
	return message_loop();
}

//********************************************************
//private class methods

uint
Looper::_thread_proc(void* pointer_to_this)
{
	Looper* self = reinterpret_cast<Looper*>(pointer_to_this);

	self->message_loop_();

	return 0;
}


//########################################################
}// namespace ntk
