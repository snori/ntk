/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: htmlview.h,v 1.7 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_HTMLVIEW_H__
#define __NTK_INTERFACE_HTMLVIEW_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif

#ifndef __NTK_APPLICATION_INVOKER_H__
#include <ntk/application/invoker.h>
#endif

#ifndef __NTK_WINDOWS_ATL_H__
#include <ntk/windows/atl.h>
#endif


namespace ntk {


class HTMLView : public View, public Invoker {
public:
	//
	// constants
	//
	enum {
		ADDRESS_CHANGED = '_ADC',
		CAPTURE_PAGE    = '_CAP',
	};

	//
	// methods
	//
	NtkExport HTMLView(
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE);
	NtkExport virtual ~HTMLView();

	NtkExport virtual void go(const String& url);
	NtkExport virtual void refresh(uint refresh_level = REFRESH_NORMAL) const;
	NtkExport virtual void stop() const;

	NtkExport virtual void go_back();
	NtkExport virtual void go_forward();

	//
	// accessors and manipulators
	//
	NtkExport virtual String name() const;
	NtkExport virtual void set_name(const String& name);

	NtkExport virtual String current_address() const;

	NtkExport virtual String html() const;
	NtkExport virtual void set_html(const String& source);

	NtkExport virtual Rect page_bounds() const;

	//
	// message handlers
	//
	NtkExport virtual void attached_to_window();
	NtkExport virtual void message_received(const Message& message);
	NtkExport virtual LRESULT system_message_received(const Message& message);

private:
	//
	// data
	//
	String m_name;
	CComQIPtr<IWebBrowser2> m_web_browser;
	String m_current_address;

	//
	// friends
	//
	friend class HTMLViewSink;

};// class HTMLView


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef HTMLView html_view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::HTMLView NHTMLView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::HTMLView ntk_html_view;
	#endif
#endif


#endif//EOH
