/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: defaultwindowcontrol.h,v 1.12 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_DEFAULTWINDOWCONTROL_H__
#define __NTK_INTERFACE_DEFAULTWINDOWCONTROL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NKT_INTERFACE_WINDOWCONTROL_H__
#include <ntk/interface/windowcontrol.h>
#endif


namespace ntk {


class View;


class DefaultWindowControl : public WindowControl {
public:
	//
	// methods
	//
	virtual Rect client_frame() const;
	virtual Rect client_bounds() const;

	virtual void set_look(uint look);
	virtual void set_feel(uint feel);
	virtual void set_flags(uint flags);

	virtual uint area_test(const Point& point, uint default_value) const;

};// class DefaultWindowControl


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef DefaultWindowControl default_window_control_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::DefaultWindowControl NDefaultWindowControl;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::DefaultWindowControl     ntk_default_window_control;
	#endif
#endif


#endif//EOH
