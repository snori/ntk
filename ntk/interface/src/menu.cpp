/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: menu.cpp,v 1.6 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/menu.h"


#include <assert.h>
#include <vector>
#include <ntk/windows/windows.h>
#include <ntk/interface/menuitem.h>
#include "menuwindow.h"


namespace ntk {


//########################################################


enum{
	END_SELECTING = '_ESL',
};//massages for menu


const Menu::MenuLayout Menu::DEFAULT_LAYOUT = Menu::ITEMS_IN_COLUMN;


//########################################################
// public methods

Menu::Menu(const String& name, MenuLayout layout, const RGBColor& color)
:	View(Rect(0, 0, 10, 10), name, FOLLOW_LEFT | FOLLOW_TOP, DEFAULT_FLAGS, color)
,	m_menu_layout(layout)
,	m_selecting(false)
,	m_opened_submenu(NULL)
,	m_owner_menu_item(NULL)
{
}

Menu::Menu(const Rect& frame, const String& name, uint mode, uint flags, MenuLayout layout, const RGBColor& color)
:	View(frame, name, mode, flags, color)
,	m_menu_layout(layout)
,	m_selecting(false)
,	m_opened_submenu(NULL)
,	m_owner_menu_item(NULL)
{
}

Menu::~Menu()
{
	while(! m_item_list.empty())
	{
		delete m_item_list.back();
		m_item_list.pop_back();
	}
}

bool
Menu::add_item(Menu* menu_, int index)
{
	if(menu_ == NULL)
		return false;

	MenuItem* item = new MenuItem(menu_);
	menu_->m_owner_menu_item = item;

	return add_item(item);
}

bool
Menu::add_item(MenuItem* item, int index)
{
	if(item == NULL)
		return false;

	if(index < 0)
		m_item_list.push_back(item);
	else
		m_item_list.insert(m_item_list.begin() + index, item);

	item->m_menu = this;

	if(is_submenu())
	{
		Looper* looper = NULL;
		Handler* handler = owner_menu_item()->target(&looper);

		item->set_target(handler, looper);
	}
	else item->set_target(NULL, window());

	reposition();

	return true;
}

void
Menu::add_separator_item()
{
	add_item(new SeparatorItem());
}

void
Menu::reposition()
{
	switch(m_menu_layout)
	{
	case ITEMS_IN_COLUMN:
		reposition_by_column_();
		break;
	case ITEMS_IN_ROW:
		reposition_by_row_();
		break;
	case ITEMS_IN_MATRIX:
		reposition_by_matrix_();
		break;
	}
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

Menu::MenuLayout
Menu::menu_layout() const
{
	return m_menu_layout;
}

status_t
Menu::set_target(Handler* target_, Looper* looper_)
{
	status_t sts;
	bool error = false;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
		if(! (sts=(*it)->set_target(target_, looper_)))
			error = true;

	return error ? st::ERR : st::OK;
}

MenuItem*
Menu::owner_menu_item() const
{
	return m_owner_menu_item;
}

bool
Menu::is_submenu() const
{
	return m_owner_menu_item != NULL;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message hnalder

void
Menu::attached_to_window()
{
	if(dynamic_cast<MenuWindow*>(window()))
	{
		// サブメニューだったらデフォルトで選択状態。target の設定はしてはいけない
		begin_selecting_();
	}
	else
	{
		// トップレベルのメニュー（コントロールとしてウィンドウに貼り付けられる） のときは
		// target の looper が正しく設定されるように再設定する。
		for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
			(*it)->set_target(NULL, window());
	}

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
		(*it)->attached_to_window();
}

void
Menu::detached_from_window()
{
	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
		(*it)->detached_from_window();
}

void
Menu::mouse_down(const Point& point, uint buttons, bool dbl)
{
	if(m_selecting == false)
	{
		begin_selecting_();
		selecting_(point);

//		HANDLE track_thread = spawn_thread(track_mouse_, "MouseTracker", NORMAL_PRIORITY, this);
//		resume_thread(track_thread);
	}
	else select_menu_item_(point);
}

void
Menu::mouse_up(const Point& point, uint buttons)
{
	select_menu_item_(point);
}

void
Menu::mouse_moved(const Point& point, uint buttons, uint transit, const Message* data)
{
	if(m_selecting)
		selecting_(point);
}

void
Menu::draw(PaintDC& dc)
{
	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
	{
		dc.push_state();
		(*it)->draw(dc);
		dc.pop_state();
	}
}

void
Menu::message_received(const Message& message)
{
	switch(message.what)
	{
	case END_SELECTING:
		end_selecting_();
		close_submenu_();
		break;

	default:
		View::message_received(message);
	}
}

//********************************************************
// private methods

void
Menu::reposition_by_column_()
{
	Rect rect = bounds();
	coord width, height, max_width = 0, last_bottom = rect.top;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
	{
		MenuItem* item = *it;
		assert(item);

		item->get_content_size(&width, &height);
		if(width > max_width) max_width = width;

		item->m_frame.left		= rect.left;
		item->m_frame.top			= last_bottom;
		item->m_frame.right		= rect.right;
		item->m_frame.bottom	= last_bottom = item->m_frame.top + height;
	}

	if(flags() & RESIZE_TO_FIT)
	{
		resize_to(max_width, last_bottom - rect.top);
		rect = bounds();

		for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
			(*it)->m_frame.right = rect.right;
	}
}

void
Menu::reposition_by_row_()
{
	Rect rect = bounds();
	coord width, height, max_height = 0, last_right = rect.top;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
	{
		MenuItem* item = *it;
		assert(item);

		item->get_content_size(&width, &height);
		if(height > max_height) max_height = height;

		item->m_frame.left		= last_right;
		item->m_frame.top			= rect.top;
		item->m_frame.right		= last_right = item->m_frame.left + width;
		item->m_frame.bottom	= rect.bottom;
	}

	if(flags() & RESIZE_TO_FIT)
	{
		resize_to(last_right - rect.left, max_height);
		rect = bounds();

		for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
			(*it)->m_frame.bottom = rect.bottom;
	}
}

void
Menu::reposition_by_matrix_()
{
}

void
Menu::begin_selecting_()
{
	m_selecting = true;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
		(*it)->m_selected = false;
}

void
Menu::end_selecting_()
{
	m_selecting = false;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
	{
		MenuItem* item = *it;

		if(item->m_selected)
			item->m_selected = false;

		if(item->submenu())
			item->submenu()->end_selecting_();
	}

	invalidate();
}

void
Menu::selecting_(const Point& point)
{
	MenuItem *selected = NULL, *deselected = NULL;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
	{
		MenuItem* item = *it;
		if(item->m_frame.contains(point))
		{
			if(item->m_selected == false)
				selected = item;
		}
		else if(item->m_selected)
			deselected = item;
	}

	if(deselected)// && selected)
	{
		deselected->m_selected = false;
		deselected->draw(dc());

		close_submenu_();

		end_capture();
	}

	if(selected)
	{
		selected->m_selected = true;
		selected->draw(dc());

		if(selected->submenu())
		{
			m_opened_submenu = selected->submenu();
			MenuWindow* window = new MenuWindow(m_opened_submenu);

			Point submenu_point;
			switch(menu_layout())
			{
			case Menu::ITEMS_IN_COLUMN:
				submenu_point.x = frame().right;
				submenu_point.y = selected->frame().top;
				break;

			case Menu::ITEMS_IN_ROW:
				submenu_point.x = selected->frame().left;
				submenu_point.y = frame().bottom;
				break;
			}

			convert_to_screen(&submenu_point);

			window->go(submenu_point);
		}
		else// サブメニューを持たない NMenuItem はマウスが NMenu の範囲の外に出た時点でもセレクト状態を解除する。
		{
			begin_capture();
		}
	}
}

void
Menu::close_submenu_()
{
	if(m_opened_submenu)
	{
		m_opened_submenu->close_submenu_();

		m_opened_submenu->window()->quit();
		m_opened_submenu = NULL;
	}
}

void
Menu::select_menu_item_(const Point& point)
{
	MenuItem* item = NULL;

	for(ItemList::iterator it = m_item_list.begin(); it != m_item_list.end(); ++it)
		if((*it)->m_frame.contains(point) && (*it)->submenu() == NULL)
		{
			item = *it;

			// トップのメニュー階層を見つけ、全部閉じる。
			Menu* menu = item->menu();
			while(menu->owner_menu_item())
				menu = menu->owner_menu_item()->menu();

			menu->end_selecting_();
			menu->close_submenu_();

			end_capture();

			item->invoke();

			break;
		}
}

uint
Menu::track_mouse_(void* data)
{
	Menu* root_menu = reinterpret_cast<Menu*>(data);

	Point point;
	uint buttons;

	do {
		ntk::get_mouse(&point, &buttons);
		snooze(20);

	} while(buttons & LEFT_MOUSE_BUTTON);

	Messenger messenger(root_menu);
	messenger.send_message(Message(END_SELECTING));

	return 0;
}


//########################################################
}// namespace ntk
