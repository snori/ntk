/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: flattenable.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_FLATTENABLE_H__
#define __NTK_SUPPORT_FLATTENABLE_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class Flattenable {
public:
	//
	// types
	//
	typedef size_t Size;
	typedef uint   TypeCode;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Size     size_t;
		typedef TypeCode type_code_t;
	#endif

	//
	// methods
	//
	NtkExport virtual Size flattened_size() const =0;
	NtkExport virtual status_t flatten(void* buffer, Size num_bytes) const =0;
	NtkExport virtual status_t unflatten(TypeCode type_code, void* buffer, Size num_bytes) =0;

	NtkExport virtual TypeCode type_code() const =0;
	NtkExport virtual bool allows_type_code(TypeCode type_code) const;

};// class Flattenable


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Flattenable flattenable_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Flattenable NFlattenable;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Flattenable ntk_flattenable;
	#endif
#endif


#endif
