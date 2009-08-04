/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: symlink.cpp,v 1.3 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/symlink.h"


namespace ntk {


//########################################################


SymLink::SymLink()
{
}

SymLink::SymLink(const String& path)
{
	set_to(path);
}

SymLink::SymLink(const Entry& entry)
{
	set_to(entry);
}

SymLink::SymLink(const Directory& dir, const String& path)
{
	set_to(dir, path);
}

SymLink::SymLink(const SymLink& rhs)
{
	operator=(rhs);
}

SymLink&
SymLink::operator=(const SymLink& rhs)
{
	if(&rhs == this)
		return *this;

	Node::operator=(rhs);

	return *this;
}

SymLink::~SymLink()
{
}


//########################################################
}// namespace ntk
