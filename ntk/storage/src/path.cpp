/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: path.cpp,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/path.h"


#include <vector>
#include <ntk/support/debug.h>
#include <ntk/storage/directory.h>


namespace ntk {


//########################################################


namespace {

const char SEPARATOR = '\\';

bool
is_separator(const char* c)
{
	return c[0] == '/' || c[0] == '\\';
}

bool
drive_name_check(const String& path)
{
	return
		('a' <= path[0] && path[0] <= 'z' || 'A' <= path[0] && path[0] <= 'Z') &&
		path[1] == ':' &&
		(path[2] == '\\' || path[2] == '/');
}

}// anonymous namespace


//########################################################


Path::Path()
:	Flattenable()
{
}

Path::Path(const String& path, const String& leaf, bool normalize)
:	Flattenable()
{
	set_to(path, leaf, normalize);
}

Path::Path(const Directory& dir, const String& leaf, bool normalize)
:	Flattenable()
{
	set_to(dir, leaf, normalize);
}

Path::Path(const Entry& entry)
{
	set_to(entry);
}

Path::Path(const Path& rhs)
:	Flattenable(rhs)
{
	m_path = rhs.m_path;
}

Path&
Path::operator=(const Path& rhs)
{
	if(&rhs == this)
		return *this;

	Flattenable::operator=(rhs);
	m_path = rhs.m_path;

	return *this;
}

Path::~Path()
{
}

status_t
Path::set_to(const String& path, const String& leaf, bool normalize_)
{
	if(! drive_name_check(path))
		return status_t(st::BAD_VALUE_ERROR, "ドライブ名がありません\n");

	m_path = path;

	make_up_();

	if(leaf != "")
		append(leaf);

	if(normalize_)
		normalize();

	return st::OK;
}

status_t
Path::set_to(const Directory& dir, const String& leaf, bool normalize)
{
	return set_to(dir.entry().path().as_string(), leaf, normalize);
}

status_t
Path::set_to(const Entry& entry)
{
	return set_to(entry.path().as_string());
}

void
Path::unset()
{
	m_path = "";
}

status_t
Path::init_check() const
{
	return drive_name_check(m_path) ? st::OK : st::NOT_INITIALIZED_ERROR;
}

status_t
Path::append(const String& path, bool normalize)
{
	if(path == "" || is_separator(&path[0]))
		return status_t(st::BAD_VALUE_ERROR,
			"path が正しくありません (Path::append(const String&, bool)\n");

	if(! is_separator(&m_path[m_path.size() -1]))
		m_path += SEPARATOR;

	m_path += path;

	make_up_();

	return st::OK;
}

status_t
Path::up()
{
	return st::ERR;
}

Path
Path::up(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	Path path = *this;
	sts = path.up();

	return path;
}

void
Path::normalize()
{
	for(String::iterator c = m_path.begin(); c != m_path.end(); ++c)
	{
		if(! (is_separator(&c[0]) && c[1] == '.'))
			continue;

		if(c[2] == '.' && is_separator(&c[3]))// "../"
		{
			String::iterator prev_separator = c -1, next_separator = c +3;

			// hige/hoge/../huga
			//     ^(1)    ^(2)
			// (1) prev_separator
			// (2) next_separator

			if(prev_separator == m_path.begin())
				return;

			// 直前にセパレータが連続であるかもしれない
			while(is_separator(&prev_separator[0]))
			{
				--prev_separator;
				if(prev_separator == m_path.begin())
					return;
			}

			// 一階層上る
			while(is_separator(&prev_separator[0]) == false)
			{
				--prev_separator;
				if(prev_separator == m_path.begin())
					return;
			}

			m_path.erase(prev_separator, next_separator);
			c = m_path.begin();
		}
		else if(is_separator(&c[2]))// "./"
		{
			m_path.erase(c, c +2);
			c = m_path.begin();
		}
	}
}

status_t
Path::flatten(void* buffer, Size num_bytes) const
{
	return st::ERR;
}

status_t
Path::unflatten(TypeCode type_code, void* buffer, Size num_bytes)
{
	return st::ERR;
}

//********************************************************
// public accessors

String
Path::as_string(status_t* status) const
{
	if(status) *status = st::OK;
	return m_path;
}

status_t
Path::get_string(String* str) const
{
	if(str == NULL)
		return st::BAD_VALUE_ERROR;

	*str = m_path;

	return st::OK;
}

String
Path::leaf(status_t* status) const
{
	String leaf_;
	status_t sts_, &sts = status ? *status : sts_;

	sts = get_leaf(&leaf_);

	return leaf_;
}

status_t
Path::get_leaf(String* str) const
{
	if(str == NULL)
		return st::BAD_VALUE_ERROR;

	const char* last_separator = strrchr(&m_path[0], SEPARATOR);
	*str = String(last_separator +1, &m_path[m_path.size()]);

	return st::OK;
}

String
Path::parent(status_t* status) const
{
	String parent_;
	status_t sts_, &sts = status ? *status : sts_;

	sts = get_parent(&parent_);

	return parent_;
}

status_t
Path::get_parent(String* str) const
{
	if(str == NULL)
		return st::BAD_VALUE_ERROR;

	status_t sts = init_check();
	if(sts.is_valid_without_eval() == false)
		return sts;

	const char* last_separator = strrchr(&m_path[0], SEPARATOR);
	*str = String(&m_path[0], last_separator);

	return st::OK;
}

Path::Size
Path::flattened_size() const
{
	return 0;
}

Path::TypeCode
Path::type_code() const
{
	return 0;
}

bool
Path::allows_type_code(TypeCode type_code_) const
{
	return type_code_ == type_code();
}

bool
Path::is_root() const
{
	return m_path[3] == '\0' && drive_name_check(m_path);
}

//********************************************************
// functions

void
Path::make_up_()
{
	replace_separators_();
	remove_last_separator_();
}

void
Path::replace_separators_()
{
	const char SEP = '\\';
	const char BAD_SEP = '/';

	for(String::iterator it = m_path.begin(); it != m_path.end(); ++it)
		if(*it == BAD_SEP)
			*it = SEP;
}

void
Path::remove_last_separator_()
{
	String::iterator c = m_path.end() -1;

	if(c == m_path.begin() || (*c != '\\' && *c != '/') || is_root())
		return;

	// 末尾に separator が複数あった時のため
	while(*(c -1) == '\\' || *(c -1) == '/')
	{
		--c;
		if(c == m_path.begin()) return;
	}

	m_path.erase(c, m_path.end());
}


//########################################################
}// namespace ntk
