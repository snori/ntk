/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: messagequeue.cpp,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/messagequeue.h"


#include <algorithm>
#include <ntk/windows/windows.h>
#include <ntk/kernel/thread.h>
#include <ntk/application/looper.h>


namespace ntk {


//########################################################
// public methods

MessageQueue::MessageQueue()
:	m_sem(1)
{
	m_sem.acquire();
}

MessageQueue::~MessageQueue()
{
	m_sem.release();
}

void
MessageQueue::add_message(Message* message)
{
	if(message == NULL) return;

	m_locker.lock();
	m_queue.push_back(message);
	m_locker.unlock();

	if(m_sem.count() < 0 && m_looper->is_running())
		m_sem.release();
}

void
MessageQueue::remove_message(Message* message)
{
	if(message == NULL) return;

	m_locker.lock();
	{
		Queue::iterator it = std::find(
			m_queue.begin(), m_queue.end(), message);

		if(it != m_queue.end())// 見つかったら消す
			m_queue.erase(it);
	}
	m_locker.unlock();
}

Message*
MessageQueue::next_message()
{
	// 無駄に時間は過ごしません
	m_locker.lock();
	bool empty = m_queue.empty();
	m_locker.unlock();

	if(empty) m_sem.acquire();

	// メッセージは必ずあることが保証されている。
	Message* message;
	m_locker.lock();
	{
		message = m_queue.front();
		m_queue.pop_front();
	}
	m_locker.unlock();

	return message;
}

Message*
MessageQueue::find_message(uint what, int index) const
{
	Message* msg = NULL;
	int count = 0;

	m_locker.lock();
	for(Queue::const_iterator it = m_queue.begin(); it != m_queue.end(); ++it)
		if((*it)->what == what && count++ == index)
			{
				msg = *it;
				break;
			}
	m_locker.unlock();

	return msg;
}

Message*
MessageQueue::find_message(int index) const
{
	Message* msg = NULL;

	if(0 <= index && index < m_queue.size())
	{
		m_locker.lock();
		msg = m_queue[index];
		m_locker.unlock();
	}

	return msg;
}

bool
MessageQueue::lock()
{
	return m_locker.lock();
}

void
MessageQueue::unlock()
{
	m_locker.unlock();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

Looper*
MessageQueue::looper() const
{
	return m_looper;
}

int
MessageQueue::count_messages() const
{
	return m_queue.size();
}

bool
MessageQueue::is_empty() const
{
	return m_queue.empty();
}


//########################################################
}// namespace ntk
