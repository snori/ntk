#include "previewpanel.h"


#include "titoon.h"
#include "titoonwindow.h"
#include "animation.h"
#include "image.h"


//########################################################


PreviewPanel::PreviewPanel(const NString& name)
:	NPanel(NRect(), name, DEFAULT_RESIZING_MODE, DEFAULT_FLAGS, ntk::white_color())
,	m_play_thread(NULL)
,	m_stop(false)
,	m_pause(false)
,	m_animation(NULL)
{
}

void
PreviewPanel::play(Animation* anim)
{
	if(anim == NULL)
		return;

	m_animation = anim;

	if(m_play_thread == NULL)
	{
		m_stop = false;
		m_play_thread = ntk::spawn_thread(
			play_anim_, "PlayAnimationThread", ntk::NORMAL_PRIORITY, this);
	}

	m_pause = false;
	ntk::resume_thread(m_play_thread);
}

void
PreviewPanel::stop()
{
	m_stop = true;
	ntk::wait_for_thread(m_play_thread);
}

void
PreviewPanel::pause()
{
	m_pause = true;
}

uint
PreviewPanel::play_anim_(void* data)
{
	PreviewPanel* self = reinterpret_cast<PreviewPanel*>(data);
	NDC dc(self->hwnd());

	int current_frame = 0;
	while(self->m_stop == false)
	{
		if(self->m_pause)
			ntk::suspend_thread(self->m_play_thread);

		Animation::Frame* frame = self->m_animation->frame_at(current_frame);

		if(frame)
		{
			if(frame->image)
				dc.draw_bitmap(frame->image->bitmap());

			int delay = frame->delay;
			if(frame->delay < 0.f)
				delay = 100;//default_delay;

			if(delay > 0)
				ntk::snooze(delay);
		}

		current_frame++;

		if(current_frame >= self->m_animation->length())
			current_frame = 0;
	}

	self->m_play_thread = NULL;

	return 0;
}


//########################################################


PreviewWindow::PreviewWindow(const NRect& frame)
:	NWindow(
		new PreviewPanel("PreviewPanel"),
		titoon()->conf().get_rect("PreviewWindow::frame", frame),
		"Preview", FLOATING_WINDOW)
{
	m_preview = static_cast<PreviewPanel*>(panel());
}

PreviewWindow::~PreviewWindow()
{
	titoon()->conf().set_rect("PreviewWindow::frame", frame());
}

void
PreviewWindow::play(Animation* anim)
{
	m_preview->play(anim);
}

bool
PreviewWindow::quit_requested()
{
	m_preview->stop();
	return true;
}


//########################################################
