/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: locker.cpp,v 1.4 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/kernel/locker.h"


#include <tchar.h>
#include <assert.h>
#include <ntk/windows/windows.h>
#include <ntk/support/debug.h>
//#include <ntk//Thread.h>


namespace ntk {


//########################################################
// functions

String
Locker::status::default_message(int what)
{
	switch(what)
	{
	case TIME_OUT_ERROR:
		#ifdef NTK_LANG_JP
		return _T("Locker: タイムアウトエラーです\n");
		#else
		return _T("Locker: time out error\n");
		#endif

	default:
		return st::default_message(what);
	}
}


//########################################################
// ILocker

class ILocker {
public:
	ILocker() {}
	virtual ~ILocker() {}

	virtual bool lock() =0;
	virtual status_t lock_with_timeout(bigtime_t time) =0;
	virtual void unlock() =0;

	virtual bool is_locked() const =0;
};


//########################################################
// critical section

class CriticalSection : public ILocker {
public:
	//
	// methods
	//
	CriticalSection()
	:	m_lock_count(0)
	{
		InitializeCriticalSection(&m_critical_section);
	}
	~CriticalSection()
	{
		DeleteCriticalSection(&m_critical_section);
	}

	bool lock()
	{
		EnterCriticalSection(&m_critical_section);

		++m_lock_count;

		return true;
	}
	status_t lock_with_timeout(bigtime_t time)// だめだめ
	{
		#ifdef NTK_WINNT
			bigtime_t start_time = GetTickCount();//timeGetTime();

			while(TryEnterCriticalSection(&m_critical_section))
			{
				if(GetTickCount() - start_time > time)//timeGetTime()
					return Locker::status::TIME_OUT_ERROR;

				snooze(1);// 数値は適当
			}

			++m_lock_count;
			return st::OK;
		#else
			time = time;
			return status_t(st::ERR,
				"TryEnterCriticalSection: Windows9x ではサポートされてないようです。");
		#endif
	}
	void unlock()
	{
		--m_lock_count;

		LeaveCriticalSection(&m_critical_section);

		assert(m_lock_count >= 0);
	}
	
	//
	// accessors
	//
	bool is_locked() const
	{
		return m_lock_count > 0;
	}

private:
	//
	// data
	//
	CRITICAL_SECTION m_critical_section;
	int m_lock_count;

};// class CriticalSection


//########################################################
// critical section

class Mutex : public ILocker {
public:
	//
	// methods
	//
	Mutex()
	:	m_lock_count(0)
	{
		m_handle = CreateMutex(NULL, FALSE, NULL);

		if(m_handle == NULL)
			status_t(st::SYSTEM_ERROR).show_error();
	}
	Mutex(const String& name)
	:	m_lock_count(0)
	{
		m_handle = CreateMutex(NULL, FALSE, name.c_str());

		if(m_handle == NULL)
			status_t(st::SYSTEM_ERROR).show_error();
	}
	~Mutex()
	{
		CloseHandle(m_handle);
	}

	bool lock()
	{
		WaitForSingleObject(m_handle, INFINITE);

		++m_lock_count;

		return true;
	}
	status_t lock_with_timeout(bigtime_t time)
	{
		dword ret = WaitForSingleObject(m_handle, (dword)time);

		if(ret == WAIT_OBJECT_0)
			return st::OK;
		else if(ret == WAIT_TIMEOUT)
			return Locker::status::TIME_OUT_ERROR;
		else
			return st::SYSTEM_ERROR;
	}
	void unlock()
	{
		--m_lock_count;

		ReleaseMutex(m_handle);

		assert(m_lock_count >= 0);
	}

	//
	// accessors
	//
	bool is_locked() const
	{
		return m_lock_count > 0;
	}

private:
	//
	// data
	//
	HANDLE m_handle;
	int m_lock_count;

};// class Mutex


//########################################################
// public methods

Locker::Locker(LockerType type)
{
	if(type == TYPE_CRITICAL_SECTION)
	{
		m_locker = new CriticalSection();
	}else{
		m_locker = new Mutex();
	}
}

Locker::Locker(const String& name, LockerType type)
:	m_name(name)
{
	if(type == TYPE_CRITICAL_SECTION)
	{
		m_locker = new CriticalSection();
	}else{
		m_locker = new Mutex(name);
	}
}

Locker::~Locker()
{
	delete m_locker;
}

bool
Locker::lock() const
{
	return m_locker->lock();
}

Locker::status_t
Locker::lock_with_timeout(bigtime_t time) const
{
	return m_locker->lock_with_timeout(time);
}

void
Locker::unlock() const
{
	m_locker->unlock();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

bool
Locker::is_locked() const
{
	return m_locker->is_locked();
}


//########################################################
}// namespace ntk
