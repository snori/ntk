#include "paintbox.h"


#include <ntk/interface/button.h>
#include <ntk/interface/layout.h>
#include "titoon.h"
#include "titoonwindow.h"
#include "canvas.h"
#include "brush.h"
#include "brushstroke.h"
#include "resource.h"


//########################################################


namespace {


enum {
	BRUSH_CHANGED        = 'BRCH',
	BRUSH_STROKE_CHANGED = 'BSCH',
};


}


PaintBox::PaintBox(const NRect& frame, const NString& name)
:	NView(frame, name, FOLLOW_ALL_SIDES, DEFAULT_FLAGS, ntk::app_color())
,	m_brush(NULL)
,	m_brush_stroke(NULL)
{
	set_up_controls_();
}

PaintBox::~PaintBox()
{
	delete m_brush;
	delete m_brush_stroke;
}

void
PaintBox::set_brush(Brush* brush)
{
	delete m_brush;
	m_brush = brush;
}

void
PaintBox::set_brush_stroke(BrushStroke* stroke)
{
	delete m_brush_stroke;
	m_brush_stroke = stroke;
}

void
PaintBox::message_received(const NMessage& message)
{
	switch(message.what)
	{
	case BRUSH_CHANGED:
		{
			NString type = message.find_string("type");

			if(type == "flat1")
				set_brush(new FlatBrush(1));
			else if(type == "flat2")
				set_brush(new FlatBrush(2));
			else if(type == "flat3")
				set_brush(new FlatBrush(3));
			else if(type == "clone1")
				set_brush(new CloneBrush(1));
			else if(type == "clone3")
				set_brush(new CloneBrush(3));
			else if(type == "clone5")
				set_brush(new CloneBrush(5));
			else if(type == "eraser1")
				set_brush(new FlatBrush(1, NRGBColor(255, 255, 255)));
			else if(type == "eraser2")
				set_brush(new FlatBrush(2, NRGBColor(255, 255, 255)));
			else if(type == "eraser3")
				set_brush(new FlatBrush(3, NRGBColor(255, 255, 255)));
		}
		break;

	case BRUSH_STROKE_CHANGED:
		{
			NString type = message.find_string("type");

			if(type == "dot")
				set_brush_stroke(new DiscreteBrushStroke);
			else if(type == "line")
				set_brush_stroke(new SequentialBrushStroke);
		}
		break;

	default:
		NView::message_received(message);
	}
}

void
PaintBox::set_up_controls_()
{
	enum{ SPACE = 3, WIDTH = 120, HEIGHT = 17 };

	NLayout layout_0(this, 0, -1);
	layout_0.rest_rect().inset_by(SPACE, SPACE);

	{
		NLayout layout_1 = layout_0.split(WIDTH, NLayout::LEFT);

		NButton* button;
		NMessage* message;

		message = new NMessage(WM_COMMAND, IDM_EDIT_INSERT_FRAME, 0);
		button = new NButton(NRect(), "Insert", "Insert Frame", message);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(WM_COMMAND, IDM_EDIT_ERASE_FRAME, 0);
		button = new NButton(NRect(), "Erase", "Erase Frame", message);
		layout_1.add_child(button, HEIGHT);

		layout_0 = layout_0.split(layout_0.rest_rect().width() - WIDTH, NLayout::RIGHT);
		layout_0.rest_rect().left += SPACE;
	}

	{
		NLayout layout_1 = layout_0.split(WIDTH, NLayout::LEFT);

		NButton* button;
		NMessage* message;

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "flat1");
		button = new NButton(NRect(), "FlatBrush 1", "Flat Brush 1x1", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "flat2");
		button = new NButton(NRect(), "FlatBrush 2", "Flat Brush 2x2", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "flat3");
		button = new NButton(NRect(), "FlatBrush 3", "Flat Brush 3x3", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "clone1");
		button = new NButton(NRect(), "CloneBrush 1", "Clone Brush 1x1", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "clone3");
		button = new NButton(NRect(), "CloneBrush 3", "Clone Brush 3x3", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "clone5");
		button = new NButton(NRect(), "CloneBrush 5", "Clone Brush 5x5", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "eraser1");
		button = new NButton(NRect(), "Eraser 1", "Eraser 1x1", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "eraser2");
		button = new NButton(NRect(), "Eraser 2", "Eraser 2x2", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_CHANGED);
		message->add_string("type", "eraser3");
		button = new NButton(NRect(), "Eraser 3", "Eraser 3x3", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		layout_0 = layout_0.split(layout_0.rest_rect().width() - WIDTH, NLayout::RIGHT);
		layout_0.rest_rect().left += SPACE;
	}

	{
		NLayout layout_1 = layout_0.split(WIDTH, NLayout::LEFT);

		NButton* button;
		NMessage* message;

		message = new NMessage(BRUSH_STROKE_CHANGED);
		message->add_string("type", "dot");
		button = new NButton(NRect(), "Dot Brush Stroke", "Dot Brush Stroke", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		message = new NMessage(BRUSH_STROKE_CHANGED);
		message->add_string("type", "line");
		button = new NButton(NRect(), "Line Brush Stroke", "Line Brush Stroke", message);
		button->set_target(this);
		layout_1.add_child(button, HEIGHT);

		layout_0 = layout_0.split(WIDTH, NLayout::RIGHT);
		layout_0.rest_rect().left += SPACE;
	}
}


//########################################################
