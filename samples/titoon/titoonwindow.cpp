#include "titoonwindow.h"


#include <ntk/application/messenger.h>
#include <ntk/application/clipboard.h>
#include <ntk/interface/alert.h>
#include <ntk/interface/panel.h>
#include <ntk/interface/filepanel.h>
#include <ntk/storage/file.h>
#include <ntk/translation/utility.h>
#include "titoon.h"
#include "animation.h"
#include "canvas.h"
#include "toolbox.h"
#include "timecontrol.h"
#include "previewpanel.h"
#include "zfile.h"
#include "newdocumentdialog.h"
#include "resource.h"


//########################################################


TitoonWindow::TitoonWindow(const NRect& frame)
:	NWindow(
		titoon()->conf().get_rect("TitoonWindow::frame", frame),
		titoon()->name(),
		TITLED_WINDOW)
,	m_animation(NULL)
{
	set_menu_handle(LoadMenu(titoon()->handle(), MAKEINTRESOURCE(ID_TITOON_MENU)));

	reset_title_();

	show();
	hide();

	if(titoon()->conf().get_bool("TitoonWindow::is_zoomed", false))
		zoom();

	set_up_controls_();
}

TitoonWindow::~TitoonWindow()
{
	titoon()->conf().set_bool("TitoonWindow::is_zoomed", is_zoomed());

	NRect rect;
	if(is_zoomed())
		rect = m_frame_before_zoom;
	else
		rect = frame();

	titoon()->conf().set_rect("TitoonWindow::frame", rect);

	delete m_animation;
}

void
TitoonWindow::set_animation(Animation* animation)
{
	if(animation == m_animation)
		return;

	if(m_animation)
		m_animation->remove_observer(NMessenger(NULL, this));

	delete m_animation;
	m_animation = animation;

	if(m_animation)
		m_animation->add_observer(NMessenger(NULL, this));

	reset_title_();

	invalidate();
}

void
TitoonWindow::invalidate()
{
	panel()->invalidate();
}

void
TitoonWindow::zoom(const ntk_point& origin, coord width, coord height)
{
	if(! is_zoomed())
		m_frame_before_zoom = frame();

	NWindow::zoom(origin, width, height);
}

bool
TitoonWindow::quit_requested()
{
	if(animation() && animation()->is_modified())
	{
		int ret = ntk::alert("WARNING", "Are you sure you want to quit without saving modified document?", "Yes", "No", "Cancel");
		if(ret != 0)
			return false;
	}

	titoon()->quit();
	return true;
}

void
TitoonWindow::message_received(const NMessage& message)
{
	switch(message.what)
	{
	case ntk::REFS_RECEIVED:
		{
			ntk::ZFILE file(message.find_string("name"), "rb");
			if(file.init_check() == false)
				return;

			Animation* anim = Animation::load(&file);
			if(anim == NULL)
			{
				file.unset();

				NFILE nfile(message.find_string("name"), "rb");
				if(nfile.init_check() == false)
					return;

				anim = Animation::load(&nfile);
				if(anim == NULL)
					return;
			}

			set_animation(anim);
			anim->set_modification_flag(false);
			anim->set_path(file.path());

			time_control()->set_current_frame(0);
		}
		break;

	case ntk::SAVE_REQUESTED:
		{
			if(animation() == NULL)
				return;

			NString file_name = message.find_string("name");

			ntk::ZFILE file(file_name, "wb");
			if(file.init_check() == false)
				return;

			animation()->save(&file);
			animation()->set_modification_flag(false);

			if(animation()->path() != file_name)
				animation()->set_path(file_name);
		}
		break;

	case IDM_FILE_EXPORT_SEQUENTIAL_IMAGE:
		{
			Animation* anim = animation();
			if(anim == NULL)
				return;

			NString file_name_prefix = message.find_string("name");

			for(int i=0; i<anim->length(); ++i)
				NTranslationUtils::store_bitmap(
					file_name_prefix + "_" + ntk::format_string("%d", i) + ".bmp",
					anim->image_at(i)->bitmap());
		}
		break;

	case Document::MODIFICATION_FLAG_CHANGED:
	case Document::PATH_CHANGED:
		reset_title_();
		break;

	default:
		NWindow::message_received(message);
	}
}

