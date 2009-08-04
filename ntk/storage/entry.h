/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: entry.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_ENTRY_H__
#define __NTK_STORAGE_ENTRY_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATABLE_H__
#include <ntk/support/statable.h>
#endif

#ifndef __NTK_STORAGE_PATH_H__
#include <ntk/storage/path.h>
#endif


namespace ntk {


class Entry : public Statable {
public:
	//
	// types
	//
	struct Ref {};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Ref ref_t;
	#endif

	//
	// methods
	//
	NtkExport Entry();
	NtkExport Entry(const String& path, bool traverse = false);
	NtkExport Entry(const Directory& dir, const String& path, bool traverse = false);
	NtkExport Entry(const Entry& rhs);
	NtkExport Entry& operator=(const Entry& rhs);
	NtkExport virtual ~Entry();

	NtkExport status_t set_to(const String& path, bool traverse = false);
	NtkExport status_t set_to(const Directory& dir, const String& path, bool traverse = false);
	NtkExport void unset();

	NtkExport status_t init_check() const;

	NtkExport void traverse();

	NtkExport status_t move_to();
	NtkExport status_t rename();
	NtkExport status_t remove();

	//
	// accessors and manipulators
	//
	NtkExport Ref ref(status_t* status = NULL) const;
	NtkExport Path path(status_t* status = NULL) const;
	NtkExport String name(status_t* status = NULL) const;
	NtkExport String extension(status_t* status = NULL) const;
	NtkExport String parent(status_t* status = NULL) const;
	NtkExport status_t get_ref(Ref* ref) const;
	NtkExport status_t get_path(Path* path) const;
	NtkExport status_t get_name(String* str) const;
	NtkExport status_t get_extension(String* str) const;
	NtkExport status_t get_parent(String* str) const;

//	get_stat();

	NtkExport bool is_exists() const;

	NtkExport virtual bool is_file() const;
	NtkExport virtual bool is_directory() const;
	NtkExport virtual bool is_sym_link() const;

private:
	//
	// data
	//
	Path m_path;

};// class Entry


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Entry entry_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Entry NEntry;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Entry ntk_entry;
	#endif
#endif


#endif//EOH
