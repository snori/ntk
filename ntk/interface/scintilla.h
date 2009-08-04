/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: scintilla.h,v 1.7 2003/11/11 12:07:06 nsuzuki Exp $

*******************************************************************************

	◆ How to use Scintilla Edit Control (http://www.scintilla.org/)

		I. 最新ソースからビルドする

		・まず、www.scintilla.org から最新のソース一式を入手してきます。
		・マニュアル通りにビルドしたらヘッダとライブラリのパスを追加します。
		・Scintilla.dll, SciLexer.dll をパスの通ったディレクトリにコピーします。

		II. 付属のファイルを利用する

		・まず ntk-x.x.x/misc/scintilla.zip を解凍します。
		  (ntk-x.x.x/scintilla/scintilla.h というファイルが出来るように)
		・scintilla.dll, scilexer.dll を bin/ にコピーします。

		以上で準備完了です。あとは <ntk/config.h> の中の NTK_USE_SCINTILLA マクロを
		有効にすれば利用可能になります。

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_SCINTILLA_H__
#define __NTK_INTERFACE_SCINTILLA_H__


#ifndef __NTK_CONFIG_H__
#include <ntk/config.h>
#endif

#ifdef NTK_ENABLE_SCINTILLA


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_CONTROL_H__
#include <ntk/interface/control.h>
#endif

#ifndef __NTK_INTERFACE_GDIOBJECT_H__
#include <ntk/interface/gdiobject.h>
#endif


namespace ntk {


class Scintilla : public Control {
public:
	//
	// constants
	//
	enum {
		SHOW_LINE_NUMBER = Control::FLAG_LAST << 1,
		FLAG_LAST        = Control::FLAG_LAST << 2,
	};

	enum {
		DEFAULT_STYLE = Control::DEFAULT_STYLE | WS_VSCROLL | //WS_HSCROLL |
			ES_AUTOVSCROLL | /*ES_AUTOHSCROLL |*/ ES_MULTILINE | ES_WANTRETURN,
	};

	enum {
		END_OF_BUFFER = 0x7fffffff,
		SAME_AS_START = -2,
	};

	//
	// methods
	//
	NtkExport Scintilla(
		const Rect& frame,
		const String& name,
		const String& text,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = ntk::view_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE);
	NtkExport virtual ~Scintilla();

	NtkExport virtual void insert(const String& text);
	NtkExport virtual void insert(const String& text, int length);
	NtkExport virtual void insert(int offset, const String& text, int length);
	NtkExport virtual void erase();
	NtkExport virtual void erase(int start, int finish);

	NtkExport virtual void go_to_line(int line_no);
class Clipboard;
	NtkExport virtual void cut(Clipboard* clipboard = NULL);
	NtkExport virtual void copy(Clipboard* clipboard = NULL) const;
	NtkExport virtual void paste(Clipboard* clipboard = NULL);
	NtkExport virtual void clear();

	NtkExport virtual bool accepts_paste(Clipboard* clipboard) const;
	NtkExport virtual bool accepts_drop(const Message& message) const;

	NtkExport virtual void undo();
	NtkExport virtual void redo();
	NtkExport virtual bool undoable() const;

	NtkExport virtual void zoom_in();
	NtkExport virtual void zoom_out();
	NtkExport virtual int zoom_factor() const;
	NtkExport virtual void set_zoom(int factor);

	NtkExport virtual void search_next(const String& word);
	NtkExport virtual void search_prev(const String& word);

	//
	// accessors and manipulators
	//
	NtkExport virtual String text() const;
	NtkExport virtual String text(int offset, int length) const;
	NtkExport virtual void get_text(int offset, int length, char_t* buffer) const;
	NtkExport virtual void set_text(const char_t* text);
	NtkExport virtual void set_text(const String& text);
	NtkExport virtual int text_length() const;

	NtkExport virtual int count_lines() const;
	NtkExport virtual int current_line() const;

	NtkExport virtual int anchor_pos() const;
	NtkExport virtual void set_anchor_pos(int pos);

	NtkExport virtual int current_pos() const;
	NtkExport virtual void set_current_pos(int pos);

	NtkExport virtual void get_selection(int* start, int* finish) const;
	NtkExport virtual void select(int start, int finish = SAME_AS_START);
	NtkExport virtual void select_all();

	NtkExport virtual int tab_width() const;
	NtkExport virtual void set_tab_width(int width);

	NtkExport virtual const Font& font() const;
	NtkExport virtual void set_font(const Font& font);

	NtkExport virtual const RGBColor& view_color() const;
	NtkExport virtual void set_view_color(const RGBColor& color);

	NtkExport virtual const RGBColor& text_color() const;
	NtkExport virtual void set_text_color(const RGBColor& color);

	NtkExport virtual bool is_modified() const;
	NtkExport virtual void clear_modification_flag();

	NtkExport virtual void set_flags(uint flags);

private:
	//
	// data
	//
	Font m_font;
	RGBColor m_view_color, m_text_color;

	//
	// functions
	//
	LRESULT send_message_(UINT message, WPARAM wp = 0, LPARAM lp = 0) const;

};// class Scintilla


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Scintilla scintilla_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Scintilla NScintilla;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Scintilla ntk_scintilla;
	#endif
#endif


#endif// NTK_ENABLE_SCINTILLA


#endif//EOH
