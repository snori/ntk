/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: textview.cpp,v 1.4 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/textview.h"


#include <stdarg.h>
#include <string.h>
#include <tchar.h>
#include <richedit.h>
#include <ntk/windows/windows.h>
#include <boost/scoped_array.hpp>


namespace ntk {


//########################################################


namespace {


static uint
flag2style(uint flags)
{
	uint style = 0;

	if(flags & TextView::AUTO_V_SCROLL)
		style |= ES_AUTOVSCROLL;

	if(flags & TextView::AUTO_H_SCROLL)
		style |= ES_AUTOHSCROLL;

	if(flags & TextView::MULTI_LINE)
		style |= ES_MULTILINE;

	if(flags & TextView::WANT_RETURN)
		style |= ES_WANTRETURN;

	return style;
}

uint
flags2ex_style(uint flags)
{
	uint style = 0;
	return style;
}

String
rich_edit_window_class()
{
	static bool done = false;
	if(! done)
	{
		done = true;

		HINSTANCE h = LoadLibrary("RICHED20.DLL");
		if(h == NULL)
			status_t(st::SYSTEM_ERROR).show_error();
	}

	#ifdef NTK_UNICODE
		return "RichEdit20W";
	#else
		return "RichEdit20A";
	#endif
}


}// anonymous namespace


//########################################################
// public methods


TextView::TextView(const Rect& frame, const String& name_, const String& text_, uint mode, uint flags)
:	SystemControl("EDIT", frame, text_, "", NULL, mode, flags, transparent_color(), DEFAULT_STYLE | flag2style(flags), DEFAULT_EX_STYLE | flags2ex_style(flags))
{
}

TextView::TextView(const String& window_class, const Rect& frame, const String& name_, const String& text_, uint mode, uint flags, uint style, uint ex_style)
:	SystemControl(window_class, frame, text_, "", NULL, mode, flags, transparent_color(), flag2style(flags) | style, flags2ex_style(flags) | ex_style)
{
}

void
TextView::print(const char_t* str)
{
	SendMessage(hwnd(), EM_SETSEL, -1, -1);
	SendMessage(hwnd(), EM_REPLACESEL, FALSE, (LPARAM)str);
}

void
TextView::print(const String& str)
{
	print(str.c_str());
}

void
TextView::printf(const char_t* format, ...)
{
	boost::scoped_array<char_t> buf(new char_t[2048]);

	va_list args;
	va_start(args, format);
	_vstprintf(&buf[0], format, args);
	va_end(args);

	print(&buf[0]);
}

String
TextView::text() const
{
	size_t length = GetWindowTextLength(hwnd());
	boost::scoped_array<char_t> buf(new char_t[length +1]);
	buf[0] = '\0';

	GetWindowText(hwnd(), &buf[0], length);

	return &buf[0];
}

void
TextView::set_text(const String& text_)
{
	set_name(text_);
}

void
TextView::get_line(int index, String* str_buf)
{
	enum {BUF_SIZE = 512};
	boost::scoped_array<char_t> buf(new char_t[BUF_SIZE +1]);
	buf[0] = '\0';

	::SendMessage(hwnd(), EM_GETLINE, index, (LPARAM)&buf[0]);

	*str_buf = &buf[0];
}


//########################################################
// public methods


RichTextView::RichTextView(const Rect& frame, const String& name, const String& text, uint mode, uint flags)
:	TextView(rich_edit_window_class(), frame, name, text, mode, flags, DEFAULT_STYLE | flag2style(flags), DEFAULT_EX_STYLE | flags2ex_style(flags))
{
	initialize_();
}


//********************************************************
// private functions


status_t
RichTextView::initialize_()
{
	CHARFORMAT2 format;
	memset(&format, 0, sizeof(format));

	format.cbSize = sizeof(format);
	format.dwMask = CFM_CHARSET | CFM_SIZE;
	format.bCharSet = SHIFTJIS_CHARSET;
	format.yHeight = 200;
	strcpy(format.szFaceName, "ÇlÇr ÉSÉVÉbÉN");

	LRESULT ret = SendMessage(hwnd(), EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&format);
	if(ret == 0)
		return st::SYSTEM_ERROR;

	return true;
}


//########################################################
}// namespace ntk
