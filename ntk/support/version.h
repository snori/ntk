/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: version.h,v 1.18 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_VERSION_H__
#define __NTK_SUPPORT_VERSION_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_DATAIO_H__
#include <ntk/support/dataio.h>
#endif


namespace ntk {


class Version {
public:
	//
	// constants
	//
	enum {
		INT_TYPE,
		INT8_TYPE,
		INT16_TYPE,
	};

	//
	// methods
	//
	NtkExport Version();
	NtkExport Version(
		int generation,
		int version = 0,
		int release = 0,
		int patch = 0,
		const String& name = "");
	NtkExport Version(const String& name);
	NtkExport Version(const Version& rhs);
	NtkExport Version& operator=(const Version& rhs);
	NtkExport virtual ~Version();

	NtkExport virtual bool read(DataIO& io, uint type = INT_TYPE);
	NtkExport virtual bool write(DataIO* io, uint type = INT_TYPE) const;

	//
	// aacessors and manipulators
	//
	NtkExport virtual int generation() const;
	NtkExport virtual int version() const;
	NtkExport virtual int release() const;
	NtkExport virtual int patch() const;
	NtkExport virtual String name() const;
	NtkExport virtual void reset(
		int generation,
		int version = 0,
		int release = 0,
		int patch = 0,
		const String& name = "");

	NtkExport virtual String as_string() const;

	//
	// operators
	//

private:
	//
	// data
	//
	int m_generation, m_version, m_release, m_patch;
	String m_name, m_string;

};// class Version


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Version version_t;
#endif


} namespace Ntk = ntk;


NtkExport bool operator==(const ntk::Version& lhs, const ntk::Version& rhs);
NtkExport bool operator<(const ntk::Version& lhs, const ntk::Version& rhs);
NtkExport bool operator>(const ntk::Version& lhs, const ntk::Version& rhs);

inline bool
operator!=(const ntk::Version& lhs, const ntk::Version& rhs)
{
	return !operator==(lhs, rhs);
}

inline bool
operator<=(const ntk::Version& lhs, const ntk::Version& rhs)
{
	return !operator>(lhs, rhs);
}

inline bool
operator>=(const ntk::Version& lhs, const ntk::Version& rhs)
{
	return !operator<(lhs, rhs);
}

template<>
inline ntk::DataIO&
operator>>(ntk::DataIO& lhs, ntk::Version& rhs)
{
	rhs.read(lhs);
	return lhs;
}

template<>
inline ntk::DataIO&
operator<<(ntk::DataIO& lhs, const ntk::Version& rhs)
{
	rhs.write(&lhs);
	return lhs;
}


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Version NVersion;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Version ntk_version;
	#endif
#endif


//########################################################


//
// version-info utility macros
//
#define NTK_GENERATION_NO(ver) ((ver & 0xff000000) >> 24)
#define NTK_VERSION_NO(ver)    ((ver & 0x00ff0000) >> 16)
#define NTK_RELEASE_NO(ver)    ((ver & 0x0000ff00) >> 8)
#define NTK_PATCH_NO(ver)      ((ver & 0x000000ff) >> 0)

//
// NTK version
//	format:0x[??:generation][??:version][??:release][??:patch]
//
#define NTK_VERSION      0x00030200
#define NTK_RELEASE_DATE 20031119

#define NTK_VER_GENERATION NTK_GENERATION_NO(NTK_VERSION)
#define NTK_VER_VERSION    NTK_VERSION_NO(NTK_VERSION)
#define NTK_VER_RELEASE    NTK_RELEASE_NO(NTK_VERSION)
#define NTK_VER_PATCH      NTK_PATCH_NO(NTK_VERSION)

#ifndef __NTK_SUPPORT_UTILITY_H__
#include <ntk/support/utility.h>
#endif
#define NTK_Version ::ntk::Version( \
	NTK_VER_GENERATION, \
	NTK_VER_VERSION, \
	NTK_VER_RELEASE, \
	NTK_VER_PATCH, \
	::ntk::format_string("%d", NTK_RELEASE_DATE))


#endif//EOH
