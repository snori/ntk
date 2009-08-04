/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stdmenubar.h,v 1.8 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_STD_STDMENUBAR_H__
#define __NTK_INTERFACE_STD_STDMENUBAR_H__


#include <vector>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_MENUBAR_H__
#include <ntk/interface/menubar.h>
#endif

#ifndef __NTK_INTERFACE_STD_MENU_H__
#include <ntk/interface/std/menu.h>
#endif


namespace ntk {
namespace standard {


class Menu;
class MenuItem;


#if 0
// VC6 のバグのため・・・
class MenuBar___InheriteHelper : public View { //::ntk::MenuBar {
public:
	MenuBar___InheriteHelper(
		const String& window_class, const Rect& frame, const String& name,
		uint mode, uint flags, const RGBColor& color, uint style, uint ex_style)
	:	View(window_class, frame, name, mode, flags, color, style, ex_style) {}
};
#endif

class MenuBar : public View, public MenuOperator {
private:
	//
	// types
	//
	typedef View/*MenuBar___InheriteHelper*/ Super;

public:
	//
	// constants
	//
	enum {
		RESIZE_TO_FIT = View::FLAG_LAST << 0,
		FLAG_LAST     = View::FLAG_LAST << 1,
	};

	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_LEFT_RIGHT | FOLLOW_TOP,
		DEFAULT_FLAGS = Super::DEFAULT_FLAGS | RESIZE_TO_FIT,
		DEFAULT_STYLE = (Super::DEFAULT_STYLE & ~WS_CHILD) | WS_POPUP,
	};

	//
	// methods
	//
	NtkExport MenuBar(
		HMENU hmenu,
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = ntk::app_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE);
	NtkExport virtual ~MenuBar();

//	using MenuOperator::add_item(const Menu& submenu);
//	using MenuOperator::add_item(const MenuItem& item);

	//
	// message handlers
	//
	NtkExport void attached_to_window();
	NtkExport void detached_from_window();
	NtkExport LRESULT system_message_received(const Message& message);

private:
	//
	// hidden methods
	//
//	bool add_item(::ntk::Menu* submenu, int index) {return false;}
//	bool add_item(::ntk::MenuItem* item, int index) {return false;}

};// class MenuBar


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MenuBar menu_bar_t;
#endif


} namespace Standard = standard;


typedef standard::MenuBar StdMenuBar;

#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef StdMenuBar std_menu_bar_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::StdMenuBar NStdMenuBar;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::StdMenuBar ntk_std_menu_bar;
	#endif
#endif


#endif//EOH
