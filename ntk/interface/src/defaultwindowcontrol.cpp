/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: defaultwindowcontrol.cpp,v 1.13 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "defaultwindowcontrol.h"


#include <assert.h>
#include <ntk/interface/window.h>


namespace ntk {


static void
set_topmost(HWND hwnd, bool flag)
{
	if(flag)
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


//########################################################
// public methods

Rect
DefaultWindowControl::client_frame() const
{
	Rect rect = window()->window_frame();

	coord BORDER_WIDTH   = GetSystemMetrics(SM_CXFRAME);
	coord BORDER_HEIGHT  = GetSystemMetrics(SM_CYFRAME);
	coord CAPTION_HEIGHT = GetSystemMetrics(SM_CYCAPTION);

	rect.inset_by(BORDER_WIDTH, BORDER_HEIGHT);
	rect.top += CAPTION_HEIGHT;

	coord menu_height = window()->menu_height();
	if(menu_height)
		rect.top += menu_height;

	return rect;
}

Rect
DefaultWindowControl::client_bounds() const
{
	return client_frame().offset_to_self(0, 0);
}

void
DefaultWindowControl::set_look(uint look_)
{
	if(look_ == look())
		return;

	WindowControl::set_look(look_);

	// フラグを初期化
	uint style        = window()->style();
	uint ex_style     = window()->ex_style();
	uint style_old    = style;
	uint ex_style_old = ex_style;

	style &= ~(WS_OVERLAPPEDWINDOW & ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX));
	style &= ~(WS_POPUP | /*WS_SYSMENU |*/ WS_POPUPWINDOW);
	style &= ~WS_BORDER;
//	ex_style &= ~(WS_EX_CLIENTEDGE);

	switch(look())
	{
	case Window::TITLED_LOOK:
	case Window::DOCUMENT_LOOK:
	case Window::MODAL_LOOK:
	case Window::FLOATING_LOOK:
		style |= WS_OVERLAPPEDWINDOW & ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX);// min/max box は set_flags() で設定される。
		break;

//	case Window::POPUP_LOOK:
//		style |= WS_POPUPWINDOW;
//		break;

	case Window::BORDERED_LOOK:
		style |= WS_BORDER;
		break;

	case Window::NOBORDER_LOOK:
		style |= WS_POPUP;// | WS_SYSMENU;
		break;

	default:
		error("不正な WINDOW LOOK が指定されました。");
		return;
	}

	if(style    != style_old)    window()->set_style(style, true);
	if(ex_style != ex_style_old) window()->set_ex_style(ex_style, true);

//	if(style != style_old || ex_style != ex_style_old)
//		window()->update_frame();
}

void
DefaultWindowControl::set_feel(uint feel_)
{
	if(feel_ == feel())
		return;

	WindowControl::set_feel(feel_);

	uint style        = window()->style();
	uint ex_style     = window()->ex_style();
	uint style_old    = style;
	uint ex_style_old = ex_style;

	switch(feel())
	{
	case Window::NORMAL_FEEL:
		set_flags(flags() & ~Window::ALWAYS_ON_TOP);
		break;

	case Window::MODAL_SUBSET_FEEL:
	case Window::MODAL_APP_FEEL:
	case Window::MODAL_ALL_FEEL:
		set_flags(flags() | Window::ALWAYS_ON_TOP);
		break;

	case Window::FLOATING_SUBSET_FEEL:
	case Window::FLOATING_APP_FEEL:
	case Window::FLOATING_ALL_FEEL:
		set_flags(flags() | Window::ALWAYS_ON_TOP);
		break;

	default:
		error("不明な WINDOW FEEL が指定されました");
	}

	if(style    != style_old)    window()->set_style(style, true);
	if(ex_style != ex_style_old) window()->set_ex_style(ex_style, true);

//	if(style != style_old || ex_style != ex_style_old)
//		window()->update_frame();
}

void
DefaultWindowControl::set_flags(uint flags_)
{
	if(flags_ == flags())
		return;

	WindowControl::set_flags(flags_);

	uint style        = window()->style();
	uint ex_style     = window()->ex_style();
	uint style_old    = style;
	uint ex_style_old = ex_style;

	// 閉じるボタン
//	if(m_flags & Window::NOT_CLOSABLE)
//		style &= ~WS_SYSMENU;
//	else
//		style |= WS_SYSMENU;

	// 最大化ボタン
	if(flags() & Window::NOT_ZOOMABLE)
		style &= ~WS_MAXIMIZEBOX;
	else
		style |= WS_MAXIMIZEBOX;

	// 最小化ボタン
	if(flags() & Window::NOT_MINIMIZABLE)
		style &= ~WS_MINIMIZEBOX;
	else
		style |= WS_MINIMIZEBOX;

	if(flags() & Window::ALWAYS_ON_TOP)
		set_topmost(window()->hwnd(), true);
	else
		set_topmost(window()->hwnd(), false);

	if(style    != style_old)    window()->set_style(style, true);
	if(ex_style != ex_style_old) window()->set_ex_style(ex_style, true);

//	if(style != style_old || ex_style != ex_style_old)
//		update_frame();
}

uint
DefaultWindowControl::area_test(const Point& point, uint default_value) const
{
	const Rect& frame = window()->window_frame();
	coord x = point.x - frame.left;
	coord y = point.y - frame.top;
	coord width = frame.width();
	coord height = frame.height();
	Rect frame_width(
		GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME),
		GetSystemMetrics(SM_CXFRAME), GetSystemMetrics(SM_CYFRAME));

	if(
		(frame_width.left <= x && x < width - frame_width.right) &&
		(frame_width.top <= y && y < height - frame_width.bottom))
	{
		return default_value;//Wnd::system_message_received(message);
	}
	else
	{
		uint flag = this->flags();

		if(flag & Window::NOT_V_RESIZABLE && flag & Window::NOT_H_RESIZABLE)
		{
			return HTCLIENT;
		}
		else if(flag & Window::NOT_H_RESIZABLE)
		{
			if(y < frame_width.top || height - frame_width.bottom <= y)
				return default_value;
			else
				return HTCLIENT;
		}
		else if(flag & Window::NOT_V_RESIZABLE)
		{
			if(x < frame_width.left || width - frame_width.right <= x)
				return default_value;
			else
				return HTCLIENT;
		}
		else return default_value;//Wnd::system_message_received(message);
	}
}


//########################################################
}// namespace ntk
