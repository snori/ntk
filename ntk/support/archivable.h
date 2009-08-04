/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: archivable.h,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_ARCHIVABLE_H__
#define __NTK_SUPPORT_ARCHIVABLE_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class Message;


class Archivable {
public:
	//
	// methods
	//
	NtkExport Archivable();
	NtkExport Archivable(Message* archive);
	NtkExport virtual ~Archivable();

	NtkExport virtual status_t archive(Message* archive);

	//
	// functions
	//
	static Archivable* instantiate(Message* archive);

};// class Archivable


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Archivable archivable_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Archivable NArchivable;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Archivable ntk_archivable;
	#endif
#endif


#endif//EOH
