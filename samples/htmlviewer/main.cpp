#include <ntk/ntk.h>
#include "resource.h"


//########################################################


namespace {


NSetting g_conf(NApplication::directory().path().as_string() + "\\htmlviewer.conf");


enum {
	GO_URL = 'GO!!'
};// messages


}// anonymous namespace


//########################################################


class AddressBar : public NScintilla {
public:
	AddressBar(
		const NRect& frame,
		const NString& name,
		const NString& text,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS)
	:	NScintilla(frame, name, text, mode, flags)
	{}

	LRESULT system_message_received(const NMessage& message)
	{
		switch(message.what)
		{
		case WM_CHAR:
			{
				uint key = message.wparam();

				switch(key)
				{
				case 27:// ESC
				case '\r':
				case '\n':
					return 0;
				}

				return NScintilla::system_message_received(message);
			}
			break;

		case WM_KEYDOWN:
			{
				uint key = message.wparam();

				if(key == VK_RETURN)
				{
					NMessage msg(GO_URL);
					msg.add_string("url", text());
					window()->post_message(msg);

					return 0;
				}
				else if(key == VK_ESCAPE)
				{
					set_text("");
					return 0;
				}

				return NScintilla::system_message_received(message);
			}
			break;

		default:
			return NScintilla::system_message_received(message);
		}
		
		return 0;
	}
};


//########################################################


class ViewerWindow : public NWindow {
public:
	enum {
		BACK,
		FORWARD,
		STOP,
		REFRESH,
		HOME,
	};

	ViewerWindow(const NRect& frame)
	:	NWindow(g_conf.get_rect("ViewerWindow::frame", frame), ntk::application()->name(), TITLED_WINDOW)
	{
		enum {SPACE = 2};// Œ„ŠÔ

		set_icon_handle(LoadIcon(NApplication::handle(), MAKEINTRESOURCE(ID_APP_ICON)));

		NRect rect = bounds();

		NStdMenuBar* menu_bar = new NStdMenuBar(
			LoadMenu(NApplication::handle(), MAKEINTRESOURCE(ID_APP_MENU)),
			rect, "MenuBar");
		add_child(menu_bar);

		rect.top += menu_bar->frame().height();
		rect.top += SPACE;

		{
			NRect r = rect;
			r.bottom = r.top + 20;
			rect.top += r.height();

			NRGBColor color = panel()->view_color();
			color.blue += 25;

			r.offset_by(SPACE, 0);

			r.right = r.left + 32;
			NButton* back_button = new NButton(
				r, "Back", "‘O", new NMessage(BACK));
			back_button->set_view_color(color);
			add_child(back_button);

			r.offset_by(back_button->frame().width(), 0);
			NButton* forward_button = new NButton(
				r, "Forward", "ŽŸ", new NMessage(FORWARD));
			forward_button->set_view_color(color);
			add_child(forward_button);

			r.offset_by(SPACE, 0);

			r.offset_by(forward_button->frame().width(), 0);
			NButton* stop_button = new NButton(
				r, "Stop", "Ž~", new NMessage(STOP));
			stop_button->set_view_color(color);
			add_child(stop_button);

			r.offset_by(stop_button->frame().width(), 0);
			NButton* refresh_button = new NButton(
				r, "Refresh", "Ä", new NMessage(REFRESH));
			refresh_button->set_view_color(color);
			add_child(refresh_button);

			r.offset_by(SPACE, 0);

			r.offset_by(refresh_button->frame().width(), 0);
			NButton* home_button = new NButton(
				r, "Home", "‰Æ", new NMessage(HOME));
			home_button->set_view_color(color);
			add_child(home_button);

			r.offset_by(SPACE, 0);

			r.offset_by(home_button->frame().width(), 0);
			r.right = rect.right;
			r.right -= SPACE;
			m_address_bar = new AddressBar(
				r, "Address", "http://home", NView::FOLLOW_LEFT_RIGHT | NView::FOLLOW_TOP);
			m_address_bar->set_ex_style(m_address_bar->ex_style() | WS_EX_CLIENTEDGE, true);
			add_child(m_address_bar);
		}

		rect.top += SPACE;

		{
			NRect r = rect;

			r.bottom = r.top + 1;
			NView* border = new NView(
				r, "Border", NView::FOLLOW_LEFT_RIGHT | NView::FOLLOW_TOP,
				DEFAULT_FLAGS, ntk::outline_color());
			add_child(border);

			rect.top += border->frame().height();
		}

		m_html_view = new NHTMLView(rect, "HTMLView", NView::FOLLOW_ALL_SIDES);
		add_child(m_html_view);
	}
	~ViewerWindow()
	{
		g_conf.set_rect("ViewerWindow::frame", frame());
	}

