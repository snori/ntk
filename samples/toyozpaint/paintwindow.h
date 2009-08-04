#ifndef __PAINTWINDOW_H__
#define __PAINTWINDOW_H__


#include <ntk/interface/window.h>
#include "canvas.h"
#include "resource.h"


class paint_window_t : public ntk_window {
public:
	//
	// methods
	//
	paint_window_t(canvas_t* canvas, const ntk_rect& frame)
	:	ntk_window(frame, "Paint Window", TITLED_WINDOW)
	{
		SetMenu(hwnd(), LoadMenu(ntk_application::handle(), MAKEINTRESOURCE(ID_PAINT_WINDOW_MENU)));

		if(canvas == NULL)
			canvas = new canvas_t(NULL);

		m_paint_view = new paint_view_t(canvas, bounds());
		add_child(m_paint_view);

		if(canvas && canvas->entry().init_check())
			set_title("Paint Window  --  [" + canvas->entry().path().as_string() + "]");
		else
			set_title("Paint Window  --  [untitled]");
	}

	LRESULT system_command_received(uint command)
	{
		switch(command)
		{
		case IDM_FILE_EXIT:
			quit();
			break;

		case IDM_WINDOW_NEW_VIEW:
			(new paint_window_t(m_paint_view->canvas(), frame().offset_by_copy(10, 10)))->show();
			break;

		default:
			return ntk_window::system_command_received(command);
		}

		return 0;
	}

private:
	//
	// data
	//
	paint_view_t* m_paint_view;

};// class paint_window_t


#endif//EOH