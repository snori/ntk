#include "brushstroke.h"


#include <ntk/support/utility.h>
#include "titoon.h"
#include "titoonwindow.h"
#include "animation.h"
#include "canvas.h"
#include "toolbox.h"
#include "paintbox.h"
#include "brush.h"
#include "image.h"


//########################################################


BrushStroke::BrushStroke()
:	m_painting(false)
,	m_mouse_down_outside(false)
{
	m_canvas = titoon()->window()->canvas();
}

BrushStroke::~BrushStroke()
{
}

void
BrushStroke::set_painting(bool flag)
{
	m_painting = flag;
}

void
BrushStroke::mouse_down(const NPoint& point, uint buttons, bool dbl)
{
	m_brush = titoon()->window()->tool_box()->paint_box()->brush();


	if(NRect(0, 0, m_canvas->image()->width(), m_canvas->image()->height()).contains(point))
		begin_painting();
	else
		m_mouse_down_outside = true;
}

void
BrushStroke::mouse_up(const NPoint& point, uint buttons)
{
	if(m_painting)
		end_painting();
}

void
BrushStroke::mouse_moved(const NPoint& point, uint buttons)
{
	if(m_mouse_down_outside &&
		NRect(0, 0, m_canvas->image()->width(), m_canvas->image()->height()).contains(point))
	{
		m_mouse_down_outside = false;
		begin_painting();
	}
}

void
BrushStroke::begin_painting()
{
	titoon()->window()->animation()->set_modification_flag(true);

	m_canvas->store_undo_data();
	set_painting(true);
}

void
BrushStroke::end_painting()
{
	set_painting(false);
}

void
BrushStroke::paint(const NPoint& point)
{
	if(m_brush == NULL || m_canvas == NULL)
		return;

	m_brush->paint(&m_canvas->image()->bitmap(), point);
	m_canvas->invalidate();
}


//########################################################


void
DiscreteBrushStroke::mouse_down(const NPoint& point, uint buttons, bool dbl)
{
	BrushStroke::mouse_down(point, buttons, dbl);
	paint(point);
}

void
DiscreteBrushStroke::mouse_moved(const NPoint& point, uint buttons)
{
	if(painting())
		paint(point);
}


//########################################################


void
SequentialBrushStroke::mouse_down(const NPoint& point, uint buttons, bool dbl)
{
	BrushStroke::mouse_down(point, buttons, dbl);
	paint(point);

	m_prev_point = point;
}

void
SequentialBrushStroke::mouse_moved(const NPoint& point, uint buttons)
{
	if(painting())
	{
		struct func {
			static coord abs(coord val) {return val < 0 ? -val : val;}
			static coord sgn(coord val) {return val < 0 ?   -1 :   1;}
		};

		coord x1 = m_prev_point.x, y1 = m_prev_point.y;
		coord x2 = point.x,        y2 = point.y;

		coord dx = func::abs(x2 - x1);
		coord dy = func::abs(y2 - y1);
		coord sx = func::sgn(x2 - x1);
		coord sy = func::sgn(y2 - y1);

		coord x = x1;
		coord y = y1;
		if(dx >= dy)
		{
			coord d = -dx;

			for(int i=0; i<=dx; ++i)
			{
				paint(NPoint(x, y));

				x += sx;
				d += dy * 2;

				if(d >= 0)
				{
					y += sy;
					d -= dx * 2;
				}
			}
		}
		else
		{
			coord d = -dy;

			for(int i=0; i<=dy; ++i)
			{
				paint(NPoint(x, y));

				y += sy;
				d += dx * 2;

				if(d >= 0)
				{
					x += sx;
					d -= dy * 2;
				}
			}
		}

		m_prev_point = point;
	}

	BrushStroke::mouse_moved(point, buttons);
}


//########################################################
