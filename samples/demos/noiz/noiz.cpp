#include <stdlib.h>
#include <ntk/interface/bitmap.h>
#include "../common/demo.h"
#include <ntk/main.h>


uint8
random()
{
	static uint a = rand(), b = rand(), c = rand();

	a += b;
	b += c;
	c += a;

	return (uint8)(a >> 16);
}

void
draw(ntk_dib* screen)
{
	int w = screen->width(), h = screen->height();

	for(int y=0; y<h; ++y)
	{
		uint8* p = (uint8*)screen->at(0, y);

		for(int x=0; x<w; ++x)
		{
			*p++ = random();
			*p++ = random();
			*p++ = random();
		}
	}
}


//////////////////////////////////////////////////////


class noiz_demo_t : public demo_t {
public:
	noiz_demo_t() : demo_t("N O I Z", ntk_version(1,0)) {}
};

int
main(int argc, const char_t** argv)
{
	srand(20030420);

	noiz_demo_t app;
	app.run();

	return 0;
}
