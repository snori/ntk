#include "toyozpaint.h"

#include <ntk/ntk.h>
#include "paintwindow.h"
#include "aboutwindow.h"
#include "resource.h"


class app_window_t : public ntk_window {
public:
	app_window_t()
	:	ntk_window(ntk_rect(100, 100, 600, 600), ntk::application()->name(), TITLED_WINDOW)
	{
		SetMenu(hwnd(), LoadMenu(ntk_application::handle(), MAKEINTRESOURCE(ID_APP_WINDOW_MENU)));
	}

	LRESULT system_command_received(uint command_id)
	{
		switch(command_id)
		{
		case IDM_FILE_NEW:
			(new paint_window_t(NULL, ntk_rect(100, 100, 500, 500)))->show();
			break;

		case IDM_FILE_OPEN:
			{
				ntk_file_panel().show();
			}
			break;

		case IDM_FILE_SAVE:
			{
				ntk_file_panel(ntk_file_panel::SAVE_PANEL).show();
			}
			break;

		case IDM_FILE_SAVE_AS:
			{
			}
			break;

		case IDM_FILE_EXIT:
			quit();
			break;

		case IDM_HELP_ABOUT:
			app()->post_message(ntk::ABOUT_REQUESTED);
			break;

		default:
			return ntk_window::system_command_received(command_id);
		}

		return 0;
	}

	bool quit_requested()
	{
		app()->quit();
		return true;
	}
};


//########################################################


application_t::application_t()
:	ntk_application("t o y o z p a i n t", ntk_version(0, 0, 1, 0, "ALPHA"))
{
	(new app_window_t())->show();
}

void
application_t::about_requested()
{
	(new about_window_t)->show();
}

void
application_t::message_received(const ntk_message& message)
{
	switch(message.what)
	{
	case ntk::REFS_RECEIVED:
		{
			paint_window_t* win = new paint_window_t(
				new canvas_t(message.find_string("name")), ntk_rect(100, 100, 500, 500));
			win->show();
		}
		break;

	case ntk::SAVE_REQUESTED:
		break;

	default:
		ntk_application::message_received(message);
	}
}


//########################################################


#include <ntk/main.h>

int
main(int argc, const char_t** argv)
{
	application_t app;
	app.run();

	return 0;
}