LRESULT
TitoonWindow::system_command_received(uint command)
{
	switch(command)
	{
	case IDM_FILE_NEW:
		if(animation() && animation()->is_modified())
		{
			int ret = ntk::alert("WARNING", "Current document is not saved.\nDo you cancel the modification and create a new document?", "Yes", "No", "Cancel");
			if(ret != 0)
				break;
		}

		(new NewDocumentDialog)->show();
		break;

	case IDM_FILE_OPEN:
		if(animation() && animation()->is_modified())
		{
			int ret = ntk::alert("WARNING", "Current document is not saved.\nDo you cancel the modification and open another document?", "Yes", "No", "Cancel");
			if(ret != 0)
				break;
		}

		NFilePanel(NFilePanel::OPEN_PANEL, NMessenger(NULL, this)).show();
		break;

	case IDM_FILE_SAVE:
		{
			if(animation() == NULL)
				break;

			if(animation()->has_path())
			{
				NMessage message(ntk::SAVE_REQUESTED);
				message.add_string("name", animation()->path().as_string());

				post_message(message);
			}
			else
				NFilePanel(NFilePanel::SAVE_PANEL, NMessenger(NULL, this)).show();
		}
		break;

	case IDM_FILE_SAVE_AS:
		NFilePanel(NFilePanel::SAVE_PANEL, NMessenger(NULL, this)).show();
		break;

	case IDM_FILE_IMPORT_EZT:
		ntk::alert("Sorry", "Not Implemented...");
		break;

	case IDM_FILE_EXPORT_EZT:
		ntk::alert("Sorry", "Not Implemented...");
		break;

	case IDM_FILE_EXPORT_SEQUENTIAL_IMAGE:
		{
			NMessage* message = new NMessage(IDM_FILE_EXPORT_SEQUENTIAL_IMAGE);
			NFilePanel p(NFilePanel::SAVE_PANEL, NMessenger(NULL, this));
			p.set_message(message);
			p.show();
		}
		break;

	case IDM_FILE_EXIT:
		quit();
		break;

	case IDM_EDIT_UNDO:
		canvas()->undo();
		break;

	case IDM_EDIT_REDO:
		canvas()->redo();
		break;

	case IDM_EDIT_CUT:
		cut_(time_control()->current_frame());
		break;

	case IDM_EDIT_CUT_ERASE:
		{
			if(animation() == NULL)
				break;

			int frame = time_control()->current_frame();

			cut_(frame);
			animation()->erase_frame(frame);
			time_control()->set_current_frame(frame);
		}
		break;

	case IDM_EDIT_COPY:
		copy_(time_control()->current_frame());
		break;

	case IDM_EDIT_PASTE:
		paste_(time_control()->current_frame());
		break;

	case IDM_EDIT_PASTE_INSERT:
		{
			if(animation() == NULL)
				break;
			
			int frame = time_control()->current_frame();

			animation()->insert_frame(frame +1);
			time_control()->set_current_frame(frame +1);
			paste_(frame +1);
		}
		break;

	case IDM_EDIT_INSERT_FRAME:
		if(animation())
		{
			int frame = time_control()->current_frame() +1;

			animation()->insert_frame(frame);
			time_control()->set_current_frame(frame);
		}
		break;

	case IDM_EDIT_ERASE_FRAME:
		if(animation())
		{
			int frame = time_control()->current_frame();

			animation()->erase_frame(frame);
			time_control()->set_current_frame(frame);
		}
		break;

	case IDM_VIEW_NEXT_FRAME:
		time_control()->set_current_frame(time_control()->current_frame() +1);
		break;

	case IDM_VIEW_PREV_FRAME:
		time_control()->set_current_frame(time_control()->current_frame() -1);
		break;

	case IDM_VIEW_ZOOM_IN:
		canvas()->set_zoom(canvas()->zoom() * 2);
		break;

	case IDM_VIEW_ZOOM_OUT:
		canvas()->set_zoom(canvas()->zoom() / 2);
		break;

	case IDM_VIEW_ZOOM_25:
		canvas()->set_zoom(.25f);
		break;

	case IDM_VIEW_ZOOM_50:
		canvas()->set_zoom(.5f);
		break;

	case IDM_VIEW_ZOOM_100:
		canvas()->set_zoom(1);
		break;

	case IDM_VIEW_ZOOM_200:
		canvas()->set_zoom(2);
		break;

	case IDM_VIEW_ZOOM_400:
		canvas()->set_zoom(4);
		break;

	case IDM_VIEW_ZOOM_800:
		canvas()->set_zoom(8);
		break;

	case IDM_VIEW_ZOOM_1600:
		canvas()->set_zoom(16);
		break;

	case IDM_TOOL_PREVIEW_ANIMATION:
		{
			PreviewWindow* preview_window = new PreviewWindow(NRect(100, 100, 300, 280));
			preview_window->show();
			preview_window->play(animation());
		}
		break;

	case IDM_HELP_ABOUT:
		titoon()->post_message(ntk::ABOUT_REQUESTED);
		break;

	default:
		return NWindow::system_command_received(command);
	}

	return 0;
}

