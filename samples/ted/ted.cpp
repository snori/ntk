#include "ted.h"


#include <ntk/storage/directory.h>
#include <ntk/support/version.h>
#include <ntk/interface/alert.h>
#include "editwindow.h"
#include "resource.h"


//########################################################


ntk_setting ted_application_t::m_conf(
	NApplication::directory().path().as_string() + "\\ted.conf");


ted_application_t::ted_application_t()
:	ntk_application("TED", ntk_version(0, 1, 0))
{
	set_accelerator_handle(
		LoadAccelerators(handle(), MAKEINTRESOURCE(ID_APP_ACCELERATOR)));

	(new edit_window_t)->show();
}

ted_application_t::~ted_application_t()
{
}

void
ted_application_t::about_requested()
{
	ntk_string text =
		ted()->name() + " v" + ted()->version().as_string() + "\n"
		"NTK v" + NTK_Version.as_string() + " [" + NTK_Version.name() + "]\n" +
		"\n" +
		"(C) Noritaka Suzuki(S.Nori) <snori@alles.or.jp>\n" +
		"http://tyz.yi.org/\n";

	ntk_alert* about = new ntk_alert("ABOUT " + ted()->name(), text, "OK");
	about->go();
}


//########################################################


#include <ntk/main.h>

int
main(int argc, const char_t** argv)
{
	ted_application_t app;
	app.run();

	return 0;
}
