/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: atl.cpp,v 1.3 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/windows/atl.h"


#include <ntk/application/application.h>


CComModule _Module;// Ç±Ç§Ç‚ÇÁÇ»Ç¢Ç∆Ç¢ÇØÇ»Ç¢ÇÁÇµÇ¢ÅEÅEÅE

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


namespace ntk {


//########################################################


void
initialize_atl()
{
	static bool initialized = false;
	if(! initialized)
	{
		initialized = true;

		_Module.Init(ObjectMap, Application::handle());
		AtlAxWinInit();
	}
}


void
finalize_atl()
{
}


//########################################################
}// namespace ntk
