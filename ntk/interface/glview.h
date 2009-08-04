/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: glview.h,v 1.9 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_GLVIEW_H__
#define __NTK_INTERFACE_GLVIEW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


class GLView : public View {
public:
	//
	// types
	//
	struct status : public st {
		//
		// constants
		//
		enum {
			ALREADY_INITIALIZED        = NTK_ERROR_CODE(st, 1),
			FAILED_TO_SET_PIXEL_FORMAT = NTK_ERROR_CODE(st, 2),

			ERROR_LAST = NTK_ERROR_CODE(st, 10)
		};

		//
		// functions
		//
		NtkExport static String default_message(int what);
	};

	typedef Status<status> status_t;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef status_t status_t;
	#endif

	//
	// constants
	//
	enum {
		DOUBLE_BUFFERING = View::FLAG_LAST << 0,
		FLAG_LAST        = View::FLAG_LAST << 1,
	};//flags

	enum {
		DEFAULT_FLAGS = View::DEFAULT_FLAGS | WILL_DRAW | FRAME_EVENTS | DOUBLE_BUFFERING,
	};

	//
	// methods
	//
	NtkExport GLView(
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = ntk::transparent_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE);
	NtkExport virtual ~GLView();

	//
	// accessors
	//
	NtkExport HGLRC rc() const;

	//
	// message handlers
	//
	NtkExport virtual void draw(PaintDC& dc);
	NtkExport virtual void draw_scene();

	NtkExport virtual void attached_to_window();
	NtkExport virtual void frame_resized(coord width, coord height);

private:
	//
	// data
	//
	DC* m_dc;
	HGLRC m_rc;

	//
	// functions
	//
	status_t initialize_();
	void finalize_();

};// class GLView


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef GLView gl_view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::GLView NGLView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::GLView ntk_gl_view;
	#endif
#endif


#endif//EOH
