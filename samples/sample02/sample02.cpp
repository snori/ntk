#include <ntk/ntk.h>
#include <ntk/main.h>
#include "resource.h"

using namespace ntk;

class SimpleView : public ntk::View {
public:
	SimpleView()
	:	ntk::View(Rect(0, 0, 0, 0), "sample-view", FOLLOW_ALL_SIDES, WILL_DRAW)
	{
		set_view_color(RGBColor(100, 100, 100));
	}

	void attached_to_window()
	{
		Rect rect = window()->bounds();
		rect.inset_by(5, 5);

		move_to(rect.left, rect.top);
		resize_to(rect.width(), rect.height());
	}

	void draw(PaintDC& dc)
	{
		dc.set_text_color(RGBColor(255, 255, 255));

		dc.draw_string("サンプルプログラムです。", Point(10, 10));
		dc.draw_string("ウィンドウ内の描画はすべて View が行います。", Point(10, 30));
		dc.draw_string("Window は View のコンテナの役割を果たします。", Point(10, 50));
		dc.draw_string("BeOS のクラスライブラリに似せてあります。", Point(10, 70));
	}

};

class PaintView : public View {
public:
	PaintView(coord width, coord height, Bitmap::ColorSpace space)
	:	View(Rect(), "PaintView", FOLLOW_RIGHT | FOLLOW_BOTTOM, WILL_DRAW, transparent_color()),
		m_bitmap(width, height, space),
		m_drawing(false)
	{}

	void attached_to_window()
	{
		resize_to(m_bitmap.width(), m_bitmap.height());
		move_to(
			(window()->bounds().width() - bounds().width()) /2,
			(window()->bounds().height() - bounds().height()) /2);
	}

	void draw(PaintDC& dc)
	{
		dc.draw_bitmap(m_bitmap, Point(0, 0));
	}

	void mouse_down(const Point& point, uint buttons, bool double_clicked)
	{
		View::mouse_down(point, buttons, double_clicked);
		m_drawing = true;
	}
	void mouse_up(const Point& point, uint buttons)
	{
		View::mouse_up(point, buttons);
		m_drawing = false;
	}
	void mouse_moved(const Point& point, uint buttons, uint transit, const Message* data)
	{
		View::mouse_moved(point, buttons, transit, data);

		if(m_drawing)
		{
			uint8* pixel = (uint8*)m_bitmap.at(point.x, point.y -1);
			switch(buttons)
			{
			case LEFT_MOUSE_BUTTON:
				pixel[0] = 100;
				pixel[1] = 100;
				pixel[2] = 100;

				pixel += m_bitmap.pitch() *2;

				pixel[0] = 100;
				pixel[1] = 100;
				pixel[2] = 100;

				pixel -= m_bitmap.pitch();
				pixel -= m_bitmap.bpp() >>3;

				pixel[0] = 100;
				pixel[1] = 100;
				pixel[2] = 100;

				pixel += m_bitmap.bpp() >>3;

				pixel[0] = 255;
				pixel[1] = 255;
				pixel[2] = 255;

				pixel += m_bitmap.bpp() >>3;

				pixel[0] = 100;
				pixel[1] = 100;
				pixel[2] = 100;
				break;

			case RIGHT_MOUSE_BUTTON:
				{
					static const PEN_RADIUS = 8;

					uint8* pixel;
					for(int y=point.y - PEN_RADIUS; y<point.y + PEN_RADIUS; ++y)
					for(int x=point.x - PEN_RADIUS; x<point.x + PEN_RADIUS; ++x)
					{
						if((point.x-x)*(point.x-x)+(point.y-y)*(point.y-y) <= PEN_RADIUS*PEN_RADIUS)
						{
							pixel = (uint8*)m_bitmap.at(x, y);
							pixel[0] = 255;
							pixel[1] = 100;
							pixel[2] = 255;
						}
					}
				}
				break;
			}

			invalidate();
		}
	}

private:
	Bitmap m_bitmap;
	bool m_drawing;

};

class Win : public Window {
public:
	Win()
	:	Window(
			Rect(100, 100, 600, 600),
			application()->name() + " Ver" + application()->version().as_string() + " + NTK v" + NTK_Version.as_string(),
			TITLED_WINDOW, 0, 0, 0, LoadMenu(Application::handle(), MAKEINTRESOURCE(ID_APP_MENU)))
	{
		set_icon_handle(LoadIcon(Application::handle(), MAKEINTRESOURCE(ID_APP_ICON)));
		add_child(new SimpleView());
	}

	bool quit_requested()
	{
		// ウィンドウが閉じられるとアプリケーションの終了とみなす
		application()->quit();

		// false を返すとウィンドウは閉じない
		return true;
	}
	LRESULT system_command_received(uint command_id)
	{
		switch(command_id)
		{
		case IDM_FILE_EXIT:
			quit();
			break;

		default:
			return Window::system_command_received(command_id);
		}

		return 0;
	}
};

class Sample02 : public Application {
public:
	Sample02()
	:	Application("NTK Sample02", Version(1, 0, 0, 0, "20020826"))
	{
		Win* win = new Win();
		win->show();

		Window* paint = new Window(Rect(200, 50, 600, 400), "Paint Window");
		paint->add_child(new PaintView(256, 256, Bitmap::RGB_24));
		paint->show();
	}
};

int
main(int argc, const char_t** argv)
{
	Sample02 app;
	app.run();

	return 0;
}
