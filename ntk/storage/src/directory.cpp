/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: directory.cpp,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/directory.h"


#include <vector>
#include <ntk/windows/windows.h>


namespace ntk {


//########################################################
// public methods

Directory::Directory()
:	m_hfind(NULL)
{
}

Directory::Directory(const String& path)
:	m_hfind(NULL)
{
	set_to(path);
}

Directory::Directory(const Directory& dir, const String& path)
:	m_hfind(NULL)
{
	set_to(dir, path);
}

Directory::Directory(const Directory& val)
:	m_hfind(NULL)
{
	operator=(val);
}

Directory&
Directory::operator=(const Directory& rhs)
{
	if(&rhs == this)
		return *this;

	Node::operator=(rhs);
	EntryList::operator=(rhs);

	return *this;
}

Directory::~Directory()
{
}

Directory::status_t
Directory::create_file(const String& path, File* file, bool fail_if_exists)
{
	return st::ERR;
}

Directory::status_t
Directory::create_file(const String& path, FILE* file, bool fail_if_exists)
{
	return st::ERR;
}

Directory::status_t
Directory::create_directory(const String& path, Directory* dir)
{
	return st::ERR;
}

Directory::status_t
Directory::create_sym_link(const String& path, const String& link_to_path, SymLink* sym_link)
{
	return st::ERR;
}

Entry
Directory::find_entry(const String& path, bool traverse, status_t* status)
{
	status_t sts_, &sts = status ? *status : sts_;

	Entry entry;
	sts = find_entry(path, &entry, traverse);

	return entry;
}

Directory::status_t
Directory::find_entry(const String& path, Entry* entry, bool traverse)
{
	return st::ERR;
}

Directory::status_t
Directory::get_next_entry(Entry* entry, bool traverse)
{
	if(entry == NULL)
		return st::BAD_VALUE_ERROR;

	WIN32_FIND_DATA find_data;

	if(m_hfind == NULL)
	{
		m_hfind = FindFirstFile((path().as_string() + "\\*").c_str(), &find_data);
		if(m_hfind == INVALID_HANDLE_VALUE)
		{
			m_hfind = NULL;
			return st::SYSTEM_ERROR;
		}
	}
	else
	{
		BOOL ret = FindNextFile(m_hfind, &find_data);
		if(ret == FALSE)
			return status::ENTRY_NOT_FOUND_ERROR;
	}

	return entry->set_to(path().as_string() + "\\" + find_data.cFileName, traverse);
}

Directory::status_t
Directory::get_next_ref(Entry::Ref* entry_ref)
{
	return st::ERR;
}

Directory::status_t
Directory::rewind()
{
	if(m_hfind == NULL)
		return true;

	FindClose(m_hfind);
	m_hfind = NULL;

	return true;
}

//--------------------------------------------------------
// public accessors

int
Directory::count_entries() const
{
	struct func {
		static bool is_special_entry(const String& file_name)
		{
			return file_name == "." || file_name == "..";
		}
	};

	WIN32_FIND_DATA find_data;
	int count = 0;

	HANDLE h = FindFirstFile((path().as_string() + "\\*").c_str(), &find_data);
	if(h == INVALID_HANDLE_VALUE)
		return count;

	do
	{
		if(func::is_special_entry(find_data.cFileName) == false)
			count++;
	}
	while(FindNextFile(h, &find_data));

	FindClose(h);

	return count;
}

bool
Directory::is_root_directory() const
{
	Entry entry;
	if(get_entry(&entry).is_valid() == false)
		return false;

	Path path;
	if(entry.get_path(&path).is_valid() == false)
		return false;

	return path.is_root();
}


//########################################################


Directory
current_directory(status_t* status)
{
	return "";
}


//########################################################
}// namespace ntk
