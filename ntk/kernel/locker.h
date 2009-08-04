/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: locker.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_KERNEL_LOCKER_H__
#define __NTK_KERNEL_LOCKER_H__


#include <boost/utility.hpp>

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


class ILocker;

class Locker : public boost::noncopyable {
public:
	//
	// types
	//
	struct status : public st {
		//
		// constants
		//
		enum {
			TIME_OUT_ERROR = NTK_ERROR_CODE(st, 1),
			ERROR_LAST     = TIME_OUT_ERROR,
		};

		//
		// functions
		//
		NtkExport static String default_message(int what);
	};

	typedef Status<status> status_t;

	//
	// constants
	//
	enum LockerType {
		TYPE_CRITICAL_SECTION,
		TYPE_MUTEX,
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef LockerType locker_type_t;
	#endif

	//
	// methods
	//
	NtkExport Locker(LockerType type = TYPE_CRITICAL_SECTION);
	NtkExport Locker(const String& name, LockerType type = TYPE_MUTEX);
	NtkExport ~Locker();

	NtkExport bool lock() const;
	NtkExport status_t lock_with_timeout(bigtime_t time) const;
	NtkExport void unlock() const;

	NtkExport bool is_locked() const;

private:
	//
	// data
	//
	String m_name;
	ILocker* m_locker;

};// class Locker


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Locker locker_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Locker NLocker;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Locker ntk_locker;
	#endif
#endif


#endif//EOH
