/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: button.h,v 1.6 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_BUTTON_H__
#define __NTK_INTERFACE_BUTTON_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_SYSTEMCONTROL_H__
#include <ntk/interface/systemcontrol.h>
#endif


namespace ntk {


class Button : public SystemControl {
public:
	//
	// methods
	//
	NtkExport Button(
		const Rect& frame,
		const String& name,
		const String& label,
		Message* message,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = ntk::app_color());

	NtkExport virtual void set_label(const String& label);

};// class Button


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Button button_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Button NButton;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Button ntk_button;
	#endif
#endif


#endif//EOH
