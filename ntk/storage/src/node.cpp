/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: node.cpp,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/node.h"
//--------------------------------------------------------


namespace ntk {


//########################################################


Node::Node()
{
}

Node::Node(const String& path)
{
	set_to(path);
}

Node::Node(const Entry& entry)
{
	set_to(entry);
}

Node::Node(const Directory& dir, const String& path)
{
	set_to(dir, path);
}

Node::Node(const Node& rhs)
{
	operator=(rhs);
}

Node::~Node()
{
}

Node&
Node::operator=(const Node& rhs)
{
	if(&rhs == this)
		return *this;

	Statable::operator=(rhs);
	m_entry = rhs.m_entry;

	return *this;
}

status_t
Node::set_to(const String& path)
{
	return m_entry.set_to(path);
}

status_t
Node::set_to(const Entry& entry)
{
	status_t sts = entry.init_check();
	if(sts.is_valid_without_eval() == false)
		return sts;

	m_entry = entry;
	return sts;
}

status_t
Node::set_to(const Directory& dir, const String& path)
{
	return m_entry.set_to(dir, path);
}

void
Node::unset()
{
	m_entry.unset();
}

status_t
Node::init_check() const
{
	return m_entry.init_check();
}

status_t
Node::lock()
{
	return st::ERR;
}

status_t
Node::unlock()
{
	return st::ERR;
}

//********************************************************
// public accessors

Entry
Node::entry(status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;

	Entry entry;
	sts = get_entry(&entry);

	return entry;
}

status_t
Node::get_entry(Entry* entry) const
{
	if(entry == NULL)
		return st::BAD_VALUE_ERROR;

	*entry = m_entry;

	return st::OK;
}

Path
Node::path() const
{
	return m_entry.path();
}


//########################################################
}// namespace ntk
