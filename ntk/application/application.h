/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: application.h,v 1.8 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_APPLICATION_APPLICATION_H__
#define __NTK_APPLICATION_APPLICATION_H__


#include <list>

#ifndef __NTK_WINDOWS_WINDOW_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_STORAGE_DIRECTORY_H__
#include <ntk/storage/directory.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_VERSION_H__
#include <ntk/support/version.h>
#endif

#ifndef __NTK_APPLICATION_LOOPER_H__
#include <ntk/application/looper.h>
#endif


namespace ntk {


int global_main(int, const char_t**, HINSTANCE, int);


class Window;


class Application : public Looper {
public:
	//
	// methods
	//
	NtkExport Application(
		const String& name,
		const Version& version = Version(1));
	NtkExport virtual ~Application();

	NtkExport virtual HANDLE run();
	NtkExport virtual int message_loop();

	//
	// accessors and manipulators
	//
	NtkExport static HINSTANCE handle();
	NtkExport static int show_mode();
	NtkExport static int argc();
	NtkExport static const char_t** argv();

	NtkExport static Directory directory(status_t* status = NULL);

	NtkExport const Version& version() const;

	NtkExport virtual uint count_windows() const;
	NtkExport virtual Window* window_at(int index) const;
	NtkExport virtual Window* find_window(const String& title) const;

	NtkExport HACCEL accelerator_handle() const;
	NtkExport void set_accelerator_handle(HACCEL accelerator);

	//
	// message handler
	//
	NtkExport virtual void app_activated(bool state);
	NtkExport virtual void refs_received(const Message& message);
	NtkExport virtual void argv_received(int argc, const char_t** argv);
	NtkExport virtual void about_requested();
	NtkExport virtual void message_received(const Message& message);

protected:
	//
	// functions
	//
	NtkExport void dispatch_msg(MSG& msg);

private:
	//
	// types
	//
	typedef Looper Super;
	typedef std::list<Window*> WindowList;

	//
	// data
	//
	NtkExport static HINSTANCE m_handle;
	NtkExport static int m_show_mode;
	NtkExport static int m_argc;
	NtkExport static const char_t** m_argv;

	Version m_version;
	WindowList m_window_list;

	HACCEL m_accelerator_handle;

	//
	// functions
	//
	void clean_up_windows_();

	//
	// friends
	//
	friend class Window;
	friend int global_main(int, const char_t**, HINSTANCE, int);

};// class Application


NtkExport Application* application();


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Application application_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Application NApplication;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Application ntk_application;
	#endif
#endif


#endif//EOH
