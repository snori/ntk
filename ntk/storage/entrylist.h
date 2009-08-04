/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: entrylist.h,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_ENTRYLIST_H__
#define __NTK_STORAGE_ENTRYLIST_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_STORAGE_ENTRY_H__
#include <ntk/storage/entry.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class EntryList {
public:
	//
	// types
	//
	struct status : public st {
		//
		// constants
		//
		enum {
			ENTRY_NOT_FOUND_ERROR = NTK_ERROR_CODE(st, 1),
			ERROR_LAST            = ENTRY_NOT_FOUND_ERROR,
		};

		//
		// methods
		//
		NtkExport static String default_message(int what);
	};

	typedef Status<status> status_t;

	//
	// methods
	//
	virtual int count_entries() const =0;

	NtkExport Entry next_entry(bool traverse = false, status_t* status = NULL);
	NtkExport Entry::Ref next_ref(status_t* status = NULL);
	virtual status_t get_next_entry(Entry* entry, bool traverse = false) =0;
	virtual status_t get_next_ref(Entry::Ref* entry_ref) =0;
//	virtual status_t get_next_dirents(Dirent* buf, 

	virtual status_t rewind() =0;

};// class EntryList


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef EntryList entry_list_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::EntryList NEntryList;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::EntryList ntk_entry_list;
	#endif
#endif


#endif//EOH
