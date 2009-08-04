/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: observable.h,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_OBSERVABLE_H__
#define __NTK_SUPPORT_OBSERVABLE_H__


#include <list>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_MESSENGER_H__
#include <ntk/application/messenger.h>
#endif


namespace ntk {


class Handler;
class Message;


class Observable {
public:
	//
	// constants
	//
	enum {
		NOTIFY = '_OBN',
	};// notification messages

	//
	// methods
	//
	NtkExport Observable();
	NtkExport virtual ~Observable();

	NtkExport virtual bool add_observer(const Messenger& observer);
	NtkExport virtual bool remove_observer(const Messenger& observer);
	NtkExport virtual uint count_observers() const;

	NtkExport virtual bool send_message_to_observers(const Message& message, Handler* reply_handler = NULL) const;

private:
	//
	// types
	//
	typedef std::list<Messenger> ObserverList;

	//
	// data
	//
	ObserverList m_observer_list;

};// class Observable


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Observable observable_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Observable NObservable;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Observable ntk_observable;
	#endif
#endif


#endif//EOH
