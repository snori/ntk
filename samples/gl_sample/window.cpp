#include "window.h"

#include <ntk/interface/panel.h>
#include <ntk/interface/filepanel.h>
#include "application.h"
#include "renderview.h"
#include "geom.h"
#include "resource.h"


//########################################################


gl_window_t::gl_window_t()
:	ntk_window(
		app()->conf().get_rect("gl_window::frame", ntk_rect(100, 100, 600, 600)),
		ntk::application()->name() + " v" + ntk::application()->version().as_string(),
		TITLED_WINDOW)
{
	SetMenu(hwnd(),
		LoadMenu(ntk::application()->handle(), MAKEINTRESOURCE(ID_APP_MENU)));

	add_child(new render_view_t);
}

gl_window_t::~gl_window_t()
{
	app()->conf().set_rect("gl_window::frame", frame());
}

bool
gl_window_t::quit_requested()
{
	ntk::application()->quit();
	return true;
}

void
gl_window_t::message_received(const ntk_message& message)
{
	switch(message.what)
	{
	case ntk::REFS_RECEIVED:
		app()->scene().load_MQO(message.find_string("name"));
		panel()->invalidate();
		break;

	default:
		ntk_window::message_received(message);
	}
}

LRESULT
gl_window_t::system_command_received(uint command)
{
	switch(command)
	{
	case IDM_FILE_IMPORT_MQO:
		{
			ntk_file_panel file_panel;
			file_panel.set_target(ntk_messenger(NULL, this));
			file_panel.show();
		}
		break;

	case IDM_HELP_ABOUT:
		ntk::application()->post_message(ntk::ABOUT_REQUESTED);
		break;

	case IDM_FILE_EXIT:
		quit();
		break;

	default:
		return ntk_window::system_command_received(command);
	}

	return 0;
}
