/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: symlink.h,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_SYMLINK_H__
#define __NTK_STORAGE_SYMLINK_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_STORAGE_NODE_H__
#include <ntk/storage/node.h>
#endif

#ifndef __NTK_STORAGE_ENTRY_H__
#include <ntk/storage/entry.h>
#endif


namespace ntk {


class Directory;


class SymLink : public Node {
public:
	//
	// methods
	//
	NtkExport SymLink();
	NtkExport SymLink(const String& path);
	NtkExport SymLink(const Entry& entry);
	NtkExport SymLink(const Directory& dir, const String& path);
	NtkExport SymLink(const SymLink& val);
	NtkExport SymLink& operator=(const SymLink& rhs);
	NtkExport virtual ~SymLink();

};// class SymLink


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef SymLink sym_link_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::SymLink NSymLink;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::SymLink ntk_sym_link;
	#endif
#endif


#endif//EOH
