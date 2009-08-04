/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: applicationdefs.h,v 1.5 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_APPLICATIONDEFS_H__
#define __NTK_APPLICATION_APPLICATIONDEFS_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


extern NtkExport const uint NTK_MESSAGE;


//--------------------------------------------------------
// messages

enum {
	REFS_RECEIVED   = '_ARR',// Application kit: Refs Received
	SAVE_REQUESTED  = '_ASR',// Application kit: Save Requested
	APP_ACTIVATED   = '_AAA',// Application kit: App Activated
	ABOUT_REQUESTED = '_AAR',// Application kit: About Requested
	ARGV_RECEIVED   = '_AAG',// Application kit: ArGv received
	QUIT_REQUESTED  = '_QRQ',// Global: Quit ReQuested
};//messages


} namespace Ntk = ntk;


#endif//EOH
