/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: alert.cpp,v 1.7 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/alert.h"


#include <ntk/windows/windows.h>
#include <ntk/application/application.h>
#include <ntk/application/invoker.h>
#include <ntk/interface/screen.h>
#include <ntk/interface/button.h>
#include <ntk/interface/stringview.h>
#include <ntk/interface/panel.h>


namespace ntk {


//########################################################


namespace {


enum{
	ALERT_BUTTON0 = '@AB0',
	ALERT_BUTTON1 = '@AB1',
	ALERT_BUTTON2 = '@AB2',
};// alert window messages


class AlertPanel : public Panel {
public:
	enum{
		CURTAIN_WIDTH = 75
	};

	AlertPanel()
	:	Panel(Rect(), "Panel", FOLLOW_ALL_SIDES, WILL_DRAW | COVER_WINDOW)
	{}

	void draw(PaintDC& dc)
	{
		Rect rect = bounds();
		rect.right = CURTAIN_WIDTH;

		dc.set_brush(Brush(view_color() - 25));
		dc.fill_rect(rect);
	}
};


}// anonymous namespace


//########################################################
// public methods

Alert::Alert(const String& title, const String& text, const String& button0_label, const String& button1_label, const String& button2_label, ButtonWidth button_width, AlertType alert_type)
:	Window(new AlertPanel(), Rect(0, 0, 450, 200), title, TITLED_LOOK, MODAL_APP_FEEL, NOT_RESIZABLE | NOT_ZOOMABLE | NOT_MINIMIZABLE | ALWAYS_ON_TOP)
,	m_text(NULL)
,	m_button0(NULL)
,	m_button1(NULL)
,	m_button2(NULL)
,	m_invoker(NULL)
,	m_result_which(NULL)
{
	enum {
		INSET = 5,
		BUTTON_HEIGHT = 20,
		BUTTON_DEFAULT_WIDTH = 65,
		BUTTON_LABEL_SIDE_SPACE = 15,
		SPACE_BETWEEN_BUTTONS = 10,
	};

	Rect rect = bounds();
	coord text_width[3];

	rect.left = AlertPanel::CURTAIN_WIDTH;
	rect.bottom -= BUTTON_HEIGHT + INSET;
	rect.inset_by(INSET, INSET);

	m_text = new StringView(rect, "Text", text, View::FOLLOW_ALL_SIDES);
	Rect text_rect = m_text->dc().calculate_rect(text);
	m_text->resize_to(text_rect.width(), text_rect.height());

	// ウィンドウを、すべてを含むことが出来る大きさにする
	resize_to(
		INSET * 2 + text_rect.width() + AlertPanel::CURTAIN_WIDTH,
		INSET * 3 + text_rect.height() + BUTTON_HEIGHT);

	// ここで add_child するのは、FOLLOW_ALL_SIDES によってウィンドウのリサイズに影響を受けないようにするため
	add_child(m_text);

	rect = bounds();
	rect.inset_by(INSET, INSET);
	rect.top = rect.bottom - BUTTON_HEIGHT;

	m_button0 = new Button(rect, "Button0", button0_label, new Message(ALERT_BUTTON0), View::FOLLOW_BOTTOM | View::FOLLOW_RIGHT);
	add_child(m_button0);
	text_width[0] = DC(m_button0->hwnd()).string_width(m_button0->label());

	if(button1_label != "")
	{
		m_button1 = new Button(rect, "Button1", button1_label, new Message(ALERT_BUTTON1), View::FOLLOW_BOTTOM | View::FOLLOW_RIGHT);
		add_child(m_button1);
		text_width[1] = DC(m_button1->hwnd()).string_width(m_button1->label());
	}

	if(button2_label != "")
	{
		m_button2 = new Button(rect, "Button2", button2_label, new Message(ALERT_BUTTON2), View::FOLLOW_BOTTOM | View::FOLLOW_RIGHT);
		add_child(m_button2);
		text_width[2] = DC(m_button2->hwnd()).string_width(m_button2->label());
	}

	switch(button_width)
	{
	case WIDTH_AS_USUAL:
		{
			for(int i=0; i<3; ++i)
				text_width[i] = BUTTON_DEFAULT_WIDTH;
		}
		break;

	case WIDTH_FROM_LABEL:
		{}
		break;

	case WIDTH_FROM_WIDEST:
		{
			coord widest = 0;

			{for(int i=0; i<3; ++i)
				if(text_width[i] > widest)
					widest = text_width[i];
			}

			{for(int i=0; i<3; ++i)
				text_width[i] = widest;
			}
		}
		break;
	}

	coord left = frame().width() - INSET;

	m_button0->resize_to(text_width[0] + BUTTON_LABEL_SIDE_SPACE *2, m_button0->frame().height());
	rect = m_button0->frame();
	m_button0->move_to(left - rect.width(), rect.top);

	left -= m_button0->frame().width();
	left -= SPACE_BETWEEN_BUTTONS;

	if(m_button1)
	{
		m_button1->resize_to(text_width[1] + BUTTON_LABEL_SIDE_SPACE *2, m_button1->frame().height());
		rect = m_button1->frame();
		m_button1->move_to(left - rect.width(), rect.top);

		left -= m_button1->frame().width();
		left -= SPACE_BETWEEN_BUTTONS;
	}

	if(m_button2)
	{
		m_button2->resize_to(text_width[2] + BUTTON_LABEL_SIDE_SPACE *2, m_button2->frame().height());
		rect = m_button2->frame();
		m_button2->move_to(left - rect.width(), rect.top);

		left -= m_button2->frame().width();
		left -= SPACE_BETWEEN_BUTTONS;
	}

	if(left < AlertPanel::CURTAIN_WIDTH)
		resize_to(frame().width() + AlertPanel::CURTAIN_WIDTH - left, frame().height());
}

Alert::~Alert()
{
	delete m_invoker;
}

int
Alert::go()
{
	int result = 0;

	m_invoker = NULL;
	m_result_which = &result;

	move_to(
		(Screen::frame().width() - frame().width()) /2,
		(Screen::frame().height() - frame().height()) /2);

	show();
	application()->message_loop();// これでいいのだろうか・・・。確信はないかも。

	// ここに来た時点で this は delete 済み。
	return result;
}

status_t
Alert::go(Invoker* invoker)
{
	if(invoker == NULL)
	{
		go();
		return st::OK;
	}

	m_invoker = invoker;
	m_result_which = NULL;

	move_to(
		(Screen::frame().width() - frame().width()) /2,
		(Screen::frame().height() - frame().height()) /2);

	show();

	return st::OK;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

Button*
Alert::button0()
{
	return m_button0;
}

const Button*
Alert::button0() const
{
	return m_button0;
}

Button*
Alert::button1()
{
	return m_button1;
}

const Button*
Alert::button1() const
{
	return m_button1;
}

Button*
Alert::button2()
{
	return m_button2;
}

const Button*
Alert::button2() const
{
	return m_button2;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public messge handlers

bool
Alert::quit_requested()
{
	if(m_invoker == NULL)
		PostQuitMessage(0);// synchronous

	return Window::quit_requested();
}

void
Alert::message_received(const Message& message)
{
	switch(message.what)
	{
	case ALERT_BUTTON0:
		button_pushed_(0);
		quit();
		break;

	case ALERT_BUTTON1:
		button_pushed_(1);
		quit();
		break;

	case ALERT_BUTTON2:
		button_pushed_(2);
		quit();
		break;

	default:
		Window::message_received(message);
	}
}

//********************************************************
// functions

void
Alert::button_pushed_(int which)
{
	if(m_result_which)
		*m_result_which = which;
	else
	{
		Message msg = *m_invoker->message();
		msg.add_int("which", which);

		m_invoker->invoke(msg);// asynchronous
	}
}


//########################################################


int
alert(const String& title, const String& text, const String& button0_label, const String& button1_label, const String& button2_label, Alert::ButtonWidth button_width, Alert::AlertType alert_type)
{
	Alert* alert = new Alert(title, text, button0_label, button1_label, button2_label, button_width, alert_type);
	return alert->go();
}


//########################################################
}// namespace ntk
