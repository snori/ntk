/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: entry.cpp,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/entry.h"


#include <ntk/windows/windows.h>


namespace ntk {


//########################################################


Entry::Entry()
{
}

Entry::Entry(const String& path, bool traverse)
{
	set_to(path, traverse);
}

Entry::Entry(const Directory& dir, const String& path, bool traverse)
{
	set_to(dir, path, traverse);
}

Entry::Entry(const Entry& rhs)
{
	operator=(rhs);
}

Entry::~Entry()
{
}

Entry&
Entry::operator=(const Entry& rhs)
{
	if(&rhs == this)
		return *this;

	Statable::operator=(rhs);
	m_path = rhs.m_path;

	return *this;
}

status_t
Entry::set_to(const String& path, bool traverse_)
{
	status_t sts = m_path.set_to(path);

	if(sts.is_valid_without_eval())
	{
		if(traverse_)
			traverse();
	}

	return sts;
}

status_t
Entry::set_to(const Directory& dir, const String& path, bool traverse_)
{
	status_t sts = m_path.set_to(dir, path);

	if(sts.is_valid_without_eval())
	{
		if(traverse_)
			traverse();
	}

	return sts;
}

void
Entry::unset()
{
	m_path.unset();
}

status_t
Entry::init_check() const
{
	return m_path.init_check();
}

void
Entry::traverse()
{
}

//--------------------------------------------------------
// public accessors

Entry::Ref
Entry::ref(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	Ref ref_;
	sts = get_ref(&ref_);

	return ref_;
}

status_t
Entry::get_ref(Ref* ref) const
{
	if(ref == NULL)
		return st::BAD_VALUE_ERROR;

	status_t sts = m_path.init_check();
	if(sts.is_valid_without_eval() == false)
		return sts;

	return st::ERR;
}

Path
Entry::path(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	Path path_;
	sts = get_path(&path_);

	return path_;
}

status_t
Entry::get_path(Path* path) const
{
	if(path == NULL)
		return st::BAD_VALUE_ERROR;

	status_t sts = m_path.init_check();
	if(sts.is_valid_without_eval() == false)
		return sts;

	*path = m_path;

	return st::OK;
}

String
Entry::name(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	String str;
	sts = get_name(&str);

	return str;
}

status_t
Entry::get_name(String* str) const
{
	if(str == NULL)
		return st::BAD_VALUE_ERROR;

	return m_path.get_leaf(str);
}

String
Entry::extension(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	String str;
	sts = get_extension(&str);

	return str;
}

status_t
Entry::get_extension(String* str) const
{
	if(str == NULL)
		return st::BAD_VALUE_ERROR;

	if(is_file() == false)
		return status_t(st::ERR,
			"このエントリはファイルではないので拡張子は取得できません\n");

	String file_name = name();
	char_t* dot = strrchr(&file_name[0], '.');

	if(dot == NULL)
		return status_t(st::ERR, "拡張子はありません\n");

	*str = dot+1;

	parent();// why is here this? (a question to me...

	return true;
}

String
Entry::parent(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	String str;
	sts = get_parent(&str);

	return str;
}

status_t
Entry::get_parent(String* str) const
{
	if(str == NULL)
		return st::BAD_VALUE_ERROR;

	return m_path.get_parent(str);
}

bool
Entry::is_exists() const
{
	return m_path.init_check() &&
		GetFileAttributes(m_path.as_string().c_str()) != 0xffffffff;
}

bool
Entry::is_file() const
{
	return is_directory() == false;
}

bool
Entry::is_directory() const
{
	return (GetFileAttributes(m_path.as_string().c_str()) & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool
Entry::is_sym_link() const
{
	return false;
}

//########################################################
}// namespace ntk
