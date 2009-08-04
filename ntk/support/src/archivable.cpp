/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: archivable.cpp,v 1.3 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/archivable.h"


namespace ntk {


//########################################################
// public methods

Archivable::Archivable()
{
}

Archivable::Archivable(Message* archive)
{
}

Archivable::~Archivable()
{
}

status_t
Archivable::archive(Message* archive)
{
	return st::OK;
}


//########################################################
}// namespace ntk
