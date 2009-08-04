/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: scintilla.cpp,v 1.9 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#include <ntk/config.h>
#ifdef NTK_ENABLE_SCINTILLA


#define NTK_BUILD
#include "ntk/interface/scintilla.h"
#include <assert.h>
#include <boost/scoped_array.hpp>
#include <ntk/windows/windows.h>
#include <richedit.h>
#include <ntk/support/status.h>
#include <ntk/interface/gdiobject.h>
#include <ntk/misc/scintilla/scintilla.h>


#define for if(false); else for


namespace ntk {


//########################################################


namespace {

HMODULE g_scintilla_module = NULL;
int g_ref_count = 0;

String
scintilla_window_class()
{
	if(g_ref_count == 0)
	{
		g_scintilla_module = ::LoadLibrary("Scintilla.dll");
		if(g_scintilla_module == NULL)
		{
			error(status_t(st::SYSTEM_ERROR).message() + "['Scintilla.dll']");
			return "";
		}
	}

	g_ref_count++;

	return "Scintilla";
}

void
clean_up_scintilla()
{return;
	g_ref_count--;

	if(g_ref_count == 0)
	{
		::FreeLibrary(g_scintilla_module);
		g_scintilla_module = NULL;
	}
}

}// anonymous namespace


//########################################################
// public methods

Scintilla::Scintilla(const Rect& frame, const String& name, const String& text, uint mode, uint flags, const RGBColor& color, uint style, uint ex_style)
:	Control(scintilla_window_class(), frame, name, text, NULL, mode, flags, transparent_color(), style, ex_style)
{
	set_flags(flags);// コンストラクタの中では仮想関数が正しく動作しない為

	send_message_(SCI_SETVSCROLLBAR, style & WS_VSCROLL);
	send_message_(SCI_SETHSCROLLBAR, style & WS_HSCROLL);

	set_tab_width(2);
	set_font(Font("ＭＳ ゴシック", 10));
	set_view_color(color);
	set_text_color(RGBColor(0, 0, 0));

	send_message_(SCI_SETTABINDENTS, true);
	send_message_(SCI_SETMARGINWIDTHN, 1, 0);

	send_message_(SCI_STYLESETBACK, STYLE_LINENUMBER, (view_color() - 25).ref());
	send_message_(SCI_STYLESETFORE, STYLE_LINENUMBER, (ntk::white_color() - (view_color() - 25)).ref());
}

Scintilla::~Scintilla()
{
	clean_up_scintilla();
}

void
Scintilla::insert(const String& text)
{
//	int start, finish;
//	get_selection(&start, &finish);

//	select(start);
	send_message_(EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(text.c_str()));
}

void
Scintilla::insert(const String& text, int length)
{
	insert(String(text.c_str(), text.length() < length ? text.length() : length));
}

void
Scintilla::insert(int offset, const String& text, int length)
{
	select(offset);
	insert(text, length);
}

void
Scintilla::erase()
{
	send_message_(EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(""));
}

void
Scintilla::erase(int start, int finish)
{
	select(start, finish);
	erase();
}

void
Scintilla::go_to_line(int line_no)
{
	send_message_(SCI_GOTOLINE, line_no);
}

void
Scintilla::cut(Clipboard* clipboard)
{
	send_message_(SCI_CUT);
}

void
Scintilla::copy(Clipboard* clipboard) const
{
	send_message_(SCI_COPY);
}

void
Scintilla::paste(Clipboard* clipboard)
{
	send_message_(SCI_PASTE);
}

void
Scintilla::clear()
{
	send_message_(SCI_CLEAR);
}

bool
Scintilla::accepts_paste(Clipboard* clipboard) const
{
	return send_message_(SCI_CANPASTE) != FALSE;
}

bool
Scintilla::accepts_drop(const Message& message) const
{
	return false;
}

bool
Scintilla::undoable() const
{
	return send_message_(SCI_CANUNDO) != FALSE;
}

void
Scintilla::undo()
{
	send_message_(SCI_UNDO);
}

void
Scintilla::redo()
{
	send_message_(SCI_REDO);
}

void
Scintilla::zoom_in()
{
	send_message_(SCI_ZOOMIN);
}

void
Scintilla::zoom_out()
{
	send_message_(SCI_ZOOMOUT);
}

int
Scintilla::zoom_factor() const
{
	return send_message_(SCI_GETZOOM);
}

void
Scintilla::set_zoom(int factor)
{
	send_message_(SCI_SETZOOM, factor);
}

void
Scintilla::search_next(const String& word)
{
	int a = anchor_pos();
	int c = current_pos();
	OutputDebugString(format_string("[%d-%d]\n", a, c).c_str());

	select(anchor_pos());
	send_message_(SCI_SEARCHANCHOR);
	send_message_(SCI_SEARCHNEXT, SCFIND_MATCHCASE, reinterpret_cast<LPARAM>(word.c_str()));
	
	send_message_(SCI_SCROLLCARET);
}

void
Scintilla::search_prev(const String& word)
{
	int a = anchor_pos();
	int c = current_pos();
	OutputDebugString(format_string("[%d-%d] (reverse)\n", a, c).c_str());

	select(current_pos() -1);
	send_message_(SCI_SEARCHANCHOR);
	send_message_(SCI_SEARCHPREV, SCFIND_MATCHCASE, reinterpret_cast<LPARAM>(word.c_str()));
	
	send_message_(SCI_SCROLLCARET);
}

//--------------------------------------------------------
// public accessors and manipulators

String
Scintilla::text() const
{
	int len = text_length() +1;
	boost::scoped_array<char_t> buf(new char_t[len]);

	send_message_(SCI_GETTEXT, len, reinterpret_cast<LPARAM>(&buf[0]));

	return &buf[0];
}

String
Scintilla::text(int offset, int length) const
{
	length += 1;

	boost::scoped_array<char_t> buf(new char_t[length]);
	get_text(offset, length, &buf[0]);

	return &buf[0];
}

void
Scintilla::get_text(int offset, int length, char_t* buffer) const
{
	int start, finish;
	get_selection(&start, &finish);

	const_cast<Scintilla*>(this)->select(offset, offset + length);
	send_message_(SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(buffer));

	const_cast<Scintilla*>(this)->select(start, finish);
}

void
Scintilla::set_text(const char_t* text)
{
	send_message_(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
}

void
Scintilla::set_text(const String& text)
{
	set_text(text.c_str());
}

int
Scintilla::text_length() const
{
	return send_message_(SCI_GETTEXTLENGTH);
}

int
Scintilla::count_lines() const
{
	return send_message_(SCI_GETLINECOUNT);
}

int
Scintilla::current_line() const
{
	return send_message_(SCI_LINEFROMPOSITION, send_message_(SCI_GETCURRENTPOS));
}

int
Scintilla::anchor_pos() const
{
	return send_message_(SCI_GETANCHOR);
}

void
Scintilla::set_anchor_pos(int pos)
{
	send_message_(SCI_SETANCHOR, pos);
}

int
Scintilla::current_pos() const
{
	return send_message_(SCI_GETCURRENTPOS);
}

void
Scintilla::set_current_pos(int pos)
{
	send_message_(SCI_SETCURRENTPOS, pos);
}

void
Scintilla::get_selection(int* start, int* finish) const
{
	*start  = anchor_pos();
	*finish = current_pos();
}

void
Scintilla::select(int start, int finish)
{
	if(start < 0)
		start = END_OF_BUFFER;

	set_anchor_pos(start);

	if(finish == SAME_AS_START)
		finish = start;
	else if(finish < 0)
		finish = END_OF_BUFFER;

	set_current_pos(finish);
}

void
Scintilla::select_all()
{
	send_message_(SCI_SELECTALL);
}

int
Scintilla::tab_width() const
{
	return send_message_(SCI_GETTABWIDTH);
}

void
Scintilla::set_tab_width(int width)
{
	send_message_(SCI_SETTABWIDTH, width);
}

const Font&
Scintilla::font() const
{
	return m_font;
}

void
Scintilla::set_font(const Font& font)
{
	m_font = font;

	send_message_(SCI_STYLESETFONT, STYLE_DEFAULT, reinterpret_cast<LPARAM>(m_font.face().c_str()));
	for(int i=0; i<=STYLE_MAX; ++i)
	{
		send_message_(SCI_STYLESETFONT, i, reinterpret_cast<LPARAM>(m_font.face().c_str()));
		send_message_(SCI_STYLESETSIZE, i, font.size());
		send_message_(SCI_STYLESETCHARACTERSET, i, static_cast<LPARAM>(SHIFTJIS_CHARSET));
	}
}

const RGBColor&
Scintilla::view_color() const
{
	return m_view_color;
}

void
Scintilla::set_view_color(const RGBColor& color)
{
	m_view_color = color;

	for(int i=0; i<=STYLE_MAX; ++i)
		send_message_(SCI_STYLESETBACK, i, static_cast<LPARAM>(color.ref()));
}

const RGBColor&
Scintilla::text_color() const
{
	return m_text_color;
}

void
Scintilla::set_text_color(const RGBColor& color)
{
	m_text_color = color;

	for(int i=0; i<=STYLE_MAX; ++i)
		send_message_(SCI_STYLESETFORE, i, static_cast<LPARAM>(color.ref()));
}

bool
Scintilla::is_modified() const
{
	return send_message_(SCI_GETMODIFY) != 0;
}

void
Scintilla::clear_modification_flag()//bool flag)
{
//	if(flag == false)
		send_message_(SCI_SETSAVEPOINT);
//	else
//		send_message_();
}

void
Scintilla::set_flags(uint flags)
{
	Control::set_flags(flags);

	if(flags & SHOW_LINE_NUMBER)
		send_message_(SCI_SETMARGINWIDTHN, 0, 56);
	else
		send_message_(SCI_SETMARGINWIDTHN, 0, 0);
}

//--------------------------------------------------------
// functions

LRESULT
Scintilla::send_message_(UINT message, WPARAM wp, LPARAM lp) const
{
	return SendMessage(hwnd(), message, wp, lp);
}


//########################################################
}// namespace ntk


#endif// NTK_ENABLE_SCINTILLA
