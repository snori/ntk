/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dialogpanel.h,v 1.6 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_DIALOGPANEL_H__
#define __NTK_INTERFACE_DIALOGPANEL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_PANEL_H__
#include <ntk/interface/panel.h>
#endif


namespace ntk {


class DialogPanel : public Panel {
public:
	//
	// constants
	//
	enum {
		RESIZE_TO_FIT = Panel::FLAG_LAST << 1,
		FLAG_LAST     = Panel::FLAG_LAST << 2
	};

	//
	// methods
	//
	NtkExport DialogPanel(
		const char_t* resource_id,
		const Rect& frame,
		const String& name,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = app_color());

	HWND handle() const {return m_dialog_handle;}

private:
	//
	// data
	//
	HWND m_dialog_handle;

};// class DialogPanel


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef DialogPanel dialog_panel_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::DialogPanel NDialogPanel;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::DialogPanel ntk_dialog_panel;
	#endif
#endif


#endif//EOH
