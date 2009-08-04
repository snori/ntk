/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: tabbar.h,v 1.5 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_TABBAR_H__
#define __NTK_INTERFACE_TABBAR_H__


#include <vector>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_CONTROL_H__
#include <ntk/interface/control.h>
#endif


namespace ntk {


class TabBar : public Control {
public:
	//
	// constants
	//
	enum {
		TAB_CHANGED = '_CNG',
	};

	enum {
		DEFAULT_FLAGS = Control::DEFAULT_FLAGS | WILL_DRAW,
	};

	//
	// methods
	//
	NtkExport TabBar(
		const Rect& frame,
		const String& name,
		Message* message = new Message(TAB_CHANGED),
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = ntk::app_color() -25);
	NtkExport virtual ~TabBar();

	NtkExport virtual bool add_tab(const String& label, int index = -1);// -1: add to last position
	NtkExport virtual bool remove_tab(const String& label);
	NtkExport virtual bool remove_tab(uint index);

	NtkExport virtual const String& active_tab() const;
	NtkExport virtual void activate_tab(const String& name);
	NtkExport virtual void activate_tab(uint index);

	NtkExport virtual void mouse_down(const Point& point, uint buttons, bool double_clicked);
	NtkExport virtual void mouse_up(const Point& point, uint buttons);
	NtkExport virtual void mouse_moved(const Point& point, uint buttons, uint transit, const Message* data);
	NtkExport virtual void draw(PaintDC& dc);

private:
	//
	// types
	//
	class Tab {
	public:
		Tab(const Rect& frame, const String& name);

		const Rect& frame() const;
		const String& name() const;
		bool is_active() const;

		void draw(DC& dc);

	private:
		Rect m_frame;
		String m_name;
		bool m_active;

		friend class TabBar;

	};// class Tab

	typedef ::std::vector<Tab> TabList;

	//
	// data
	//
	TabList m_tabs;

	//
	// functions
	//
	void reposition_();

};// class TabBar


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef TabBar tab_bar_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
		typedef ntk::TabBar NTabBar;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef ntk::TabBar ntk_tab_bar;
	#endif
#endif


#endif//EOH
