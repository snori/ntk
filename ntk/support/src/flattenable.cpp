/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: flattenable.cpp,v 1.3 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/flattenable.h"


#include <stddef.h>


namespace ntk {


//########################################################


bool
Flattenable::allows_type_code(TypeCode) const
{
	return false;
}


//########################################################
}// namespace ntk
