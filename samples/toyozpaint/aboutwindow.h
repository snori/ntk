#ifndef __ABOUTWINDOW_H__
#define __ABOUTWINDOW_H__


#include <ntk/interface/window.h>
#include <ntk/interface/screen.h>
#include <ntk/interface/stringview.h>
#include "toyozpaint.h"


class about_window_t : public ntk_window {
public:
	//
	// constants
	//
	enum {
		OK_J_MESSAGE = 'OK_J'
	};// messages

	//
	// methods
	//
	about_window_t()
	:	ntk_window(ntk_rect(0, 0, 400, 300), "バージョン情報", TITLED_WINDOW, NOT_RESIZABLE)
	{
		move_to_center_();
		setup_controls_();
	}

	void message_received(const ntk_message& message)
	{
		switch(message.what)
		{
		case OK_J_MESSAGE:
			quit();
			break;

		default:
			ntk_window::message_received(message);
		}
	}

private:
	//
	// functions
	//
	void move_to_center_()
	{
		move_to(
			(ntk_screen::frame().width() - frame().width()) /2,
			(ntk_screen::frame().height() - frame().height()) /2);
	}
	void setup_controls_()
	{
		enum {BUTTON_HEIGHT = 30};

		ntk_rect rect = bounds();

		rect.inset_by(10, 10);
		rect.bottom -= BUTTON_HEIGHT;
		ntk_string_view* ver_info = new ntk_string_view(
			rect, "VerInfo", ver_info_string(), ntk_view::FOLLOW_ALL_SIDES,
			ntk_string_view::DEFAULT_FLAGS | ntk_view::FULL_UPDATE_ON_RESIZE,
			panel()->view_color() + 20);
		ver_info->set_alignment(ntk_string_view::ALIGN_CENTER | ntk_string_view::ALIGN_VCENTER);
		ver_info->set_ex_style(ver_info->ex_style() | WS_EX_STATICEDGE);
		ver_info->resize_by(1, 0);ver_info->resize_by(-1, 0);// style 更新の為（＾＾；
		add_child(ver_info);

		rect = bounds();
		rect.top = rect.bottom - 30;
		rect.inset_by(-1, 0);
		rect.bottom += 1;
		{
			ntk_rect left = rect;
			left.right = left.left + left.width() /2;
			add_child(new ntk_button(left, "CloseButton", "OK", new ntk_message(ntk::QUIT_REQUESTED), ntk_view::FOLLOW_LEFT_RIGHT | ntk_view::FOLLOW_BOTTOM, ntk_button::DEFAULT_FLAGS | ntk_view::FULL_UPDATE_ON_RESIZE));

			ntk_rect right = rect;
			right.left = right.right - right.width() /2;
			add_child(new ntk_button(right, "CloseButton", "おーけー", new ntk_message(OK_J_MESSAGE), ntk_view::FOLLOW_LEFT_RIGHT | ntk_view::FOLLOW_BOTTOM, ntk_button::DEFAULT_FLAGS | ntk_view::FULL_UPDATE_ON_RESIZE));
		}
	}
	ntk_string ver_info_string()
	{
		const ntk_version& ver = app()->version();
		return
			app()->name() + "\n" +
			"\n" +
			"v" + ver.as_string() + "\n" +
			"[" + ver.name() + "]\n" +
			"\n" +
			"COPYRIGHT (C) 1998-2002 Snori, ALL RIGHTS RESERVED.\n"
			"\n"
			"mailto:snori@alles.or.jp\nhttp://ntk.yi.org/";
	}

};// class about_window_t


#endif//EOH