/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: thread.cpp,v 1.4 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/kernel/thread.h"


#include <process.h>
#include <boost/shared_ptr.hpp>
#include <ntk/kernel/locker.h>
#include <ntk/support/debug.h>
#include <ntk/support/utility.h>


namespace ntk {


//########################################################
// thread

struct ThreadParam {
	HANDLE handle;
	ThreadID id;

	String name;
	ThreadFunc func;
	void* data;

	bool close_handle_flag;
};


static uint WINAPI
thread_start_point_(void* PARAM)
{
	boost::shared_ptr<ThreadParam> param(reinterpret_cast<ThreadParam*>(PARAM));

	uint ret = param->func(param->data);

	if(param->close_handle_flag == true)
		CloseHandle(param->handle);

#ifdef NTK_DEBUG
	error(String("\"") + param->name + "\"");
#endif

	return ret;
}

HANDLE
spawn_thread(ThreadFunc func, const String& name, int priority, void* data, status_t* status, ThreadID* return_id, bool close_handle_flag)
{
	ThreadParam* param = new ThreadParam;
	param->name = name;
	param->func = func;
	param->data = data;
	param->close_handle_flag = close_handle_flag;

	// スレッドの作成
	param->handle = (HANDLE)_beginthreadex(
		NULL,
		0,// default stack size
		thread_start_point_,
		param,
		CREATE_SUSPENDED,
		&param->id);

	// ステータスが要求されない時は一時オブジェクトに入れる。
	// そうするとエラーがあっても、未処理のエラーがあると報告される。
	status_t sts, *psts = status ? status : &sts;

	if(reinterpret_cast<uint>(param->handle) == 0xffffffff)
		*psts = status_t(st::ERR, "スレッドの生成に失敗しました。(" +name+ ")");
	else
		*psts = st::OK;

	// ユーザがスレッドの id を欲していたら設定する
	if(return_id != NULL)
		*return_id = param->id;

	return param->handle;
}

status_t
suspend_thread(HANDLE handle)
{
	dword ret = SuspendThread(handle);

	if(ret == 0xffffffff)
		return st::ERR;

	return st::OK;
}

status_t
resume_thread(HANDLE handle)
{
	dword ret = ResumeThread(handle);

	if(ret == 0xffffffff)
		return st::SYSTEM_ERROR;
	else if(ret == 0)
		return status_t(st::ERR, "resume_thread(): このスレッドはサスペンドされていません");

	return st::OK;
}

bool
kill_thread(HANDLE handle, ulong code)
{
	return to_bool(TerminateThread(handle, code));
}

void
exit_thread(uint return_value)
{
	_endthreadex(return_value);
}

status_t
wait_for_thread(HANDLE handle, uint* return_value)
{
	if(WaitForSingleObject(handle, INFINITE) != WAIT_OBJECT_0)
		return st::SYSTEM_ERROR;

	if(return_value && GetExitCodeThread(handle, reinterpret_cast<dword*>(return_value)) != 0)
		return st::SYSTEM_ERROR;

	return st::OK;
}

bool
set_thread_priority(HANDLE handle, int priority)
{
	return to_bool(SetThreadPriority(handle, priority));
}

void
snooze(bigtime_t milliseconds)//microseconds)
{
	Sleep((dword)milliseconds);//microseconds / 1000);
}


//########################################################
// public methods

Thread::Thread(ThreadFunc func, const String& name, int priority, void* data, status_t* status, bool close_thread_handle)
:	m_suspend(true)
{
	status_t sts;
	m_thread_handle = ::ntk::spawn_thread(
		func, name, priority, data, status,
		&m_thread_id, close_thread_handle);
}

Thread::Thread(const String& name, int priority, status_t* status, bool close_thread_handle)
:	m_suspend(true)
{
	status_t sts;
	m_thread_handle = ::ntk::spawn_thread(
		start_point_, name, priority, this, status,
		&m_thread_id, close_thread_handle);
}

Thread::~Thread()
{
}

uint
Thread::proc()
{
	return 0;
}

status_t
Thread::run()
{
	m_suspend = false;
	return ::ntk::resume_thread(m_thread_handle);
}

status_t
Thread::suspend()
{
	m_suspend = true;
	return ::ntk::suspend_thread(m_thread_handle);
}

status_t
Thread::resume()
{
	m_suspend = false;
	return ::ntk::resume_thread(m_thread_handle);
}

void
Thread::snooze(bigtime_t milliseconds)
{
	::ntk::snooze(milliseconds);
}

status_t
Thread::wait_for(uint* return_value)
{
	return ::ntk::wait_for_thread(m_thread_handle, return_value);
}

//********************************************************
// public accessors

HANDLE
Thread::thread_handle() const
{
	return m_thread_handle;
}

ThreadID
Thread::thread_id() const
{
	return m_thread_id;
}

bool
Thread::is_suspended() const
{
	return m_suspend;
}

//********************************************************
// private class methpds

uint
Thread::start_point_(void* data)
{
	Thread* self = reinterpret_cast<Thread*>(data);
	return self->proc();
}


//########################################################
// semaphore

struct Sem {
	HANDLE handle;
	int thread_count;
	Locker count_locker;
};

SemID
create_sem(uint thread_count, const String& name, status_t* status)
{
	Sem* id = new Sem;
	id->handle = CreateSemaphore(NULL, thread_count, thread_count, name.c_str());
	id->thread_count = thread_count;

	status_t sts, *psts = status ? status : &sts;
	if(id->handle != NULL)
		*psts = st::OK;
	else
		*psts = st::SYSTEM_ERROR;

	return reinterpret_cast<SemID>(id);
}

status_t
delete_sem(SemID sem_id)
{
	Sem* id = reinterpret_cast<Sem*>(sem_id);
	BOOL ret = CloseHandle(id->handle);

	if(ret)
	{
		delete id;
		return st::OK;
	}
	else return st::SYSTEM_ERROR;
}

status_t
acquire_sem(SemID sem_id)
{
	Sem* id = reinterpret_cast<Sem*>(sem_id);

	id->count_locker.lock();
	--(id->thread_count);
	id->count_locker.unlock();

	dword ret = WaitForSingleObject(id->handle, INFINITE);

	if(ret == WAIT_OBJECT_0)
		return st::OK;
	else if(ret == WAIT_FAILED)
		return st::SYSTEM_ERROR;
	else
		return st::ERR;
}

status_t
acquire_sem(SemID sem_id, int count, bigtime_t time_out)
{
	Sem* id = reinterpret_cast<Sem*>(sem_id);

	dword ret;
	while(--count)// count よりも一回少なく
	{
		id->count_locker.lock();
		--(id->thread_count);
		id->count_locker.unlock();

		WaitForSingleObject(id->handle, (dword)time_out);// これでいいのだろうか
	}

	// 残りの一回
	id->count_locker.lock();
	--(id->thread_count);
	id->count_locker.unlock();
	ret = WaitForSingleObject(id->handle, (dword)time_out);

	if(ret == WAIT_OBJECT_0)
		return st::OK;
	else if(ret == WAIT_FAILED)
		return st::SYSTEM_ERROR;
	else
		return st::ERR;
}

status_t
release_sem(SemID sem_id)
{
	Sem* id = reinterpret_cast<Sem*>(sem_id);

	BOOL ret = ReleaseSemaphore(id->handle, 1, NULL);
	id->count_locker.lock();
	++(id->thread_count);
	id->count_locker.unlock();

	return ret ? st::OK : st::SYSTEM_ERROR;
}

status_t
release_sem(SemID sem_id, int count)
{
	Sem* id = reinterpret_cast<Sem*>(sem_id);

	BOOL ret = ReleaseSemaphore(id->handle, count, NULL);
	id->count_locker.lock();
	id->thread_count += count;
	id->count_locker.unlock();

	return ret ? st::OK : st::SYSTEM_ERROR;
}

status_t
get_sem_count(SemID sem_id, int* thread_count)
{
	Sem* id = reinterpret_cast<Sem*>(sem_id);

	if(thread_count)
	{
		id->count_locker.lock();
		*thread_count = id->thread_count;
		id->count_locker.unlock();

		return st::OK;
	}
	else return st::ERR;
}


//########################################################
// public methods

Semaphore::Semaphore(uint thread_count, const String& name, status_t* status)
{
	m_sem = create_sem(thread_count, name, status);
}

Semaphore::~Semaphore()
{
	delete_sem(m_sem);
}

status_t
Semaphore::acquire()
{
	return acquire_sem(m_sem);
}

status_t
Semaphore::acquire(int count, bigtime_t time_out)
{
	return acquire_sem(m_sem, count, time_out);
}

status_t
Semaphore::release()
{
	return release_sem(m_sem);
}

status_t
Semaphore::release(int count)
{
	return release_sem(m_sem);
}

int
Semaphore::count(status_t* status) const
{
	status_t sts;
	status_t* psts = status ? status : &sts;

	int count;
	*psts = get_sem_count(m_sem, &count);

	return count;
}


//########################################################
}// namespace ntk
