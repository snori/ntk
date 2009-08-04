/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menu.h,v 1.10 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_NMENU_H__
#define __NTK_INTERFACE_NMENU_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif


namespace ntk {


class MenuItem;


class Menu : public View {
public:
	//
	// constants
	//
	enum MenuLayout {
		ITEMS_IN_COLUMN =0,
		ITEMS_IN_ROW,
		ITEMS_IN_MATRIX
	};

	enum {
		RESIZE_TO_FIT = View::FLAG_LAST << 0,
		FLAG_LAST     = View::FLAG_LAST << 1,
	};

	enum {
		DEFAULT_FLAGS = View::DEFAULT_FLAGS | WILL_DRAW | RESIZE_TO_FIT,
	};

	NtkExport static const MenuLayout DEFAULT_LAYOUT;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef MenuLayout menu_layout_t;
	#endif

	//
	// methods
	//
	NtkExport Menu(
		const String& name,
		MenuLayout layout     = DEFAULT_LAYOUT,
		const RGBColor& color = app_color());
	NtkExport Menu(
		const Rect& frame,
		const String& name,
		uint mode             = DEFAULT_RESIZING_MODE,
		uint flags            = DEFAULT_FLAGS,
		MenuLayout layout     = DEFAULT_LAYOUT,
		const RGBColor& color = app_color());
	NtkExport virtual ~Menu();

	NtkExport virtual bool add_item(Menu* menu, int index = -1);
	NtkExport virtual bool add_item(MenuItem* item, int index = -1);
	NtkExport virtual void add_separator_item();

	NtkExport virtual void reposition();

	//
	// accessors and manipulators
	//
	NtkExport virtual MenuLayout menu_layout() const;

	NtkExport virtual status_t set_target(Handler* target_, Looper* looper_ = NULL);

	NtkExport virtual MenuItem* owner_menu_item() const;
	NtkExport virtual bool is_submenu() const;

	//
	// message handlers
	//
	NtkExport virtual void attached_to_window();
	NtkExport virtual void detached_from_window();// -

	NtkExport virtual void mouse_down(const Point& point, uint buttons, bool double_clicked);
	NtkExport virtual void mouse_up(const Point& point, uint buttons);
	NtkExport virtual void mouse_moved(const Point& point, uint buttons, uint transit, const Message* data);

	NtkExport virtual void draw(PaintDC& dc);

	NtkExport virtual void message_received(const Message& message);

private:
	//
	// types
	//
	typedef std::vector<MenuItem*> ItemList;

	//
	// data
	//
	MenuLayout m_menu_layout;
	ItemList m_item_list;

	bool m_selecting;
	Menu* m_opened_submenu;
	MenuItem* m_owner_menu_item;

	//
	// methods
	//
	void reposition_by_column_();
	void reposition_by_row_();
	void reposition_by_matrix_();

	void begin_selecting_();
	void end_selecting_();
	void selecting_(const Point& point);
	void close_submenu_();
	void select_menu_item_(const Point& point);

	static uint track_mouse_(void* data);

	//
	// friend
	//
	friend class MenuWindow;

};// class Menu


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Menu menu_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Menu NMenu;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Menu ntk_menu;
	#endif
#endif


#endif//EOH
