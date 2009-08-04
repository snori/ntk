/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: invoker.h,v 1.6 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_INVOKER_H__
#define __NTK_APPLICATION_INVOKER_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif

#ifndef __NTK_APPLICATION_MESSENGER_H__
#include <ntk/application/messenger.h>
#endif


namespace ntk {


class Handler;
class Looper;
class Message;


class Invoker {
public:
	//
	// methods
	//
	NtkExport Invoker(Message* msg, Handler* target, Looper* looper = NULL);
	NtkExport Invoker(const Invoker& val);
	NtkExport Invoker& operator=(const Invoker& rhs);
	NtkExport virtual ~Invoker();

	NtkExport virtual status_t invoke() const;
	NtkExport virtual status_t invoke(const Message& message) const;

	//
	// accessors and manipulators
	//
	NtkExport virtual uint command() const;
	NtkExport virtual const Message* message() const;
	NtkExport virtual status_t set_message(Message* message);

	NtkExport virtual const Messenger& messegner() const;
	NtkExport virtual status_t set_target(const Messenger& messenger_);

	NtkExport virtual Handler* target(Looper** looper_ = NULL) const;
	NtkExport virtual status_t set_target(Handler* target_, Looper* looper_ = NULL);

	NtkExport virtual Handler* handler_for_reply() const;
	NtkExport virtual status_t set_handler_for_reply(Handler* handler_for_reply_);

private:
	//
	// data
	//
	Message* m_message;
	Messenger m_messenger;
	Handler* m_handler_for_reply;

};// class Invoker


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Invoker invoker_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Invoker NInvoker;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Invoker ntk_invoker;
	#endif
#endif


#endif//EOH
