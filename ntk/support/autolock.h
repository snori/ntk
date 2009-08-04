/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: autolock.h,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_AUTOLOCK_H__
#define __NTK_SUPPORT_AUTOLOCK_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_KERNEL_LOCKER_H__
#include <ntk/kernel/locker.h>
#endif


namespace ntk {


class Autolock {
public:
	//
	// methods
	//
	Autolock(Locker* locker)
	:	m_locker(locker)
	{
		m_locker->lock();
	}
	Autolock(Locker& locker)
	:	m_locker(&locker)
	{
		m_locker->lock();
	}
	~Autolock()
	{
		m_locker->unlock();
	}

	//
	// accessors
	//
	bool is_locked()
	{
		return m_locker->is_locked();
	}

private:
	//
	// data
	//
	Locker* m_locker;

};// class Autolock


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Autolock auto_lock_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Autolock NAutolock;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Autolock ntk_autolock;
	#endif
#endif


#endif//EOH
