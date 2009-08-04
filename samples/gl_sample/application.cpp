#include "application.h"


#include <ntk/storage/directory.h>
#include "window.h"
#include "aboutwindow.h"


//########################################################


ntk_setting gl_application_t::g_conf(ntk_application::directory().path().as_string() + "\\gl_sample.conf");


gl_application_t::gl_application_t()
:	ntk_application("NTK/GLView-SAMPLE", ntk_version(1,0,0,0, __DATE__))
{
	gl_window_t* win = new gl_window_t;
	win->show();
}

void
gl_application_t::about_requested()
{
	about_window_t* win = new about_window_t;
	win->show();
}
