/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: scrollview.h,v 1.4 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_SCROLLVIEW_H__
#define __NTK_INTERFACE_SCROLLVIEW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif


namespace ntk {


class ScrollBar;


class ScrollView : public View {
public:
	//
	// methods
	//
	NtkExport ScrollView(
		const String& name,
		View* target,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		bool v_scroll_bar = false,
		bool h_scroll_bar = false,
		const RGBColor& color = transparent_color());
	NtkExport virtual ~ScrollView();

	//
	// accessors and manipulators
	//
	NtkExport virtual View* target() const;
	NtkExport virtual void set_target(View* target);

private:
	//
	// data
	//
	View* m_target;
	ScrollBar *m_v_scroll_bar, *m_h_scroll_bar;

};// class ScrollView


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ScrollView scroll_view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::ScrollView NScrollView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::ScrollView ntk_scroll_view;
	#endif
#endif


#endif//EOH
