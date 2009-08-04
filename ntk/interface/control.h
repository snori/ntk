/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: control.h,v 1.9 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_CONTROL_H__
#define __NTK_INTERFACE_CONTROL_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif

#ifndef __NTK_APPLICATION_INVOKER_H__
#include <ntk/application/invoker.h>
#endif


namespace ntk {


class Control : public View, public Invoker {
public:
	//
	// types
	//
	class ClickTracker {
	public:
		//
		// methods
		//
		NtkExport ClickTracker(Control* control = NULL);
		NtkExport virtual ~ClickTracker();

		//
		// accessors
		//
		NtkExport Control* target() const;
		NtkExport void set_target(Control* target);

		NtkExport virtual bool is_clicking() const;

		//
		// message handlers
		//
		NtkExport virtual void mouse_down(const Point& point, uint buttons, bool double_clicked);
		NtkExport virtual void mouse_up(const Point& point, uint buttons);
		NtkExport virtual void mouse_moved(const Point& point, uint buttons, uint transit, const Message* data);
		NtkExport virtual void lost_capture();

	private:
		//
		// data
		//
		Control* m_control;
		bool m_pushing;

	};// class ClickTracker

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef ClickTracker click_tracker_t;
	#endif

	//
	// constants
	//
	enum {
		DEFAULT_FLAGS = View::DEFAULT_FLAGS | NAVIGABLE,
	};

	//
	// methods
	//
	NtkExport Control(
		const Rect& frame,
		const String& name,
		const String& label,
		Message* message,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = ntk::app_color());
	NtkExport virtual ~Control();

	//
	// accessors and manipulators
	//
	NtkExport virtual String label() const;
	NtkExport virtual void set_label(const String& label);

	NtkExport virtual int value() const;
	NtkExport virtual void set_value(int val);

	NtkExport virtual bool is_enabled() const;
	NtkExport virtual void set_enabled(bool enabled);

	//
	// message handlers
	//
	NtkExport virtual void attached_to_window();
	NtkExport virtual void focus_changed(bool state);
	NtkExport virtual LRESULT system_message_received(const Message& message);

protected:
	//
	// methods
	//
	NtkExport Control(
		const String& class_name,
		const Rect& frame,
		const String& name,
		const String& label,
		Message* message,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		const RGBColor& color = app_color(),
		uint style            = DEFAULT_STYLE,
		uint ex_style         = DEFAULT_EX_STYLE);

private:
	//
	// data
	//
	String m_label;
	int m_value;
	bool m_enabled;

	//
	// functions
	//
	void initialize_();
	void finalize_();

};// class Control


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Control control_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Control NControl;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Control ntk_control;
	#endif
#endif


#endif//EOH
