/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: observable.cpp,v 1.3 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/observable.h"


namespace ntk {


//########################################################
// public methods

Observable::Observable()
{
}

Observable::~Observable()
{
}

bool
Observable::add_observer(const Messenger& observer)
{
	ObserverList::iterator it = m_observer_list.begin();
	for(; it != m_observer_list.end(); ++it)
	{
		if(*it == observer) return false;
	}

	m_observer_list.push_back(observer);

	return true;
}

bool
Observable::remove_observer(const Messenger& observer)
{
	ObserverList::iterator it = m_observer_list.begin();
	for(; it != m_observer_list.end(); ++it)
		if(*it == observer)
		{
			m_observer_list.erase(it);
			it = m_observer_list.begin();// ‚à‚¤ˆê“x‰‚ß‚©‚çŒŸõB•¡”‚ ‚Á‚½‚Ì‚½‚ßB
		}

	return true;
}

uint
Observable::count_observers() const
{
	return m_observer_list.size();
}

bool
Observable::send_message_to_observers(const Message& message, Handler* reply_handler) const
{
	bool ret = true;

	ObserverList::const_iterator it = m_observer_list.begin();
	for(; it != m_observer_list.end(); ++it)
	{
		ret = (*it).send_message(message, reply_handler) ? ret : false;
	}

	return ret;
}


//########################################################
}// namespace ntk
