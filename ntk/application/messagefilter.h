/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: messagefilter.h,v 1.7 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_MESSAGEFILTER_H__
#define __NTK_APPLICATION_MESSAGEFILTER_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


typedef uint FilterResult;


class Message;
class Handler;
class Looper;


class MessageFilter {
public:
	//
	// types
	//
	typedef FilterResult (*Hook)(Message&, Handler**, MessageFilter*);

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef FilterResult filter_result_t;
		typedef Hook hook_t;
	#endif

	//
	// constants
	//
	enum {
		DISPATCH_MESSAGE,
		SKIP_MESSAGE
	};

	//
	// methods
	//
	NtkExport MessageFilter();
	NtkExport MessageFilter(Hook filter);
	NtkExport virtual ~MessageFilter();

	NtkExport virtual FilterResult filter(Message& message, Handler** target);

	//
	// accessors
	//
	NtkExport virtual Looper* looper() const;

private:
	//
	// data
	//
	Looper* m_looper;
	Hook m_hook;

	//
	// methods
	//
	FilterResult filter_(Message& message, Handler** target);

	//
	// friends
	//
	friend class Handler;
	friend class Looper;

};// class MessageFilter


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MessageFilter message_filter_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::MessageFilter NMessageFilter;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::MessageFilter ntk_message_filter;
	#endif
#endif


#endif//EOH
