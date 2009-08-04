/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: messagequeue.h,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_MESSAGEQUEUE_H__
#define __NTK_APPLICATION_MESSAGEQUEUE_H__


#include <deque>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_KERNEL_THREAD_H__
#include <ntk/kernel/thread.h>
#endif

#ifndef __NTK_KERNEL_LOCKER_H__
#include <ntk/kernel/locker.h>
#endif


namespace ntk {


class Message;
class Looper;

class MessageQueue {
public:
	//
	// methods
	//
	NtkExport MessageQueue();
	NtkExport virtual ~MessageQueue();

	NtkExport virtual void add_message(Message* message);
	NtkExport virtual void remove_message(Message* message);

	NtkExport virtual Message* next_message();
	NtkExport virtual Message* find_message(uint what, int index = 0) const;
	NtkExport virtual Message* find_message(int index) const;

	NtkExport bool lock();
	NtkExport void unlock();

	//
	// accessors and manipulators
	//
	NtkExport Looper* looper() const;

	NtkExport int count_messages() const;
	NtkExport bool is_empty() const;

private:
	//
	// types
	//
	typedef std::deque<Message*> Queue;

	Looper* m_looper;
	Queue m_queue;
	Locker m_locker;
	Semaphore m_sem;

	//
	// friends
	//
	friend class Looper;

};// class MessageQueue


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MessageQueue message_queue_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::MessageQueue NMessageQueue;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::MessageQueue ntk_message_queue;
	#endif
#endif


#endif//EOH
