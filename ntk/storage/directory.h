/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: directory.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_DIRECTORY_H__
#define __NTK_STORAGE_DIRECTORY_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_STORAGE_NODE_H__
#include <ntk/storage/node.h>
#endif

#ifndef __NTK_STORAGE_ENTRYLIST_H__
#include <ntk/storage/entrylist.h>
#endif

#ifndef __NTK_SUPPORT_STATUS__
#include <ntk/support/status.h>
#endif


namespace ntk {


class File;
class FILE;
class SymLink;


class Directory : public Node, public EntryList {
public:
	//
	// methods
	//
	NtkExport Directory();
	NtkExport Directory(const String& path);
	NtkExport Directory(const Directory& dir, const String& path);
	NtkExport Directory(const Directory& val);
	NtkExport Directory& operator=(const Directory& rhs);
	NtkExport virtual ~Directory();

	status_t set_to(const String& path) {return Node::set_to(path);}
	status_t set_to(const Directory& dir, const String& path) {return Node::set_to(dir, path);}

	NtkExport status_t create_file(const String& path, File* file, bool fail_if_exists = false);
	NtkExport status_t create_file(const String& path, FILE* file, bool fail_if_exists = false);
	NtkExport status_t create_directory(const String& path, Directory* dir);
	NtkExport status_t create_sym_link(const String& path, const String& link_to_path, SymLink* sym_link);

	NtkExport Entry find_entry(const String& path, bool traverse = false, status_t* status = NULL);
	NtkExport status_t find_entry(const String& path, Entry* entry, bool traverse = false);

	NtkExport virtual status_t get_next_entry(Entry* entry, bool traverse = false);
	NtkExport virtual status_t get_next_ref(Entry::Ref* entry_ref);
	NtkExport virtual status_t rewind();

	//
	// accessors and manipulators
	//
	NtkExport virtual int count_entries() const;

	NtkExport bool is_root_directory() const;

private:
	//
	// data
	//
	HANDLE m_hfind;

};// class Directory


NtkExport status_t create_directory(const String& path, int mode = 0);
//NtkExport status_t find_directory(
NtkExport Directory current_directory(status_t* status = NULL);


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Directory directory_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Directory NDirectory;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Directory ntk_directory;
	#endif
#endif


#endif//EOH
