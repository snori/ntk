/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: node.h,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_NODE_H__
#define __NTK_STORAGE_NODE_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATABLE_H__
#include <ntk/support/statable.h>
#endif

#ifndef __NTK_SUPPORT_ENTRY_H__
#include <ntk/storage/entry.h>
#endif


namespace ntk {


class Directory;


class Node : public Statable {
public:
	//
	// types
	//
	struct Ref {};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Ref ref_t;
	#endif

	//
	// methods
	//
	NtkExport Node();
	NtkExport Node(const String& path);
	NtkExport Node(const Entry& entry);
	NtkExport Node(const Directory& dir, const String& path);
	NtkExport Node(const Node& val);
	NtkExport Node& operator=(const Node& rhs);
	NtkExport virtual ~Node();

	NtkExport status_t set_to(const String& path);
	NtkExport status_t set_to(const Entry& entry);
	NtkExport status_t set_to(const Directory& dir, const String& path);
	NtkExport void unset();

	NtkExport status_t init_check() const;

	NtkExport status_t lock();
	NtkExport status_t unlock();

	//
	// accessors
	//
	NtkExport Entry entry(status_t* status = NULL) const;
	NtkExport status_t get_entry(Entry* entry) const;

	NtkExport Path path() const;

private:
	//
	// data
	//
	Entry m_entry;

};// class Node


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Node node_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Node NNode;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Node ntk_node;
	#endif
#endif


#endif//EOH
