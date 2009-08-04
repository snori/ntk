/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: translatorroster.h,v 1.5 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_TRANSLATION_TRANSLATORROSTER_H__
#define __NTK_TRANSLATION_TRANSLATORROSTER_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_MESSAGE_H__
#include <ntk/application/message.h>
#endif

#ifndef __NTK_STORAGE_PATH_H__
#include <ntk/storage/path.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif

#ifndef __NTK_TRANSLATION_TRANSLATOR_H__
#include <ntk/translation/translator.h>
#endif


namespace ntk {


class PositionIO;


class TranslatorRoster {
public:
	//
	// methods
	//
	NtkExport TranslatorRoster();
	NtkExport ~TranslatorRoster();

	NtkExport virtual status_t translate(
		PositionIO* destination,
		const PositionIO& source,
		const Translator::Info& source_info,
		const Message& source_extension,
		uint reserved_0 = 0, uint reserved_1 = 0, const String& reserved = "");

	NtkExport virtual status_t add_translator(Translator* translator);
	NtkExport virtual status_t add_translators(const Path& path);

	//
	// functions
	//
	static TranslatorRoster* default_roster();

};// class TranslatorRoster


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef TranslatorRoster translator_roster_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::TranslatorRoster NTranslatorRoster;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::TranslatorRoster ntk_translator_roster;
	#endif
#endif


#endif//EOH
