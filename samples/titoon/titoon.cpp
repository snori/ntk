#include "titoon.h"


#include <ntk/storage/directory.h>
#include "animation.h"
#include "titoonwindow.h"
#include "resource.h"

#include "toolbox.h"
#include "paintbox.h"
#include "brush.h"
#include "brushstroke.h"


//########################################################


NSetting Titoon::m_conf(NApplication::directory().path().as_string() + "\\titoon.conf");


Titoon::Titoon()
:	NApplication("Titoon", NVersion(1, 0, 1, 0))
{
	set_accelerator_handle(
		LoadAccelerators(handle(), MAKEINTRESOURCE(ID_TITOON_ACCELERATOR)));

	m_titoon_window = new TitoonWindow(NRect(100, 100, 500, 500));
	m_titoon_window->show();

	window()->tool_box()->paint_box()->set_brush(new FlatBrush);
	window()->tool_box()->paint_box()->set_brush_stroke(new SequentialBrushStroke);
}

Titoon::~Titoon()
{
}


//########################################################


#include <ntk/main.h>

int
main(int argc, const char_t** argv)
{
	Titoon app;
	app.run();

	return 0;
}
