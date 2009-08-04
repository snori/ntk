/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: status.cpp,v 1.3 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/status.h"


#include <tchar.h>
#include <ntk/support/debug.h>


namespace ntk {


//########################################################
// functions

String
st::default_message(int what)
{
	if(what == 0)
	{
		#ifdef NTK_DEBUG
			return _T("status_t: NO_ERROR\n");
		#else
			return NULL;
		#endif
	}
	else if(what > 0)
		return _T("status_t: 未知のステータスです\n");

	switch(what)
	{
	case ERR:
		return _T("[ERR] エラーが発生しました。\n");

	case FAILED:
		return _T("[FAILED] 失敗しました。\n");

	case SYSTEM_ERROR:
		return _T("[SYSTEM_ERROR] " + ntk::last_error_message());// + "\n");

	case ANSI_ERROR:
		return _T("[ANSI_ERROR] ランタイムエラーが発生しました。\n");

	case NO_MEMORY_ERROR:
		return _T("メモリが足りません\n");

	case BAD_VALUE_ERROR:
		return _T("値が不正です\n");

	case NOT_INITIALIZED_ERROR:
		return _T("未初期化のエラーです\n");

	case INDEX_ERROR:
		return _T("インデクスエラーです\n");

	default:
		return _T("status_t: 未知のエラーです");
	}
}


//########################################################
}// namespace ntk
