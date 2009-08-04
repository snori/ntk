#include "toolbox.h"


#include "iobox.h"
#include "editbox.h"
#include "paintbox.h"
#include "compositebox.h"
#include "effectbox.h"


//########################################################


ToolBox::ToolBox(const NRect& frame)
:	NView(frame, "ToolBox", FOLLOW_LEFT_RIGHT | FOLLOW_BOTTOM, DEFAULT_FLAGS, ntk::app_color())
,	m_active_box(NULL)
{
}

ToolBox::~ToolBox()
{
}

void
ToolBox::attached_to_window()
{
	NView::attached_to_window();

	set_up_controls_();
}

void
ToolBox::message_received(const NMessage& message)
{
	switch(message.what)
	{
	case NTabBar::TAB_CHANGED:
		{
			NView* box = find_view(message.find_string("tab-name"));
			if(box == NULL)
				return;

			if(m_active_box)
				m_active_box->hide();

			m_active_box = box;
			m_active_box->show();
		}
		break;

	default:
		NView::message_received(message);
	}
}

void
ToolBox::set_up_controls_()
{
	NRect& rect = bounds();

	{
		NRect r = rect;
		r.bottom = r.top + 20;

		m_tab_bar = new NTabBar(r, "TabBar", new NMessage(NTabBar::TAB_CHANGED), NView::FOLLOW_LEFT_RIGHT | NView::FOLLOW_TOP);
		m_tab_bar->set_target(this);
		add_child(m_tab_bar);

		rect.top += r.height();
	}

	set_up_boxes_(rect);
}

void
ToolBox::set_up_boxes_(const NRect& frame)
{
	add_child(m_io_box        = new IOBox(frame, "I/O"));
	add_child(m_edit_box      = new EditBox(frame, "Edit"));
	add_child(m_paint_box     = new PaintBox(frame, "Paint"));
	add_child(m_composite_box = new CompositeBox(frame, "Composite"));
	add_child(m_effect_box    = new EffectBox(frame, "Effect"));

	m_tab_bar->add_tab("I/O");
	m_tab_bar->add_tab("Edit");
	m_tab_bar->add_tab("Paint");
//	m_tab_bar->add_tab("Composite");
//	m_tab_bar->add_tab("Effect");

	m_io_box->hide();
	m_edit_box->hide();
	m_paint_box->hide();
	m_composite_box->hide();
	m_effect_box->hide();

	m_tab_bar->activate_tab("Paint");
}


//########################################################
