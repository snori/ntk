#include "timecontrol.h"


#include "titoon.h"
#include "titoonwindow.h"
#include "canvas.h"
#include "animation.h"


#define for if(false); else for


//########################################################


class TimeBar : public NView {
public:
	TimeBar(const NRect& frame)
	:	NView(frame, "TimeBar", FOLLOW_ALL_SIDES, DEFAULT_FLAGS | WILL_DRAW, ntk::app_color())
	,	m_mouse_flag(false)
	{
	}

	void draw(NPaintDC& dc)
	{
		NRect rect = bounds();

		dc.set_pen(ntk::outline_color());
		dc.stroke_rect(rect);

		rect.inset_by(1, 1);

		dc.set_brush(ntk::app_color());
		dc.fill_rect(rect);

		Animation* anim = titoon()->window()->animation();
		if(anim == NULL)
			return;

		rect.inset_by(2, 2);

		dc.set_pen(view_color() - 50);
		for(int i=0; i < anim->length(); ++i)
		{
			enum{ SPACE=3 };

			coord x = rect.left;
			x += ((float)i / (float)anim->length()) * rect.width();
			x += ((float)rect.width() / (float)anim->length()) /2.f;
			dc.stroke_line(x, rect.top + SPACE, x, rect.bottom -SPACE);
		}

		coord x = rect.left;
		x += ((float)time_control()->current_frame() / (float)anim->length()) * rect.width();
		x += ((float)rect.width() / (float)anim->length()) / 2.f;
		dc.set_pen(ntk::red_color());
		dc.stroke_rect(x -2, rect.top, x +3, rect.bottom);
	}

	void mouse_down(const NPoint& point, uint buttons, bool)
	{
		m_mouse_flag = true;
		begin_capture();

		point_frame_(point);
	}
	void mouse_up(const NPoint& point, uint buttons)
	{
		end_capture();
		m_mouse_flag = false;
	}
	void mouse_moved(const NPoint& point, uint buttons, uint transit, const NMessage* data)
	{
		if(m_mouse_flag)
			point_frame_(point);
	}
	void lost_capture()
	{
		m_mouse_flag = false;
	}

private:
	bool m_mouse_flag;

	TimeControl* time_control() const {return (TimeControl*)parent();}
	void point_frame_(const NPoint& point)
	{
		Animation* anim = titoon()->window()->animation();
		if(anim == NULL)
			return;

		NRect rect = frame();
		if(rect.contains(point) == false)
			return;

		rect.inset_by(2, 2);
		time_control()->set_current_frame(
			((float)(point.x - rect.left) / (float)rect.width()) * anim->length());
	}

};// class TimeBar


//########################################################


TimeControl::TimeControl(const NRect& frame)
:	NView(frame, "TimeControl", FOLLOW_LEFT_RIGHT | FOLLOW_BOTTOM, DEFAULT_FLAGS | WILL_DRAW | FULL_UPDATE_ON_RESIZE, ntk::transparent_color())
,	m_current_frame(-1)
{
	set_up_controls_();
}

int
TimeControl::current_frame() const
{
	return m_current_frame;
}

void
TimeControl::set_current_frame(int frame)
{
	Animation* anim = titoon()->window()->animation();
	if(anim == NULL || anim->length() == 0)
	{
		m_current_frame = -1;
		return;
	}

	m_current_frame = frame;

	if(m_current_frame < 0)
		m_current_frame = 0;
	else if(m_current_frame >= anim->length())
		m_current_frame = anim->length() -1;

	titoon()->window()->canvas()->set_image(
		anim->image_at(current_frame()),
		anim->image_at(current_frame() -1),
		anim->image_at(current_frame() +1));

	invalidate();
}

void
TimeControl::set_up_controls_()
{
	NRect rect = bounds();

	enum{ TEXT_CONTROL_WIDTH=64 };
#if 0
	{
		NRect r = rect;
		r.right = r.left + TEXT_CONTROL_WIDTH;

		m_min_frame_text = new NScintilla(r, "MinFrame", "0", NView::FOLLOW_LEFT | FOLLOW_TOP_BOTTOM);
		m_min_frame_text->set_ex_style(m_min_frame_text->ex_style() | WS_EX_CLIENTEDGE, true);
		add_child(m_min_frame_text);

		rect.left += r.width();
	}

	{
		NRect r = rect;
		r.left = r.right - TEXT_CONTROL_WIDTH;

		m_max_frame_text = new NScintilla(r, "MaxFrame", "1", NView::FOLLOW_RIGHT | FOLLOW_TOP_BOTTOM);
		m_max_frame_text->set_ex_style(m_max_frame_text->ex_style() | WS_EX_CLIENTEDGE, true);
		add_child(m_max_frame_text);

		rect.right -= r.width();
	}
#endif
	{
		add_child(new TimeBar(rect));
	}
}


//########################################################
