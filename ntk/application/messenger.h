/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: messenger.h,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_MESSENGER_H__
#define __NTK_APPLICATION_MESSENGER_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class Message;
class Looper;
class Handler;


class Messenger {
public:
	//
	// methods
	//
	NtkExport Messenger(Handler* target, Looper* looper = NULL, status_t* result = NULL);
	NtkExport Messenger(const Messenger& messenger);
	NtkExport virtual ~Messenger();
	NtkExport virtual Messenger& operator=(const Messenger& messenger);

	NtkExport virtual status_t send_message(uint command, Handler* reply_handler = NULL) const;
	NtkExport virtual status_t send_message(const Message& message, Handler* reply_handler = NULL) const;

	//
	// accessors and manipulators
	//
	NtkExport virtual Handler* target(Looper** looper_ = NULL) const;
	NtkExport virtual status_t set_target(Handler* target_, Looper* looper_ = NULL);

	NtkExport virtual bool is_valid() const;

	//
	// operators
	//
	NtkExport friend bool operator==(const Messenger& lhs, const Messenger& rhs);

private:
	//
	// data
	//
	Handler* m_target;
	Looper* m_looper;

};// class Messenger


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Messenger messenger_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Messenger NMessenger;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Messenger ntk_messenger;
	#endif
#endif


#endif//EOH
