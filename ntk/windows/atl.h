/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: atl.h,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STD_ATL_H__
#define __NTK_STD_ATL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


#include <atlbase.h>
NtkExport extern CComModule _Module;

#ifdef min
#undef min
#endif

#define min(a,b) ((a) < (b) ? (a) : (b))

#include <atlcom.h>
#include <atlhost.h>


namespace ntk {


NtkExport void initialize_atl();
NtkExport void finalize_atl();


} namespace Ntk = ntk;


#endif//EOH
