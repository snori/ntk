/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: path.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_PATH_H__
#define __NTK_STORAGE_PATH_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_FLATTENABLE_H__
#include <ntk/support/flattenable.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class Directory;
class Entry;


class Path : public Flattenable {
public:
	//
	// methods
	//
	NtkExport Path();
	NtkExport Path(const String& path, const String& leaf = "", bool normalize = true);
	NtkExport Path(const Directory& dir, const String& leaf = "", bool normalize = true);
	NtkExport Path(const Entry& entry);
	NtkExport Path(const Path& val);
	NtkExport Path& operator=(const Path& rhs);
	NtkExport ~Path();

	NtkExport status_t set_to(const String& path, const String& leaf = "", bool normalize = true);
	NtkExport status_t set_to(const Directory& dir, const String& leaf = "", bool normalize = true);
	NtkExport status_t set_to(const Entry& entry);
	NtkExport void unset();

	NtkExport status_t init_check() const;

	NtkExport status_t append(const String& path, bool normalize = true);
	NtkExport status_t up();
	NtkExport Path up(status_t* status) const;

	NtkExport void normalize();

	NtkExport virtual status_t flatten(void* buffer, Size num_bytes) const;
	NtkExport virtual status_t unflatten(TypeCode type_code, void* buffer, Size num_bytes);

	//
	// accessors
	//
	NtkExport String as_string(status_t* status = NULL) const;
	NtkExport String leaf(status_t* status = NULL) const;
	NtkExport String parent(status_t* status = NULL) const;
	NtkExport status_t get_string(String* str) const;
	NtkExport status_t get_leaf(String* str) const;
	NtkExport status_t get_parent(String* str) const;

	NtkExport virtual Size flattened_size() const;
	NtkExport virtual TypeCode type_code() const;
	NtkExport virtual bool allows_type_code(TypeCode type_code) const;

	NtkExport bool is_root() const;

	//
	// operators
	//
	friend bool operator==(const Path& lhs, const Path& rhs)
	{
		Path l = lhs, r = rhs;
		l.normalize();
		r.normalize();

		return l.m_path == r.m_path;
	}
	friend bool operator!=(const Path& lhs, const Path& rhs) {return !operator==(lhs, rhs);}

private:
	//
	// data
	//
	String m_path;

	//
	// functions
	//
	void make_up_();
	void replace_separators_();
	void remove_last_separator_();

};// class Path


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Path path_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Path NPath;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Path ntk_path;
	#endif
#endif


#endif//EOH
