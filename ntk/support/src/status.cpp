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
		return _T("status_t: ���m�̃X�e�[�^�X�ł�\n");

	switch(what)
	{
	case ERR:
		return _T("[ERR] �G���[���������܂����B\n");

	case FAILED:
		return _T("[FAILED] ���s���܂����B\n");

	case SYSTEM_ERROR:
		return _T("[SYSTEM_ERROR] " + ntk::last_error_message());// + "\n");

	case ANSI_ERROR:
		return _T("[ANSI_ERROR] �����^�C���G���[���������܂����B\n");

	case NO_MEMORY_ERROR:
		return _T("������������܂���\n");

	case BAD_VALUE_ERROR:
		return _T("�l���s���ł�\n");

	case NOT_INITIALIZED_ERROR:
		return _T("���������̃G���[�ł�\n");

	case INDEX_ERROR:
		return _T("�C���f�N�X�G���[�ł�\n");

	default:
		return _T("status_t: ���m�̃G���[�ł�");
	}
}


//########################################################
}// namespace ntk
