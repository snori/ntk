#include "editwindow.h"


#include <vector>
#include <ntk/storage/file.h>
#include <ntk/storage/entry.h>
#include <ntk/interface/scrollbar.h>
#include <ntk/interface/panel.h>
#include <ntk/interface/alert.h>
#include <ntk/interface/std/menubar.h>
#include <ntk/interface/statusbar.h>
#include <ntk/interface/filepanel.h>
#include <ntk/interface/scintilla.h>
#include "ted.h"
#include "resource.h"


#define WINDOW_TITLE(file_name) (ntk_string("TED") + " - [" + (file_name) + "]")


//########################################################


edit_window_t::edit_window_t()
:	ntk_window(
		ted()->conf().get_rect("edit_window_t::frame", NRect(100, 100, 600, 500)),
		WINDOW_TITLE("untitled"), TITLED_WINDOW)
,	m_file_name("")
{
	set_icon_handle(LoadIcon(ntk::application()->handle(), MAKEINTRESOURCE(ID_TED_ICON)));

	panel()->set_view_color(ntk::transparent_color());

	ntk_std_menu_bar* menu_bar = new ntk_std_menu_bar(
		LoadMenu(ted()->handle(), MAKEINTRESOURCE(ID_APP_MENU)),
		ntk_rect(0, 0, 10, 10), "Menu");
	add_child(menu_bar);

	int width_list[] = {100, 100, 200};
	ntk_status_bar* status_bar = new ntk_status_bar("TestStatusBar");//, 3, width_list);
	add_child(status_bar);

//	add_child(new NScrollBar(bounds().inset_by_copy(10, 10), "VScrollBar", NULL, 0, 50));

	ntk_rect rect = bounds();
	rect.top += menu_bar->frame().height();
	rect.bottom -= status_bar->frame().height();

	m_edit = new ntk_scintilla(rect, "Text", "", ntk_view::FOLLOW_ALL_SIDES);
//	m_edit->set_ex_style(m_edit->ex_style() | WS_EX_CLIENTEDGE, true);
	add_child(m_edit);
}

edit_window_t::~edit_window_t()
{
	ted()->conf().set_rect("edit_window_t::frame", frame());
}

bool
edit_window_t::quit_requested()
{
	ted()->quit();
	return true;
}

void
edit_window_t::message_received(const ntk_message& message)
{
	switch(message.what)
	{
	case ntk::REFS_RECEIVED:
		m_file_name = message.find_string("name");
		load_file_(m_file_name);
		break;

	case ntk::SAVE_REQUESTED:
		m_file_name = message.find_string("name");
		save_file_(m_file_name);
		break;

	default:
		ntk_window::message_received(message);
	}
}

LRESULT
edit_window_t::system_command_received(uint id)
{
	switch(id)
	{
	case IDM_FILE_NEW:
		m_file_name = "";
		m_edit->set_text("");
		break;

	case IDM_FILE_OPEN:
		{
			ntk_file_panel p(ntk_file_panel::OPEN_PANEL, ntk_messenger(NULL, this));
			p.show();
		}
		break;

	case IDM_FILE_SAVE:
		if(m_file_name == "")
		{
			ntk_file_panel p(ntk_file_panel::SAVE_PANEL, ntk_messenger(NULL, this));
			p.show();
		}
		else
			save_file_(m_file_name, true);
		break;

	case IDM_FILE_SAVE_AS:
		{
			ntk_file_panel p(ntk_file_panel::SAVE_PANEL, ntk_messenger(NULL, this));
			p.show();
		}
		break;

	case IDM_FILE_EXIT:
		quit();
		break;

	case IDM_EDIT_CUT:
		m_edit->cut();
		break;

	case IDM_EDIT_COPY:
		m_edit->copy();
		break;

	case IDM_EDIT_PASTE:
		m_edit->paste();
		break;

	case IDM_EDIT_SELECT_ALL:
		m_edit->select_all();
		break;

	case IDM_HELP_ABOUT:
		ted()->post_message(ntk::ABOUT_REQUESTED);
		break;

	default:
		return ntk_window::system_command_received(id);
	}

	return 0;
}


//########################################################


void
edit_window_t::load_file_(const ntk_string& file_name)
{
	ntk_FILE file(file_name, "rb");
	if(file.init_check() == false)
	{
		ntk::alert("FILE OPEN ERROR...", file.init_check().message());
		return;
	}

	m_file_name = file_name;
	set_title(WINDOW_TITLE(file_name));

	std::vector<uint8> buf;
	buf.reserve(file.size() +1);
	file.read(&buf[0], file.size());
	buf[file.size()] = '\0';

	m_edit->set_text((const char_t*)&buf[0]);
}

void
edit_window_t::save_file_(const ntk_string& file_name, bool over_write)
{
	ntk_entry entry(file_name);
	if(entry.init_check() == false)
		return;

	if(over_write && !entry.is_exists())
		return;
	else if(!over_write && entry.is_exists())
	{
		int ret = ntk::alert("上書き",
			"ファイル(" + entry.path().leaf() + ")はすでに存在しています。\n上書きしてもよろしいですか？",
			"キャンセル", "OK");

		if(ret != 1)
			return;
	}

	ntk_FILE file(file_name, "wb");
	if(file.init_check() == false)
	{
		ntk::alert("FILE SAVE ERROR...", file.init_check().message());
		return;
	}

	const ntk_string& text = m_edit->text();
	file.write(&text[0], text.size());

	m_file_name = file_name;
	set_title(WINDOW_TITLE(file_name));
}
