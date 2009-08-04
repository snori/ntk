#include "newdocumentdialog.h"


#include <ntk/interface/dialogpanel.h>
#include <ntk/interface/screen.h>
#include "titoon.h"
#include "titoonwindow.h"
#include "animation.h"
#include "timecontrol.h"
#include "resource.h"


//########################################################


NewDocumentDialog::NewDocumentDialog()
:	NWindow(
		new NDialogPanel(
			MAKEINTRESOURCE(ID_NEW_DOCUMENT_DIALOG), NRect(), "NewDialog",
			NView::FOLLOW_LEFT | NView::FOLLOW_TOP, NDialogPanel::RESIZE_TO_FIT),
		NRect(100, 100, 300, 300), "êVãKçÏê¨", TITLED_LOOK, MODAL_APP_FEEL,
		NOT_RESIZABLE | NOT_ZOOMABLE | NOT_MINIMIZABLE)
{
	NDialogPanel* dialog_panel = reinterpret_cast<NDialogPanel*>(panel());
	SetDlgItemText(dialog_panel->handle(), ID_WIDTH, "160");
	SetDlgItemText(dialog_panel->handle(), ID_HEIGHT, "120");

	resize_to(dialog_panel->frame().width(), dialog_panel->frame().height());
	move_to(
		(NScreen::frame().width() - frame().width()) /2,
		(NScreen::frame().height() - frame().height()) /2);
}

LRESULT
NewDocumentDialog::system_command_received(uint command)
{
	switch(command)
	{
	case IDOK:
		{
			NDialogPanel* dialog_panel = reinterpret_cast<NDialogPanel*>(panel());

			char w_str[128], h_str[128];
			GetDlgItemText(dialog_panel->handle(), ID_WIDTH, w_str, 128);
			GetDlgItemText(dialog_panel->handle(), ID_HEIGHT, h_str, 128);

			int width = atoi(w_str);
			int height = atoi(h_str);

			if(width == 0 || height == 0)
				quit();

			titoon()->window()->set_animation(new Animation(width, height));
			titoon()->window()->animation()->insert_frame();
			titoon()->window()->time_control()->set_current_frame(0);

			quit();
		}
		break;

	case IDCANCEL:
		quit();
		break;

	default:
		return NWindow::system_message_received(command);
	}

	return 0;
}


//########################################################
