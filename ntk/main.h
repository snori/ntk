/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: main.h,v 1.8 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#ifdef __NTK_MAIN_H__
#error <ntk/main.h> が二度インクルードされました
#endif

#define __NTK_MAIN_H__


#include <new.h>
#include <vector>
#include <algorithm>
#include <tchar.h>

#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_SUPPORT_DEBUG_H__
#include <ntk/support/debug.h>
#endif

#ifndef __NTK_APPLICATION_APPLICATION_H__
#include <ntk/application/application.h>
#endif


int ntk_main(int, const char_t**);


namespace ntk {


int
new_handler_throws_bad_alloc(size_t)
{
	throw std::bad_alloc();
}

void
rescue_error()
{return;
	#ifdef NTK_RELEASE
		return;
	#else
		MSG msg;
		while(GetMessage(&msg, NULL, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	#endif
}

int
global_main(int argc, const char_t** argv, HINSTANCE hinstance, int show_mode)
{
	// メモリリークの検出を行う設定に
	#if defined(NTK_WINDOWS) && defined(NTK_CHECK_MEMORY_LEAKS)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	_set_new_handler(ntk::new_handler_throws_bad_alloc);

	#if 0
		std::vector<String> arg_list(argc);
		std::copy(argv, argv + argc, arg_list.begin());

		Application::m_argv    = &arg_list[0];
	#else
		Application::m_argv    = argv;
	#endif
	Application::m_argc      = argc;
	Application::m_handle    = hinstance;
	Application::m_show_mode = show_mode;

	int result = 0;
	try
	{
		result = ::ntk_main(Application::argc(), Application::argv());
	}
	catch(std::exception& e)
	{
		error(e.what());
	}
	catch(...)
	{
		#ifdef NTK_DEBUG
			throw;//rescue_error();
		#else
			throw;
		#endif
	}

	return result;
}


} namespace Ntk = ntk;
//########################################################


#ifndef NTK_CONSOLE_APPLICATION
	int WINAPI
	_tWinMain(HINSTANCE hinstance, HINSTANCE, PSTR, int show_mode)
	{
		return ntk::global_main(__argc, (const char_t**)__targv, hinstance, show_mode);
	}
#else
	#ifndef NTK_UNICODE
		int
		main(int argc, const char** argv)
		{
			return ntk::global_main(argc, argv, GetModuleHandle(NULL), 0);
		}
	#else
		int
		wmain(int argc, const wchar_t** argv)
		{
			return ntk::global_main(argc, argv, GetModuleHandle(NULL), 0);
		}
	#endif
#endif


#ifdef NTK_DEF_MAIN
#define main ntk_main
#endif
