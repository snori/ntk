/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: filepanel.h,v 1.5 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_FILEPANEL_H__
#define __NTK_INTERFACE_FILEPANEL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_MESSENGER_H__
#include <ntk/application/messenger.h>
#endif


namespace ntk {


class Message;


class FilePanel {
public:
	//
	// constants
	//
	enum PanelMode {
		OPEN_PANEL,
		SAVE_PANEL
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef PanelMode panel_mode_t;
	#endif

	//
	// methods
	//
	NtkExport FilePanel(
		PanelMode mode = OPEN_PANEL,
		const Messenger& target = Messenger(NULL, NULL),
		Message* message = NULL,
		bool modal = true,
		bool hide_when_done = true);
	NtkExport virtual ~FilePanel();

	NtkExport virtual void show();

	//
	// accessors
	//
	NtkExport virtual PanelMode panel_mode() const;
	NtkExport virtual void set_panel_mode(PanelMode panel_mode);

	NtkExport virtual const Messenger& messenger() const;
	NtkExport virtual void set_target(const Messenger& target);

	NtkExport virtual const Message& message() const;
	NtkExport virtual void set_message(Message* message);

private:
	//
	// data
	//
	PanelMode m_panel_mode;
	Messenger* m_messenger;
	Message* m_message;

};// class FilePanel


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef FilePanel file_panel_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::FilePanel NFilePanel;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::FilePanel ntk_file_panel;
	#endif
#endif


#endif//EOH
