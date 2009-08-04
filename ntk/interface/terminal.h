/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: terminal.h,v 1.4 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_TERMINAL_H__
#define __NTK_INTERFACE_TERMINAL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_SCROLLVIEW_H__
#include <ntk/interface/scrollview.h>
#endif


namespace ntk {


class TerminalView;


class Terminal : public ScrollView {
public:
	//
	// methods
	//
	NtkExport Terminal(
		const Rect& frame,
		const String& name,
		uint mode = FOLLOW_LEFT | FOLLOW_TOP,
		uint flags = 0,
		bool v_scroll_bar = true,
		bool h_scroll_bar = false,
		const RGBColor& color = ntk::view_color());
	NtkExport Terminal(
		const String& name,
		TerminalView* terminal_view,
		uint mode = FOLLOW_LEFT | FOLLOW_TOP,
		uint flags = 0,
		bool v_scroll_bar = true,
		bool h_scroll_bar = false,
		const RGBColor& color = ntk::view_color());
	NtkExport virtual ~Terminal();

	TerminalView* terminal_view() const {return m_terminal_view;}

private:
	//
	// data
	//
	TerminalView* m_terminal_view;

};// class Terminal


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Terminal terminal_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Terminal NTerminal;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Terminal ntk_terminal;
	#endif
#endif


#endif//EOH
