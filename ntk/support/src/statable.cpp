/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: statable.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/statable.h"


namespace ntk {


//########################################################
// public methods

Statable::Statable()
{
}

Statable::~Statable()
{
}

bool
Statable::is_file() const
{
	return false;
}

bool
Statable::is_directory() const
{
	return false;
}

bool
Statable::is_sym_link() const
{
	return false;
}


//########################################################
}// namespace ntk
