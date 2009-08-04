/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: invoker.cpp,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/invoker.h"


#include <ntk/windows/windows.h>
#include <ntk/application/looper.h>
#include <ntk/application/handler.h>
#include <ntk/application/message.h>


namespace ntk {


//########################################################
// public methods

Invoker::Invoker(Message* msg, Handler* target_, Looper* looper_)
:	m_message(NULL),
	m_messenger(target_, looper_),
	m_handler_for_reply(NULL)
{
	set_message(msg);
}

Invoker::Invoker(const Invoker& val)
:	m_message(NULL)
,	m_messenger(NULL)
,	m_handler_for_reply(NULL)
{
	operator=(val);
}

Invoker&
Invoker::operator=(const Invoker& rhs)
{
	if(&rhs == this)
		return *this;

	set_message(new Message(*rhs.m_message));
	set_target(rhs.m_messenger);
	set_handler_for_reply(rhs.m_handler_for_reply);

	return *this;
}

Invoker::~Invoker()
{
	delete m_message;
}

status_t
Invoker::invoke() const
{
	if(m_message == NULL)// 送る必要なし
		return st::OK;

	if(m_messenger.is_valid() == false)
		return status_t(st::ERR, "Invoker::invoke 出来ません");

	return m_messenger.send_message(*m_message, m_handler_for_reply);
}

status_t
Invoker::invoke(const Message& message) const
{
	if(m_messenger.is_valid() == false)
		return status_t(st::ERR, "Invoker::invoke 出来ません");

	return m_messenger.send_message(message, m_handler_for_reply);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

uint
Invoker::command() const
{
	return m_message ? m_message->what : 0;
}

const Message*
Invoker::message() const
{
	return m_message;
}

status_t
Invoker::set_message(Message* msg)
{
	delete m_message;
	m_message = msg;

	return st::OK;
}

const Messenger&
Invoker::messegner() const
{
	return m_messenger;
}

status_t
Invoker::set_target(const Messenger& messenger_)
{
	m_messenger = messenger_;

	return st::OK;
}

Handler*
Invoker::target(Looper** looper_) const
{
	return m_messenger.target(looper_);
}

status_t
Invoker::set_target(Handler* target_, Looper* looper_)
{
	return m_messenger.set_target(target_, looper_);
}

Handler*
Invoker::handler_for_reply() const
{
	return m_handler_for_reply;
}

status_t
Invoker::set_handler_for_reply(Handler* handler_for_reply_)
{
	m_handler_for_reply = handler_for_reply_;
	return st::OK;
}


//########################################################
}// namespace ntk
