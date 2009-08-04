#ifndef __ABOUTWINDOW_H__
#define __ABOUTWINDOW_H__


#include <ntk/interface/window.h>
#include <ntk/interface/screen.h>
#include <ntk/interface/dialogpanel.h>
#include <ntk/application/application.h>
#include "resource.h"


class about_window_t : public ntk_window {
public:
	about_window_t()
	:	ntk_window(
			new ntk_dialog_panel(
				MAKEINTRESOURCE(ID_ABOUT_DIALOG), ntk_rect(0, 0, 10, 10), "About View",
				ntk_dialog_panel::FOLLOW_LEFT | ntk_dialog_panel::FOLLOW_TOP,
				ntk_dialog_panel::RESIZE_TO_FIT),
			ntk_rect(0, 0, 400, 300),
			"About " + ntk::application()->name()
				+ " v" + ntk::application()->version().as_string()
				+ " built at " + ntk::application()->version().name(),
			TITLED_LOOK, MODAL_APP_FEEL, NOT_RESIZABLE | NOT_ZOOMABLE | NOT_MINIMIZABLE)
	{
		resize_to(panel()->frame().width(), panel()->frame().height());
		move_to(
			(ntk_screen::frame().width() - frame().width()) /2,
			(ntk_screen::frame().height() - frame().height()) /2);
	}

	LRESULT system_command_received(uint command)
	{
		switch(command)
		{
		case IDOK:
			quit();
			break;

		default:
			return ntk_window::system_command_received(command);
		}

		return 0;
	}

};// class about_window_t


#endif//EOH