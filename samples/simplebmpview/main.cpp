#include <ntk/ntk.h>
#include "resource.h"


//########################################################


namespace {


static const int GENERATION = 1;
static const int VERSION    = 0;
static const int RELEASE    = 0;


NSetting g_conf(NApplication::directory().path().as_string() + "\\simplebmpview.conf");// 設定ファイル名

enum {
	OPEN_BMP = 'OPEN'
};// messages


}// anonymous namespace


//########################################################


class BitmapPreview : public NView {
public:
	BitmapPreview()
	:	NView(NRect(0, 0, 0, 0), "Bitmap Preview", FOLLOW_ALL_SIDES, WILL_DRAW, ntk::transparent_color())
	{}

	void attached_to_window()
	{
		resize_to(parent()->frame().width(), parent()->frame().height());
	}
	void draw(NPaintDC& pdc);
};


class Win : public NWindow {
public:
	Win()
	:	NWindow(
			g_conf.get_rect("Win::frame", NRect(100, 100, 500, 500)),
			ntk::application()->name() + " v" + ntk::application()->version().as_string(),
			TITLED_WINDOW)
	,	m_image(NULL)
	{
		set_menu_handle(LoadMenu(ntk::application()->handle(), MAKEINTRESOURCE(ID_APP_MENU)));
		add_child(new BitmapPreview);
	}
	~Win()
	{
		g_conf.set_rect("Win::frame", frame());
		delete m_image;
	}

	NBitmap* image() const
	{
		return m_image;
	}

	bool quit_requested()
	{
		ntk::application()->quit();
		return true;
	}

	void message_received(const NMessage& message)
	{
		switch(message.what)
		{
		case OPEN_BMP:
			{
				delete m_image;
				m_image = NTranslationUtils::get_bitmap(message.find_string("name"));

				if(m_image == NULL)
					ntk::error("ビットマップ(" + message.find_string("name") + ") の読み込みに失敗しました。");
			}
			break;

		default:
			NWindow::message_received(message);
		}
	}

	LRESULT system_command_received(uint command)
	{
		switch(command)
		{
		case IDM_FILE_OPEN:
			{
				NFilePanel p(NFilePanel::OPEN_PANEL, NMessenger(NULL, this), new NMessage(OPEN_BMP));
				p.show();
			}
			break;

		case IDM_EXIT:
			quit();
			break;

		case IDM_ABOUT:
			ntk::message_box(
				"バージョン情報",
				ntk::application()->name() + " v" + ntk::application()->version().as_string() + "\n"
				"(C) snori@alles.or.jp\n\n");
			break;

		default:
			return NWindow::system_command_received(command);
		}

		return 0;
	}

private:
	NBitmap* m_image;
};


class App : public NApplication {
public:
	App()
	:	NApplication("SimpleBMPView", NVersion(GENERATION, VERSION, RELEASE))
	{
		(new Win)->show();
	}

};


void
BitmapPreview::draw(NPaintDC& pdc)
{
	NMemoryDC dc(pdc);
	dc.set_bitmap(NDDB(pdc, frame().width(), frame().height()));

	dc.set_brush(ntk::white_color());
	dc.fill_rect(bounds());

	if(((Win*)window())->image())
		dc.draw_bitmap(*((Win*)window())->image());

	pdc.bit_blt(0, 0, frame().width(), frame().height(), dc, 0, 0);
}


//########################################################


#include <ntk/main.h>

int
main(int argc, const char** argv)
{
	App app;
	app.run();

	return 0;
}
