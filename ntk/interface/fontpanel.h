/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: fontpanel.h,v 1.5 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_FONTPANEL_H__
#define __NTK_INTERFACE_FONTPANEL_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_MESSENGER_H__
#include <ntk/application/messenger.h>
#endif

#ifndef __NTK_INTERFACE_GDIOBJECT_H__
#include <ntk/interface/gdiobject.h>
#endif


namespace ntk {


class Message;
class Window;
class Font;


class FontPanel {
public:
	//
	// methods
	//
	NtkExport FontPanel(
		const Font& default_font = plain_font(),
		const Messenger& target = Messenger(NULL, NULL),
		Message* message = NULL,
		bool modal = true,
		bool hide_when_done = true);
	NtkExport FontPanel(
		const LOGFONT& logfont,
		const Messenger& target = Messenger(NULL, NULL),
		Message* message = NULL,
		bool modal = true,
		bool hide_when_done = true);
	NtkExport ~FontPanel();

	NtkExport void show(Window* owner = NULL);

	//
	// accessors and manipulators
	//
	NtkExport virtual const Messenger& messenger() const;
	NtkExport virtual void set_target(const Messenger& target);

	NtkExport virtual const Message& message() const;
	NtkExport virtual void set_message(Message* message);

private:
	//
	// data
	//
	Messenger* m_messenger;
	Message* m_message;
	LOGFONT m_logfont;

	//
	// functions
	//
	void initialize_(
		const LOGFONT& logfont,
		const Messenger& target,
		Message* message,
		bool modal,
		bool hide_when_done);

};// class FontPanel


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef FontPanel font_panel_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::FontPanel NFontPanel;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::FontPanel ntk_font_panel;
	#endif
#endif


#endif//EOH
