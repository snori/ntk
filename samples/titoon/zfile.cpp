/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: zfile.cpp,v 1.3 2003/11/11 12:07:11 nsuzuki Exp $

******************************************************************************/
//#define NTK_BUILD
#include "zfile.h"


#include <ntk/storage/directory.h>


namespace ntk {


//########################################################
// public methods

ZFILE::ZFILE()
:	m_fp(NULL)
{
}

ZFILE::ZFILE(const String& path, const String& access_mode)
:	m_fp(NULL)
{
	set_to(path, access_mode);
}

ZFILE::ZFILE(const Entry& entry, const String& access_mode)
:	m_fp(NULL)
{
	set_to(entry, access_mode);
}

ZFILE::ZFILE(const Directory& dir, const String& path, const String& access_mode)
:	m_fp(NULL)
{
	set_to(dir, path, access_mode);
}

ZFILE::~ZFILE()
{
	unset();
}

ZFILE::status_t
ZFILE::set_to(const String& path, const String& access_mode)
{
	// ‚·‚Å‚ÉŠJ‚¢‚Ä‚¢‚½‚ç
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(path);
	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(access_mode);
}

ZFILE::status_t
ZFILE::set_to(const Entry& entry, const String& access_mode)
{
	// ‚·‚Å‚ÉŠJ‚¢‚Ä‚¢‚½‚ç
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(entry);
	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(access_mode);
}

ZFILE::status_t
ZFILE::set_to(const Directory& dir, const String& path, const String& access_mode)
{
	// ‚·‚Å‚ÉŠJ‚¢‚Ä‚¢‚½‚ç
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(dir, path);
	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(access_mode);
}

void
ZFILE::unset()
{
	if(is_valid() == false)
		return;

	Node::unset();

	gzclose(m_fp);
	m_fp = NULL;
}

ZFILE::status_t
ZFILE::init_check() const
{
	status_t sts = Node::init_check();

	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return is_valid() ? st::OK : st::NOT_INITIALIZED_ERROR;
}

ZFILE::Size
ZFILE::read(void* buffer, Size num_bytes)
{
	if(! is_valid())
		return 0;

	return gzread(m_fp, buffer, num_bytes);
}

ZFILE::Size
ZFILE::write(const void* buffer, Size num_bytes)
{
	if(! is_valid())
		return 0;

	return gzwrite(m_fp, const_cast<void*>(buffer), num_bytes);
}

//--------------------------------------------------------
// public accessors

gzFile
ZFILE::fp() const
{
	return m_fp;
}

const String&
ZFILE::access_mode() const
{
	return m_access_mode;
}

//********************************************************
// functions

ZFILE::status_t
ZFILE::set_to_(const String& access_mode)
{
	m_access_mode = access_mode;
	m_fp = gzopen(path().as_string().c_str(), m_access_mode.c_str());

	if(! is_valid())
		return status_t::ENTRY_NOT_FOUND;

	return st::OK;
}


//########################################################
}// namespace ntk
