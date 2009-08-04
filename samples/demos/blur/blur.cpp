#include <stdlib.h>
#include <math.h>
#include <ntk/interface/bitmap.h>
#include <ntk/storage/directory.h>
#include <ntk/storage/path.h>
#include <ntk/translation/utility.h>
#include "../common/demo.h"
#include <ntk/main.h>


class blur_demo_t : public demo_t {
public:
	blur_demo_t() : demo_t("B L U R", ntk_version(1,0))
	{
		ntk_path path = ntk_application::directory().path().as_string() + "/../samples/demos/blur/hiraokun.bmp";
		path.normalize();

		m_hiraokun = ntk::TranslationUtils::get_bitmap(path.as_string());
	}
	~blur_demo_t()
	{
		delete m_hiraokun;
	}

	ntk_dib* hiraokun() const {return m_hiraokun;}

private:
	ntk_dib* m_hiraokun;
};


inline blur_demo_t* app() {return (blur_demo_t*)ntk::application();}


int
main(int argc, const char_t** argv)
{
	blur_demo_t app;
	app.run();

	return 0;
}


//////////////////////////////////////////////////////


void
blend(ntk_dib* dest_screen, ntk_dib* current_screen, ntk_dib* prev_screen, float ratio)
{
	int pitch = dest_screen->width() * 3, h = dest_screen->height();
	uint8 r = ((uint)(ratio * 255)) & 255;

	for(int y=0; y<h; ++y)
	{
		uint8* dest = (uint8*)dest_screen->at(0, y);
		uint8* curr = (uint8*)current_screen->at(0, y);
		uint8* prev = (uint8*)prev_screen->at(0, y);

		for(int x=0; x<pitch; ++x)
		{
				*dest = (*curr * (255 - r) + *prev * r) / 255;
				dest++;
				curr++;
				prev++;
		}
	}
}

void
draw(ntk_dib* screen)
{
	enum {
		SCREEN_WIDTH  = 320,
		SCREEN_HEIGHT = 200,
	};

	static ntk_dib tmp(SCREEN_WIDTH, SCREEN_HEIGHT, ntk_dib::RGB_24);
	static ntk_dib prev(SCREEN_WIDTH, SCREEN_HEIGHT, ntk_dib::RGB_24);
	static deg = 0;

	prev.dc().draw_bitmap(*screen);

	ntk_dc& dc = tmp.dc();

	dc.push_brush();
	dc.set_brush(ntk::black_color());
	dc.fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	dc.pop_brush();

	dc.draw_bitmap(*app()->hiraokun(), ntk_point(
		SCREEN_WIDTH / 2 - app()->hiraokun()->width() / 2,
		SCREEN_HEIGHT / 2 - app()->hiraokun()->height() / 2));

	float rad = 3.1415f * 2.f * (deg / 360.f);
	dc.draw_bitmap(*app()->hiraokun(), ntk_point(
		SCREEN_WIDTH / 2 + cos(rad) * 100 - app()->hiraokun()->width() / 2,
		SCREEN_HEIGHT / 2 + sin(rad) * 50 - app()->hiraokun()->height() / 2));

	deg += 5;
	deg %= 360;

	blend(screen, &tmp, &prev, 0.75f);
}
