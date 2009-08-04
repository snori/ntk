/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: version.cpp,v 1.5 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/Version.h"


#include <ntk/support/utility.h>


namespace ntk {


//########################################################
// public methods

Version::Version()
{
	reset(0, 0, 0, 0, "");
}

Version::Version(int generation, int version, int release, int patch, const String& name)
{
	reset(generation, version, release, patch, name);
}

Version::Version(const String& name)
{
	reset(0, 0, 0, 0, name);
}

Version::Version(const Version& val)
{
	operator=(val);
}

Version&
Version::operator=(const Version& rhs)
{
	if(&rhs == this)
		return *this;

	m_generation = rhs.m_generation;
	m_version    = rhs.m_version;
	m_release    = rhs.m_release;
	m_patch      = rhs.m_patch;
	m_name       = rhs.m_name;
	m_string     = rhs.m_string;

	return *this;
}

Version::~Version()
{
}

bool
Version::read(DataIO& io, uint type)
{
	switch(type)
	{
	case INT_TYPE:
		io
			>> m_generation
			>> m_version
			>> m_release
			>> m_patch;
		break;

	case INT8_TYPE:
		{
			io
				>> (int8&)m_generation
				>> (int8&)m_version
				>> (int8&)m_release
				>> (int8&)m_patch;
		}
		break;

	case INT16_TYPE:
		io
			>> (int16&)m_generation
			>> (int16&)m_version
			>> (int16&)m_release
			>> (int16&)m_patch;
		break;

	default:
		return false;
	}

	return true;
}

bool
Version::write(DataIO* io, uint type) const
{
	switch(type)
	{
	case INT_TYPE:
		*io
			<< m_generation
			<< m_version
			<< m_release
			<< m_patch;
		break;

	case INT8_TYPE:
		*io
			<< (int8)m_generation
			<< (int8)m_version
			<< (int8)m_release
			<< (int8)m_patch;
		break;

	case INT16_TYPE:
		*io
			<< (int16)m_generation
			<< (int16)m_version
			<< (int16)m_release
			<< (int16)m_patch;
		break;

	default:
		return false;
	}

	return true;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

int
Version::generation() const
{
	return m_generation;
}

int
Version::version() const
{
	return m_version;
}

int
Version::release() const
{
	return m_release;
}

int
Version::patch() const
{
	return m_patch;
}

String
Version::name() const
{
	return m_name;
}

void
Version::reset(int generation, int version, int release, int patch, const String& name)
{
	m_generation = generation;
	m_version    = version;
	m_release    = release;
	m_patch      = patch;
	m_name       = name;

	m_string = format_string("%d", m_generation);

	if(true)//m_version || m_release || m_patch)
		m_string += format_string(".%d", m_version);

	if(m_release || m_patch)
		m_string += format_string(".%d", m_release);

	if(m_patch)
		m_string += format_string(".%d", m_patch);
}

String
Version::as_string() const
{
	return m_string;
}


//########################################################
}// namespace ntk


bool
operator==(const ntk::Version& lhs, const ntk::Version& rhs)
{
	return
		lhs.generation() == rhs.generation() &&
		lhs.version()    == rhs.version() &&
		lhs.release()    == rhs.release() &&
		lhs.patch()      == rhs.patch();
}

bool
operator<(const ntk::Version& lhs, const ntk::Version& rhs)
{
	if(lhs == rhs)
		return false;

	if(lhs.generation() < rhs.generation())
		return true;

	if(
		lhs.generation() <= rhs.generation() &&
		lhs.version()    <  rhs.version())
		return true;

	if(
		lhs.generation() <= rhs.generation() &&
		lhs.version()    <= rhs.version() &&
		lhs.release()    <  rhs.release())
		return true;

	if(
		lhs.generation() <= rhs.generation() &&
		lhs.version()    <= rhs.version() &&
		lhs.release()    <= rhs.release() &&
		lhs.release()    <  rhs.release())
		return true;

	return false;
}

bool
operator>(const ntk::Version& lhs, const ntk::Version& rhs)
{
	if(lhs == rhs)
		return false;

	if(lhs.generation() > rhs.generation())
		return true;

	if(
		lhs.generation() >= rhs.generation() &&
		lhs.version()    >  rhs.version())
		return true;

	if(
		lhs.generation() >= rhs.generation() &&
		lhs.version()    >= rhs.version() &&
		lhs.release()    >  rhs.release())
		return true;

	if(
		lhs.generation() >= rhs.generation() &&
		lhs.version()    >= rhs.version() &&
		lhs.release()    >= rhs.release() &&
		lhs.release()    >  rhs.release())
		return true;

	return false;
}


//########################################################
