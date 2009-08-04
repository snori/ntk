/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: statable.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_STATABLE_H__
#define __NTK_SUPPORT_STATABLE_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifdef NTK_WINDOWS
#include <sys/stat.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class Statable {
public:
	//
	// methods
	//
	NtkExport Statable();
	NtkExport virtual ~Statable() =0;

	NtkExport virtual bool is_file() const;
	NtkExport virtual bool is_directory() const;
	NtkExport virtual bool is_sym_link() const;

};// class Statable


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Statable statable_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Statable NStatable;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Statable ntk_statable;
	#endif
#endif


#endif//EOH
