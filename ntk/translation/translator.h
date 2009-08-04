/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: translator.h,v 1.6 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_TRANSLATION_TRANSLATOR_H__
#define __NTK_TRANSLATION_TRANSLATOR_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif

#ifndef __NTK_SUPPORT_VERSION_H__
#include <ntk/support/version.h>
#endif


namespace ntk {


class PositionIO;


class Translator {
public:
	//
	// types
	//
	enum StreamType {
		UNKNOWN_STREAM = 0,
		BITMAP_STREAM
	};

	struct TranslationFormat {
		uint type;
		String name;
	};

	struct Info {
		int reserved;
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef StreamType stream_type_t;
		typedef TranslationFormat translation_format_t;
	#endif

	//
	// methods
	//
	NtkExport Translator(
		const String& name,// short name, e.g. "TIFF Translator"
		const String& info,// long information, e.g. "TIFF Translator (libtiff). by the Pie Man (Pie@the.man)"
		const Version& version);
	NtkExport virtual ~Translator();

	//NtkExport virtual status_t identify(
	//	const PositionIO& source,
	//	Format

	NtkExport virtual status_t translate(
		PositionIO* source,
		PositionIO* destination);

	//Format

	//
	// accessors
	//
	NtkExport virtual String name() const;
	NtkExport virtual String info() const;
	NtkExport virtual Version version() const;

private:
	//
	// data
	//
	String m_name, m_info;
	Version m_version;

};// class Translator


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Translator translator_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Translator NTranslator;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Translator ntk_translator;
	#endif
#endif


#endif//EOH
