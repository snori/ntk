/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: panel.h,v 1.10 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_PANEL_H__
#define __NTK_INTERFACE_PANEL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif

#include <ntk/interface/dc.h>
namespace ntk {


class MenuBar;


class Panel : public View {
public:
	//
	// constants
	//
	enum {
		COVER_WINDOW = View::FLAG_LAST << 0,
		FLAG_LAST    = View::FLAG_LAST << 1,
	};

	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_ALL_SIDES,
		DEFAULT_FLAGS         = View::DEFAULT_FLAGS | COVER_WINDOW,
	};

	//
	// methods
	//
	NtkExport Panel(
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = app_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE,
		HMENU hmenu           = NULL);
	NtkExport virtual ~Panel();

	NtkExport virtual status_t add_child(View* child);
	NtkExport virtual status_t remove_child(View* vhild);

	NtkExport virtual status_t add_child(Window* window);
	NtkExport virtual status_t remove_child(Window* window);

	NtkExport virtual MenuBar* key_menu_bar() const;
	NtkExport virtual void set_key_menu_bar(MenuBar* menu_bar);

private:
	//
	// data
	//
	MenuBar* m_key_menu_bar;

};// class Panel


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Panel panel_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Panel NPanel;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Panel ntk_panel;
	#endif
#endif


#endif//EOH
