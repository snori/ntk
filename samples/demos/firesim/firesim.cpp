/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: firesim.cpp,v 1.2 2003/08/04 12:01:20 nsuzuki Exp $

******************************************************************************/
#include "FireSim.h"

#include <stdio.h>
#include <mmsystem.h>
#include <ntk/interface/window.h>
#include "Particle.h"
#include "PreviewPanel.h"


class Win : public ntk::Window {
public:
	Win(ntk::Panel* panel)
	:	Window(
			panel, ntk::Rect(100, 100, 300, 400),
			ntk::application()->name() + " v" + ntk::application()->version().as_string())
	{}

	bool quit_requested()
	{
		ntk::application()->quit();
		return true;
	}
};

App::App()
:	Application("FireSIM(ple)", ntk::Version(1, 0, 0, 20020527, "FS-527"))
,	m_preview_panel(NULL)
{
	srand(timeGetTime());

	m_scene = new Scene;
	m_scene->add_field(new AirStream);
	m_scene->add_field(new Noise);
	m_scene->add_field(new Wind);
	m_scene->add_emitter(new Emitter(Vector(70, 200, 0), Vector(0, -1.f, 0)));
	m_scene->add_emitter(new Emitter(Vector(100, 200, 0), Vector(0, -1.f, 0)));
	m_scene->add_emitter(new Emitter(Vector(130, 200, 0), Vector(0, -1.f, 0)));

	Win* win = new Win(m_preview_panel = new PreviewPanel);
	win->show();
}

int
App::message_loop()
{
	MSG msg;

	long prev_time = timeGetTime(), time;

	while(true)
	{
		time = timeGetTime();

		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(! GetMessage(&msg, NULL, 0, 0))
				break;

			dispatch_msg(msg);
		}
		else if(time - prev_time > 20)
		{
			idle();
			prev_time = time;
		}
	}

	return msg.wParam;
}

void
App::idle()
{
	m_preview_panel->invalidate();
}

//########################################################

#include <ntk/main.h>

int
main(int argc, const char_t** argv)
{
	App app;
	app.run();

	return 0;
}
