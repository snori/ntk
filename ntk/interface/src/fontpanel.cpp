/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: fontpanel.cpp,v 1.2 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/fontpanel.h"


#include <stdlib.h>
#include <assert.h>
#include <commdlg.h>
#include <ntk/application/application.h>
#include <ntk/interface/window.h>


namespace ntk {


//########################################################
// public methods

FontPanel::FontPanel(const Font& font, const Messenger& target, Message* message, bool modal, bool hide_when_done)
{
	initialize_(font.data(), target, message, modal, hide_when_done);
}

FontPanel::FontPanel(const LOGFONT& logfont, const Messenger& target, Message* message, bool modal, bool hide_when_done)
{
	initialize_(logfont, target, message, modal, hide_when_done);
}

FontPanel::~FontPanel()
{
	delete m_messenger;
	delete m_message;
}

void
FontPanel::initialize_(const LOGFONT& logfont, const Messenger& target, Message* message, bool modal, bool hide_when_done)
{
	if(target.is_valid())
		m_messenger = new Messenger(target);
	else
		m_messenger = new Messenger(NULL, ntk::application());

	m_message = message ? message : new Message(FONT_CHANGED);

	m_logfont = logfont;
}

void
FontPanel::show(Window* owner)
{
	LOGFONT logfont = m_logfont;

	CHOOSEFONT choose_font;
	memset(&choose_font, 0, sizeof(choose_font));
	choose_font.lStructSize = sizeof(choose_font);
	choose_font.hwndOwner = owner ? owner->hwnd() : NULL;
	choose_font.lpLogFont = &logfont;
	choose_font.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;
	choose_font.rgbColors = RGB(0, 0, 0);
	choose_font.nFontType = SCREEN_FONTTYPE;

	int ret = ChooseFont(&choose_font);
	if(ret != 0)
	{
		Message message(*m_message);

		message.add_string ("face",       logfont.lfFaceName);
		message.add_int    ("size",       Font::height2size(logfont.lfHeight));
		message.add_bool   ("bold",       logfont.lfWeight    == FW_BOLD);
		message.add_bool   ("italic",     logfont.lfItalic    != FALSE);
		message.add_bool   ("under_line", logfont.lfUnderline != FALSE);
		message.add_bool   ("strike_out", logfont.lfStrikeOut != FALSE);

		m_messenger->send_message(message);
	}
}

const Messenger&
FontPanel::messenger() const
{
	return *m_messenger;
}

void
FontPanel::set_target(const Messenger& target)
{
	*m_messenger = target;
}

const Message&
FontPanel::message() const
{
	return *m_message;
}

void
FontPanel::set_message(Message* message)
{
	delete m_message;
	m_message = message;
}


//########################################################
}// namespace ntk
