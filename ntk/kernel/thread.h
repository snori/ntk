/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: thread.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_KERNEL_THREAD_H__
#define __NTK_KERNEL_THREAD_H__


#ifndef __NTK_WINDOWS_WNIDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


//--------------------------------------------------------
// thread


enum {
	// normal level priority //
	LOWEST_PRIORITY            = THREAD_PRIORITY_LOWEST,
	LOW_PRIORITY               = THREAD_PRIORITY_BELOW_NORMAL,
	NORMAL_PRIORITY            = THREAD_PRIORITY_NORMAL,
	DISPLAY_PRIORITY           = THREAD_PRIORITY_ABOVE_NORMAL,
	URGENT_DISPLAY_PRIORITY    = THREAD_PRIORITY_HIGHEST,

	// real-time level priority //
	REAL_TIME_DISPLAY_PRIORITY = THREAD_PRIORITY_TIME_CRITICAL,
	URGENT_PRIORITY            = THREAD_PRIORITY_TIME_CRITICAL,
	REAL_TIME_PRIORITY         = THREAD_PRIORITY_TIME_CRITICAL,
};// thread priority

enum {
	BAD_THREAD_ID,
	BAD_THREAD_STATE,
};//thread error codes


typedef uint ThreadID;
typedef uint (*ThreadFunc)(void*);


NtkExport HANDLE spawn_thread(
	ThreadFunc func,
	const String& name,
	int priority = NORMAL_PRIORITY,
	void* data = NULL,
	status_t* status = NULL,
	ThreadID* return_id = NULL,
	bool close_handle_flag = true);

NtkExport status_t suspend_thread(HANDLE handle);
NtkExport status_t resume_thread(HANDLE handle);
NtkExport bool kill_thread(HANDLE handle, ulong code = 1);
NtkExport void exit_thread(uint return_value);
NtkExport status_t wait_for_thread(HANDLE handle, uint* return_value = NULL);

NtkExport bool set_thread_priority(HANDLE handle, int priority);

NtkExport void snooze(bigtime_t milliseconds);


class Thread {
public:
	//
	// methods
	//
	NtkExport Thread(
		ThreadFunc func,
		const String& name,
		int priority = NORMAL_PRIORITY,
		void* data = NULL,
		status_t* status = NULL,
		bool close_handle_thread = true);
	NtkExport Thread(
		const String& name,
		int priority = NORMAL_PRIORITY,
		status_t* status = NULL,
		bool close_handle_thread = true);
	NtkExport virtual ~Thread();

	NtkExport virtual uint proc();

	NtkExport status_t run();
	NtkExport status_t suspend();
	NtkExport status_t resume();

	NtkExport void snooze(bigtime_t milliseconds);

	NtkExport status_t wait_for(uint* return_value = NULL);

	//
	// accessors
	//
	NtkExport HANDLE thread_handle() const;
	NtkExport ThreadID thread_id() const;

	NtkExport bool is_suspended() const;

private:
	//
	// class methods
	//
	static uint start_point_(void*);

	//
	// data
	//
	bool m_suspend;
	HANDLE m_thread_handle;
	ThreadID m_thread_id;

};// class Thread


//========================================================
// semaphore

typedef void* SemID;


NtkExport SemID create_sem(uint thread_count, const String& name = "", status_t* status = NULL);
NtkExport status_t delete_sem(SemID sem_id);

NtkExport status_t acquire_sem(SemID sem_id);
NtkExport status_t acquire_sem(SemID sem_id, int count, bigtime_t time_out);
NtkExport status_t release_sem(SemID sem_id);
NtkExport status_t release_sem(SemID sem_id, int count);

NtkExport status_t get_sem_count(SemID sem_id, int* thread_count);


class Semaphore {
public:
	//
	// methods
	//
	NtkExport Semaphore(uint thread_count, const String& name = "", status_t* status = NULL);
	NtkExport ~Semaphore();

	NtkExport status_t acquire();
	NtkExport status_t acquire(int count, bigtime_t time_out);
	NtkExport status_t release();
	NtkExport status_t release(int count);

	//
	// accessors
	//
	NtkExport int count(status_t* status = NULL) const;

private:
	//
	// data
	//
	SemID m_sem;

};// class Semaphore


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ThreadID   thread_id_t;
	typedef ThreadFunc thread_func_t;
	typedef Thread     thread_t;
	typedef SemID      sem_id_t;
	typedef Semaphore  semaphore_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::ThreadID   NThreadID;
	typedef ntk::ThreadFunc NThreadFunc;
	typedef ntk::Thread     NThread;
	typedef ntk::SemID      NSemID;
	typedef ntk::Semaphore  NSemaphore;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::ThreadID   ntk_thread_id;
	typedef ntk::ThreadFunc ntk_thread_func;
	typedef ntk::Thread     ntk_thread;
	typedef ntk::SemID      ntk_sem_id;
	typedef ntk::Semaphore  ntk_semaphore;
	#endif
#endif


#endif//EOH
