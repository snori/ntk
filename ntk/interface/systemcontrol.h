/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: systemcontrol.h,v 1.1 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_SYSTEMCONTROL_H__
#define __NTK_INTERFACE_SYSTEMCONTROL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_CONTROL_H__
#include <ntk/interface/control.h>
#endif


namespace ntk {


class SystemControl : public Control {
public:
	//
	// methods
	//
	NtkExport virtual ~SystemControl();

protected:
	//
	// methods
	//
	NtkExport SystemControl(
		const String& class_name,
		const Rect& frame,
		const String& name,
		const String& label,
		Message* message,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = ntk::app_color(),
		uint style = DEFAULT_STYLE,
		uint ex_style = DEFAULT_EX_STYLE);

};// class Control


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef SystemControl system_control_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::SystemControl NSystemControl;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::SystemControl ntk_system_control;
	#endif
#endif


#endif//EOH
