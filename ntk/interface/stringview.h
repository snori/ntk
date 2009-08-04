/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: stringview.h,v 1.5 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_STRINGVIEW_H__
#define __NTK_INTERFACE_STRINGVIEW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_CONTROL_H__
#include <ntk/interface/control.h>
#endif


namespace ntk {


class StringView : public Control {
public:
	//
	// constants
	//
	enum {
		ALIGN_LEFT    = 1 << 0,
		ALIGN_RIGHT   = 2 << 0,
		ALIGN_CENTER  = 3 << 0,
		ALIGN_TOP     = 1 << 8,
		ALIGN_BOTTOM  = 2 << 8,
		ALIGN_VCENTER = 3 << 8,

		HORIZONTAL_ALIGN_MASK = 0x00ff,
		VERTICAL_ALIGN_MASK   = 0xff00,
	};// alignment

	//
	// methods
	//
	NtkExport StringView(
		const Rect& frame,
		const String& name,
		const String& text,
		uint mode = FOLLOW_LEFT | FOLLOW_TOP,
		uint flags = 0,
		const RGBColor& color = app_color());
	NtkExport virtual ~StringView();

	//
	// accessors and manipulator
	//
	NtkExport virtual String text() const;
	NtkExport virtual void set_text(const String& text);

	NtkExport virtual uint alignment() const;
	NtkExport virtual void set_alignment(uint alignment);

	//
	// message handlers
	//
	NtkExport virtual void draw(PaintDC& dc);

private:
	//
	// data
	//
	uint m_alignment, m_valignment;
	coord m_text_height;// cache

	//
	// functions
	//
	void calc_text_height_();

};// class StringView


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef StringView string_view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::StringView NStringView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::StringView ntk_string_view;
	#endif
#endif


#endif//EOH
