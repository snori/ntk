/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: clipboard.h,v 1.3 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_CLIPBOARD_H__
#define __NTK_APPLICATION_CLIPBOARD_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_MESSAGE_H__
#include <ntk/application/message.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif

#ifndef __NTK_KERNEL_THREAD_H__
#include <ntk/kernel/thread.h>
#endif

#ifndef __NTK_KERNEL_LOCKER_H__
#include <ntk/kernel/locker.h>
#endif


namespace ntk {


class Clipboard {
public:
	//
	// methods
	//
	NtkExport Clipboard(HWND hwnd);
	NtkExport virtual ~Clipboard();

	NtkExport Message* data();
	NtkExport const Message* data() const;

	NtkExport status_t clear();
	NtkExport status_t commit();
	NtkExport status_t revert();

	NtkExport bool lock();
	NtkExport void unlock();
	NtkExport bool is_locked() const;

	NtkExport String name() const;

private:
	HWND m_hwnd;
	bool m_status;
	Locker m_locker;
	Message m_data;

};// class Clipboard


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Clipboard clipboard_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Clipboard NClipboard;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Clipboard ntk_clipboard;
	#endif
#endif


#endif//EOH
