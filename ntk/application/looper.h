/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: looper.h,v 1.8 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_LOOPER_H__
#define __NTK_APPLICATION_LOOPER_H__


#include <boost/utility.hpp>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_APPLICATION_DEFS_H__
#include <ntk/application/defs.h>
#endif

#ifndef __NTK_APPLICATION_HANDLER_H__
#include <ntk/application/handler.h>
#endif

#ifndef __NTK_APPLICATION_MESSAGEFILTER__
#include <ntk/application/messagefilter.h>
#endif

#ifndef __NTK_KERNEL_THREAD_H__
#include <ntk/kernel/thread.h>
#endif

#ifndef __NTK_KERNEL_LOCKER_H__
#include <ntk/kernel/locker.h>
#endif


namespace ntk {


class Looper : public Handler, public boost::noncopyable {
public:
	//
	// constants
	//
	enum {
		// normal level //
		LOWEST_PRIORITY            = ntk::LOWEST_PRIORITY,
		LOW_PRIORITY               = ntk::LOW_PRIORITY,
		NORMAL_PRIORITY            = ntk::NORMAL_PRIORITY,
		DISPLAY_PRIORITY           = ntk::DISPLAY_PRIORITY,
		URGENT_DISPLAY_PRIORITY    = ntk::URGENT_DISPLAY_PRIORITY,

		// real-time level //
		REAL_TIME_DISPLAY_PRIORITY = ntk::REAL_TIME_DISPLAY_PRIORITY,
		URGENT_PRIORITY            = ntk::URGENT_PRIORITY,
		REAL_TIME_PRIORITY         = ntk::REAL_TIME_PRIORITY,
	};

	//
	// methods
	//
	NtkExport Looper(const String& name, int priority = NORMAL_PRIORITY);
	NtkExport Looper(const Message& archive);
	NtkExport virtual ~Looper();

	NtkExport virtual int message_loop();
	NtkExport virtual void dispatch_message(Message& message, Handler* target);

	NtkExport virtual HANDLE run();
	NtkExport virtual void quit();

	NtkExport virtual bool post_message(const Message& message, Handler* target = NULL, Handler* reply_handler = NULL);
	bool post_message(uint command, Handler* target = NULL, Handler* reply_handler = NULL)
		{return post_message(Message(command), target, reply_handler);}

	NtkExport virtual bool add_handler(Handler* handler);
	NtkExport virtual bool remove_handler(Handler* handler);

	NtkExport virtual bool lock();
	NtkExport virtual void unlock();

	NtkExport void push_current_message(const Message* message);
	NtkExport void pop_current_message();

	//
	// accessors
	//
	NtkExport virtual HANDLE thread_handle() const;
	NtkExport virtual ThreadID thread_id() const;

	NtkExport virtual uint count_handlers() const;
	NtkExport virtual Handler* handler_at(int index) const;
	NtkExport virtual int index_of(Handler* handler) const;

	NtkExport virtual bool is_running() const;
	NtkExport virtual bool is_locked() const;

	NtkExport virtual const Message* current_message() const;

	//
	// message handlers
	//
	NtkExport virtual void ready_to_run();
	NtkExport virtual bool quit_requested();

	NtkExport virtual void message_received(const Message& message);

protected:
	//
	// methods
	//
	FilterResult filter_message(Message& message, Handler** target);

private:
	//
	// types
	//
	typedef std::vector<Handler*> HandlerList;
	typedef std::vector<const Message*> CurrentMessageStack;

	//
	// data
	//
	HandlerList m_handler_list;
	ThreadID m_thread_id;
	HANDLE m_thread_handle;
	int m_priority;
	Locker m_locker;
	bool m_running, m_own_thread;
	CurrentMessageStack m_current_message_stack;

	int message_loop_();

	//
	// class methods
	//
	static uint _thread_proc(void*);

};// class Looper


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Looper looper_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Looper NLooper;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Looper ntk_looper;
	#endif
#endif


#endif//EOH
