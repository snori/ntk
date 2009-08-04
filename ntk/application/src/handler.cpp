/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: handler.cpp,v 1.4 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/handler.h"


#include <algorithm>
#include <ntk/windows/windows.h>
#include <ntk/support/debug.h>
#include <ntk/application/looper.h>
#include <ntk/application/messagefilter.h>


namespace ntk {


//########################################################

Handler::Handler(const String& name)
:	m_name(name)
,	m_looper(NULL)
{
}

Handler::Handler(const Message& archive)
:	m_looper(NULL)
{
	m_name = archive.find_string("Handler::name");
}

Handler::~Handler()
{
	while(! m_filter_list.empty())
	{
		delete m_filter_list.back();
		m_filter_list.pop_back();
	}
}

void
Handler::add_filter(MessageFilter* filter)
{
	if(
		filter == NULL ||
		filter->m_looper)// ほかに所属している
		return;

	filter->m_looper = m_looper;
	m_filter_list.push_back(filter);
}

bool
Handler::remove_filter(MessageFilter* filter)
{
	FilterList::iterator it = std::find(
		m_filter_list.begin(),
		m_filter_list.end(),
		filter);
	if(it == m_filter_list.end()) return false;// 見つからなかった

	m_filter_list.erase(it);
	filter->m_looper = NULL;

	return true;
}

bool
Handler::lock_looper() const
{
	if(m_looper)
		return m_looper->lock();
	else
		return false;
}

void
Handler::unlock_looper() const
{
	if(m_looper) m_looper->unlock();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

String
Handler::name() const
{
	return m_name;
}

void
Handler::set_name(const String& name_)
{
	m_name = name_;
}

Looper*
Handler::looper() const
{
	return m_looper;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
Handler::message_received(const Message& message)
{
#ifdef NTK_DEBUG
	error("未処理のメッセージがあります: " + message.as_string() + "\n");
#else
	//do nothing...
#endif
}


//########################################################
}// namespace ntk