void
TitoonWindow::set_up_controls_()
{
	NRect rect = bounds();

	{
		NRect r = rect;
		r.top = r.bottom - 180;

		m_tool_box = new ToolBox(r);
		add_child(m_tool_box);

		rect.bottom -= r.height();
	}

	{
		NRect r = rect;
		r.top = r.bottom - 20;

		m_time_control = new TimeControl(r);
		add_child(m_time_control);

		rect.bottom -= r.height();
	}

	{
		m_canvas = new Canvas(rect);
		add_child(m_canvas);
	}
}

void
TitoonWindow::reset_title_()
{
	NString title = titoon()->name();

	if(animation())
	{
		title += " - [";

		if(animation()->path().init_check())
			title += animation()->path().leaf();
		else
			title += "untitled";

		if(animation()->is_modified())
			title += " *";

		title += "]";
	}

	set_title(title);
}

void
TitoonWindow::cut_(int frame)
{
	copy_(frame);

	if(animation() == NULL)
		return;

	Image* image = animation()->image_at(frame);
	if(image == NULL)
		return;

	NDC dc = image->bitmap().dc();
	dc.push_brush(ntk::white_color());
	dc.fill_rect(0, 0, image->width(), image->height());
	dc.pop_brush();

	invalidate();
}

void
TitoonWindow::copy_(int frame)
{
	NClipboard clip(hwnd());

	if(clip.lock() == false)
		return;

	if(animation() == NULL)
		return;

	Image* image = animation()->image_at(frame);
	if(image == NULL)
		return;

	clip.clear();

	NMemoryDC dc(image->bitmap().dc());
	NDDB bitmap(CreateCompatibleBitmap(
		image->bitmap().dc(), image->width(), image->height()), false);
	dc.set_bitmap(bitmap);

	dc.bit_blt(0, 0, image->width(), image->height(),
		image->bitmap().dc(), 0, 0, NDC::OP_COPY);

	NMessage* data = clip.data();
	data->add_int("type", CF_BITMAP);
	data->add_pointer("data", bitmap.handle());

	clip.commit();
	clip.unlock();
}

void
TitoonWindow::paste_(int frame)
{
	if(animation() == NULL)
		return;

	Image* image = animation()->image_at(frame);
	if(image == NULL)
		return;

	NClipboard clip(hwnd());
	if(clip.lock() == false)
		return;;

	NMemoryDC mem_dc(image->bitmap().dc());
	mem_dc.set_bitmap(NDDB((HBITMAP)GetClipboardData(CF_BITMAP), false));

	image->bitmap().dc().bit_blt(
		0, 0, image->width(), image->height(), mem_dc, 0, 0, NDC::OP_COPY);

	clip.unlock();

	invalidate();
}


//########################################################
