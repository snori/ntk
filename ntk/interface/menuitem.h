/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menuitem.h,v 1.5 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_MENUITEM_H__
#define __NTK_INTERFACE_MENUITEM_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_INVOKER_H__
#include <ntk/application/invoker.h>
#endif

#ifndef __NTK_AUPPORT_ARCHIVABLE_H__
#include <ntk/support/archivable.h>
#endif

#ifndef __NTK_INTERFACE_RECT_H__
#include <ntk/interface/rect.h>
#endif


namespace ntk {


class DC;
class Menu;


class MenuItem : public Invoker, public Archivable {
public:
	//
	// methods
	//
	NtkExport MenuItem(
		const String& label,
		Message* message,
		char_t shotcut = 0,
		uint modifiers = 0);
	NtkExport MenuItem(
		Menu* submenu,
		Message* message = NULL,
		char_t shortcut = 0,
		uint modifiers = 0);
	NtkExport virtual ~MenuItem();

	NtkExport void invalidate();

	//
	// accessors and manipulators
	//
	NtkExport virtual Menu* menu() const;// parent menu
	NtkExport virtual Rect frame() const;

	NtkExport virtual String label() const;
	NtkExport virtual void set_label(const String& label);

	NtkExport virtual char_t shortcut() const;
	NtkExport virtual void set_shortcut(char_t shortcut);

	NtkExport virtual uint modifiers() const;
	NtkExport virtual void set_modifiers(uint modifiers);

	NtkExport virtual Menu* submenu() const;

	NtkExport virtual bool is_selected() const;

	NtkExport virtual status_t set_target(Handler* target_, Looper* looper_ = NULL);

	//
	// message handlers
	//
	NtkExport virtual void attached_to_window();
	NtkExport virtual void detached_from_window();

	NtkExport virtual void draw(DC& dc);
	NtkExport virtual void draw_content(DC& dc);
	NtkExport virtual void highlight(DC& dc, bool state);

	NtkExport virtual void get_content_size(coord* width, coord* height) const;

protected:
	//
	// constants
	//
	enum{
		H_SPACE = 5,
		V_SPACE = 2,
	};

private:
	//
	// data
	//
	Menu* m_menu;
	Rect m_frame;
	String m_label;
	char_t m_shortcut;
	uint m_modifiers;
	Menu* m_submenu;

	bool m_selected;

	//
	// friend
	//
	friend class Menu;

};// class MenuItem


class SeparatorItem : public MenuItem {
public:
	//
	// methods
	//
	NtkExport SeparatorItem();
	NtkExport virtual ~SeparatorItem();

	//
	// message handlers
	//
	NtkExport virtual void draw_content(DC& dc);
	NtkExport virtual void highlight(DC& dc, bool state);
	NtkExport virtual void get_content_size(coord* width, coord* height) const;

};// class SeparatorItem


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef MenuItem      menu_item_t;
	typedef SeparatorItem separator_item_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::MenuItem      NMenuItem;
	typedef ntk::SeparatorItem NSeparatorItem;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::MenuItem      ntk_menu_item;
	typedef ntk::SeparatorItem ntk_separator_item;
	#endif
#endif


#endif//EOH