	void ready_to_run()
	{
		m_html_view->go("http://home");
	}
	bool quit_requested()
	{
		ntk::application()->quit();
		return 0;
	}
	void message_received(const NMessage& message)
	{
		switch(message.what)
		{
		case BACK:
			m_html_view->go_back();
			break;

		case FORWARD:
			m_html_view->go_forward();
			break;

		case STOP:
			m_html_view->stop();
			break;

		case REFRESH:
			m_html_view->refresh();
			break;

		case HOME:
			m_html_view->go("http://home");
			break;

		case GO_URL:
			m_html_view->go(message.find_string("url"));
			break;

		case NHTMLView::ADDRESS_CHANGED:
			m_address_bar->set_text(message.find_string("url"));
			break;

		default:
			NWindow::message_received(message);
		}
	}
	LRESULT system_command_received(uint command)
	{
		switch(command)
		{
		case IDM_FILE_EXIT:
			quit();
			break;

		case IDM_EDIT_CAPTURE_PAGE:
			{
				NBitmap bmp(780, 1000, NBitmap::RGB_24);

				NMessage msg(NHTMLView::CAPTURE_PAGE);
				msg.add_pointer("hdc", bmp.hdc());
				msg.add_rect("bounds", bmp.bounds());

				NMessenger(m_html_view).send_message(msg);

//				bmp.dc().bit_blt(0, 0, bmp.width(), bmp.height(), dc, 0, 0, NDC::OP_COPY);

//				NTranslationUtils::store_bitmap("c:\\ie.bmp", bmp);
			}
			break;

		case IDM_HELP_ABOUT:
			ntk::application()->post_message(ntk::ABOUT_REQUESTED);
			break;

		default:
			NWindow::system_command_received(command);
		}

		return 0;
	}

private:
	AddressBar* m_address_bar;
	NHTMLView* m_html_view;
};


//########################################################


class AboutWindow : public NWindow {
public:
	AboutWindow()
	:	NWindow(
			NRect(0, 0, 500, 300), "About HTMLViewer.", MODAL_WINDOW,
			NOT_RESIZABLE | NOT_MINIMIZABLE | NOT_ZOOMABLE)
	{
		NRect rect = bounds();

		{
			NRect r = rect;

			r.top = r.bottom - 20;
			NButton* close_button = new NButton(
				r, "CloseButton", "OK", new NMessage(ntk::QUIT_REQUESTED),
				NView::FOLLOW_LEFT_RIGHT | NView::FOLLOW_BOTTOM);
			add_child(close_button);

			rect.bottom -= close_button->frame().height();
		}

		NHTMLView* html = new NHTMLView(
			rect, "AboutHTML", NView::FOLLOW_ALL_SIDES);
		add_child(html);

		NPath path(NApplication::directory().path().as_string() + "/../samples/htmlviewer/about.html");
		path.normalize();

		html->go(path.as_string());

		// move to center
		move_to(
			(NScreen::frame().width() - frame().width()) /2,
			(NScreen::frame().height() - frame().height()) /2);
	}
};


//########################################################


class ViewerApp : public NApplication {
public:
	ViewerApp()
	:	NApplication("HTML Viewer", NVersion(1))
	{
		(new ViewerWindow(NRect(100, 100, 500, 500)))->show();
	}

	void about_requested()
	{
		(new AboutWindow)->show();
	}
};


//########################################################


#include <ntk/main.h>


int
main(int argc, const char_t** argv)
{
	ViewerApp app;
	app.run();

	return 0;
}
