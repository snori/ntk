/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: handler.h,v 1.7 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_HANDLER_H__
#define __NTK_APPLICATION_HANDLER_H__


#include <vector>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_ARCHIVABLE_H__
#include <ntk/support/archivable.h>
#endif

#ifndef __NTK_APPLICATION_MESSAGE_H__
#include <ntk/application/message.h>
#endif


namespace ntk {


class Looper;
class MessageFilter;


class Handler : public Archivable {
public:
	//
	// methods
	//
	NtkExport Handler(const String& name);
	NtkExport Handler(const Message& archive);
	NtkExport virtual ~Handler();

	NtkExport virtual void add_filter(MessageFilter* filter);
	NtkExport virtual bool remove_filter(MessageFilter* filter);

	NtkExport virtual bool lock_looper() const;
	NtkExport virtual void unlock_looper() const;

	//
	// accessors and manipulators
	//
	NtkExport virtual String name() const;
	NtkExport virtual void set_name(const String& name_);

	NtkExport virtual Looper* looper() const;

	//
	// message handlers
	//
	NtkExport virtual void message_received(const Message& message);

private:
	//
	// types
	//
	typedef std::vector<MessageFilter*> FilterList;

	//
	// data
	//
	String m_name;
	Looper* m_looper;
	FilterList m_filter_list;

	//
	// friends
	//
	friend class Looper;

};// class Handler


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Handler handler_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Handler NHandler;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Handler ntk_handler;
	#endif
#endif


#endif//EOH
