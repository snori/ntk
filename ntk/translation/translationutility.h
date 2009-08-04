/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: translationutility.h,v 1.5 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_TRANSLATION_TRANSLATIONUTILITY_H__
#define __NTK_TRANSLATION_TRANSLATIONUTILITY_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_BITMAP_H__
#include <ntk/interface/bitmap.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class Bitmap;


class TranslationUtils {
public:
	//
	// methods
	//
	NtkExport static Bitmap* get_bitmap(const String& file_name);
	NtkExport static status_t store_bitmap(const String& file_name, const Bitmap& bitmap);

};// class TranslationUtils


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef TranslationUtils translation_utils_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::TranslationUtils NTranslationUtils;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::TranslationUtils ntk_translation_utils;
	#endif
#endif


#endif//EOH
