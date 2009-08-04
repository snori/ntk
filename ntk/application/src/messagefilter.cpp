/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: messagefilter.cpp,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/messagefilter.h"


#include <ntk/windows/windows.h>


namespace ntk {


//########################################################
// public methods

MessageFilter::MessageFilter()
:	m_looper(NULL)
,	m_hook(NULL)
{
}

MessageFilter::MessageFilter(Hook filter)
:	m_looper(NULL)
,	m_hook(filter)
{
}

MessageFilter::~MessageFilter()
{
}

FilterResult
MessageFilter::filter(Message& message, Handler** target)
{
	return DISPATCH_MESSAGE;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

Looper*
MessageFilter::looper() const
{
	return m_looper;
}

//********************************************************
// private methods

FilterResult
MessageFilter::filter_(Message& message, Handler** target)
{
	if(m_hook)
		return m_hook(message, target, this);
	else
		return filter(message, target);
}


//########################################################
}// namespace ntk
