/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: messenger.cpp,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/messenger.h"


#include <assert.h>
#include <ntk/windows/windows.h>
#include <ntk/support/debug.h>
#include <ntk/application/message.h>
#include <ntk/application/handler.h>
#include <ntk/application/looper.h>


namespace ntk {


//########################################################
// public methods

Messenger::Messenger(Handler* target_, Looper* looper_, status_t* result)
:	m_target(NULL),
	m_looper(NULL)
{
	status_t sts = set_target(target_, looper_);
	if(result) *result = sts;
}

Messenger::Messenger(const Messenger& messenger)
{
	*this = messenger;
}

Messenger::~Messenger()
{
}

Messenger&
Messenger::operator=(const Messenger& rhs)
{
	if(&rhs == this) return *this;

	m_target = rhs.m_target;
	m_looper = rhs.m_looper;

	return *this;
}

status_t
Messenger::send_message(uint command, Handler* reply_handler) const
{
	return send_message(Message(command), reply_handler);
}

status_t
Messenger::send_message(const Message& message, Handler* reply_handler) const
{
	if(m_looper == NULL)
		return status_t(st::BAD_VALUE_ERROR, "Messenger::send_message(): ターゲットが不正です");

#ifdef NTK_DEBUG
	if(m_target)
	{
		uint count = m_looper->count_handlers();
		bool found = false;
		for(int i = 0; i<count; ++i)
			if(m_looper->handler_at(i) == m_target)
				found = true;

		if(! found)
		{
			error(m_target->name() + ", " + m_looper->name() + "\n");
			return status_t(st::ERR, "Messenger::send_message(): target ハンドラは looper に属していません");
		}
	}
#endif

	m_looper->post_message(message, m_target, reply_handler);

	return st::OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

Handler*
Messenger::target(Looper** looper_) const
{
	// Looper* Messenger::looper() というアクセッサを作らないのは
	// Control において NView::looper() とかぶるため

	if(looper_)
		*looper_ = m_looper;

	return m_target;
}

status_t
Messenger::set_target(Handler* target_, Looper* looper_)
{
	if(target_ && looper_ && target_->looper() != looper_)
	{
		return st::BAD_VALUE_ERROR;
	}

	m_target = target_;
	m_looper = m_target ? m_target->looper() : looper_;

	return st::OK;
}

bool
Messenger::is_valid() const
{
	return m_looper || (m_target && m_looper);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public operators

bool
operator==(const Messenger& lhs, const Messenger& rhs)
{
	return lhs.m_target == rhs.m_target && lhs.m_looper == rhs.m_looper;
}


//########################################################
}// namespace ntk
