#include <windows.h>
#include <mmsystem.h>
#include <ntk/application/application.h>
#include <ntk/interface/window.h>
#include <ntk/interface/panel.h>
#include <ntk/interface/bitmap.h>
#include <ntk/storage/directory.h>
#include <ntk/support/setting.h>
#include "demo.h"


#define for if(false); else for


namespace {


ntk_setting g_conf;

enum {
	DEFAULT_WINDOW_LEFT_POS = 100,
	DEFAULT_WINDOW_TOP_POS  = 100,
	SCREEN_WIDTH    = 320,
	SCREEN_HEIGHT   = 200,
};
const ntk_rect DEFAULT_WINDOW_FRAME(
	DEFAULT_WINDOW_LEFT_POS,
	DEFAULT_WINDOW_TOP_POS,
	DEFAULT_WINDOW_LEFT_POS + SCREEN_WIDTH,
	DEFAULT_WINDOW_TOP_POS + SCREEN_HEIGHT);


}// anonymouse namespace


uint draw_screen(void*);
void draw(ntk_dib* screen);


class demo_screen_t : public ntk_panel {
public:
	demo_screen_t()
	:	ntk_panel(ntk_rect(), "ScreenPanel")
	,	m_show_FPS(true)
	{}

	bool show_FPS() const {return m_show_FPS;}

	void key_down(uint key, uint)
	{
		if(key == VK_ESCAPE)
			window()->quit();
		else if(key == 'F')
			m_show_FPS = !m_show_FPS;
	}

private:
	bool m_show_FPS;
};


class demo_window_t : public ntk_window {
public:
	demo_window_t()
	:	ntk_window(
			new demo_screen_t,
			g_conf.get_rect("frame", DEFAULT_WINDOW_FRAME),
			ntk::application()->name() + "  v" + ntk::application()->version().as_string(),
			TITLED_WINDOW, NOT_RESIZABLE | NOT_ZOOMABLE | NOT_MINIMIZABLE)
	,	m_quit_flag(false)
	{}
	~demo_window_t()
	{
		g_conf.set_rect("frame", frame());
	}

	void show()
	{
		ntk_window::show();

		m_thread = ntk::spawn_thread(draw_screen, "drawing_thread");
		ntk::resume_thread(m_thread);
	}

	bool quit_flag() const {return m_quit_flag;}

	bool quit_requested()
	{
		m_quit_flag = true;
		ntk::wait_for_thread(m_thread);

		ntk::application()->quit();
		return true;
	}

private:
	HANDLE m_thread;
	bool m_quit_flag;
};


demo_t::demo_t(const ntk_string& name, const ntk_version& ver)
:	ntk_application(name, ver)
{
	g_conf = ntk_setting(ntk_application::directory().path().as_string() + "\\" + name + ".conf");

	m_window = new demo_window_t;

	m_screen = (demo_screen_t*)m_window->panel();
}

HANDLE
demo_t::run()
{
	m_window->show();
	return ntk_application::run();
}

inline bool
demo_t::quit_flag() const
{
	return m_window->quit_flag();
}


///////////////////////////////////////////////////


uint
draw_screen(void*)
{
	demo_t* app = demo_t::instance();
	ntk_dc dc(app->screen()->hwnd());

	ntk_dib screen(SCREEN_WIDTH, SCREEN_HEIGHT, ntk_dib::RGB_24);
	ntk_dc screen_dc = screen.dc();

	const ntk_rgb_color TEXT_COLOR = ntk::white_color();
	ulong time, prev_time = timeGetTime();
	int frame_count = 0;
	ntk_string fps;

	while(app->quit_flag() == false)
	{
		draw(&screen);
		frame_count++;

		if(app->screen()->show_FPS())
		{
			time = timeGetTime();

			if(time - prev_time > 200)
			{
				float dt = (time - prev_time) / 1000.f;
				fps = ntk::format_string("[ FPS ] %.01f", frame_count / dt);

				prev_time = time;
				frame_count = 0;
			}

			screen_dc.set_text_color(TEXT_COLOR);
			for(int i=0; i<2; ++i)// draw string as bold
				screen_dc.draw_string(fps, ntk_point(10 + i, 10));
		}

		static const ntk_string COPYRIGHT = "(C) snori. http://ntk.yi.org/";

		screen_dc.set_text_color(TEXT_COLOR);
		for(int i=0; i<2; ++i)
			screen_dc.draw_string(COPYRIGHT, ntk_point(
					SCREEN_WIDTH - (screen_dc.string_width(COPYRIGHT) + 10 + i),
					SCREEN_HEIGHT - 10 - screen_dc.string_height()));

		dc.draw_bitmap(screen);
	}

	return 0;
}
