/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: filepanel.cpp,v 1.1 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/filepanel.h"


#include <stdlib.h>
#include <assert.h>
#include <windows.h>
#include <ntk/application/application.h>


namespace ntk {


//########################################################
// public methods


FilePanel::FilePanel(PanelMode mode, const Messenger& target, Message* message, bool modal, bool hide_when_done)
:	m_panel_mode(mode)
{
	if(target.is_valid())
		m_messenger = new Messenger(target);
	else
		m_messenger = new Messenger(NULL, application());

	if(message)
		m_message = message;
	else if(mode == OPEN_PANEL)
		m_message = new Message(REFS_RECEIVED);
	else if(mode == SAVE_PANEL)
		m_message = new Message(SAVE_REQUESTED);
	else
		assert(false && "FilePanel: Illegal Mode");
}

FilePanel::~FilePanel()
{
	delete m_messenger;
	delete m_message;
}

void
FilePanel::show()
{
	enum{ BUF_SIZE = 512 };
	char buf[BUF_SIZE];
	buf[0] = '\0';

	OPENFILENAME m_info;
	memset(&m_info, 0, sizeof(m_info));
	m_info.lStructSize	= sizeof(m_info);
	m_info.hwndOwner		= NULL;
	m_info.lpstrFilter	= "*";
	m_info.nMaxFile			= BUF_SIZE;
	m_info.lpstrFile		= buf;

	int ret = 0;
	if(panel_mode() == OPEN_PANEL)
	{
		m_info.Flags |= OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		m_info.lpstrTitle = "ŠJ‚­";

		ret = GetOpenFileName(&m_info);
	} else {
		m_info.lpstrTitle = "–¼‘O‚ð•t‚¯‚Ä•Û‘¶";

		ret = GetSaveFileName(&m_info);
	}

	if(ret)
	{
		Message message(*m_message);
		message.add_string("name", buf);
		m_messenger->send_message(message);
	}
}

FilePanel::PanelMode
FilePanel::panel_mode() const
{
	return m_panel_mode;
}

void
FilePanel::set_panel_mode(PanelMode mode)
{
	m_panel_mode = mode;
	assert(false);// currently not supported
}

const Messenger&
FilePanel::messenger() const
{
	return *m_messenger;
}

void
FilePanel::set_target(const Messenger& target)
{
	*m_messenger = target;
}

const Message&
FilePanel::message() const
{
	return *m_message;
}

void
FilePanel::set_message(Message* message)
{
	delete m_message;
	m_message = message;
}


//########################################################
}// namespace ntk
