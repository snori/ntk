/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: textview.h,v 1.6 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_TEXTVIEW_H__
#define __NTK_INTERFACE_TEXTVIEW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_SYSTEMCONTROL_H__
#include <ntk/interface/systemcontrol.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class TextView : public SystemControl {
public:
	//
	//
	//
	enum {
		AUTO_V_SCROLL = SystemControl::FLAG_LAST << 1,
		AUTO_H_SCROLL = SystemControl::FLAG_LAST << 2,
		MULTI_LINE    = SystemControl::FLAG_LAST << 3,
		WANT_RETURN   = SystemControl::FLAG_LAST << 4,
		READ_ONLY     = SystemControl::FLAG_LAST << 5,

		FLAG_LAST     = SystemControl::FLAG_LAST << 6,
	};

	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_LEFT | FOLLOW_TOP,
		DEFAULT_FLAGS = NAVIGABLE | AUTO_V_SCROLL | AUTO_H_SCROLL | MULTI_LINE | WANT_RETURN,
	};

	//
	// methods
	//
	NtkExport TextView(
		const Rect& frame,
		const String& name,
		const String& text,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS);

	//void set_cursor(
	NtkExport virtual void print(const char_t* str);
	NtkExport virtual void print(const String& str);
	NtkExport virtual void printf(const char_t* format, ...);

	//
	// accessors and manipulators
	//
	NtkExport virtual String text() const;
	NtkExport virtual void set_text(const String& text);

	NtkExport virtual void get_line(int index, String* buf);

protected:
	//
	// methods
	//
	NtkExport TextView(
		const String& window_class,
		const Rect& frame,
		const String& name,
		const String& text,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		uint style = 0,
		uint ex_style = 0);

};// class TextView


class RichTextView : public TextView {
public:
	//
	// methods
	//
	NtkExport RichTextView(
		const Rect& frame,
		const String& name,
		const String& text,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS);

private:
	//
	// functions
	//
	status_t initialize_();

};// class RichTextView


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef TextView test_view_t;
	typedef RichTextView rich_text_view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::TextView NTextView;
	typedef ntk::RichTextView NRichTextView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::TextView ntk_text_view;
	typedef ntk::RichTextView ntk_rich_text_view;
	#endif
#endif


#endif//EOH
