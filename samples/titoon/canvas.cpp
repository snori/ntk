#include "canvas.h"


#include <assert.h>
#include <ntk/interface/window.h>
#include "titoon.h"
#include "titoonwindow.h"
#include "animation.h"
#include "toolbox.h"
#include "timecontrol.h"
#include "paintbox.h"
#include "brushstroke.h"


#define for if(false); else for


//########################################################


void
Canvas::UndoBuffer::store_undo_data(int frame, const Image& image)
{
	if(! m_redo_stack.empty())
		m_redo_stack.clear();

	m_undo_stack.push_back(UndoDataPtr(new UndoData(frame, image)));

	if(m_undo_stack.size() > 20)// undo limit 20
		m_undo_stack.pop_front();
}

bool
Canvas::UndoBuffer::undo(int* frame, Image* image)
{
	if(
		frame == NULL
		||
		image == NULL
		||
		m_undo_stack.empty()
		)
		return false;

	UndoDataPtr data = m_undo_stack.back();
	m_undo_stack.pop_back();
	*frame = data->frame;
	*image = data->image;

	m_redo_stack.push_back(UndoDataPtr(new UndoData(
		data->frame, *titoon()->window()->animation()->image_at(data->frame))));

	return true;
}

bool
Canvas::UndoBuffer::redo(int* frame, Image* image)
{
	if(
		frame == NULL
		||
		image == NULL
		||
		m_redo_stack.empty()
		)
		return false;

	UndoDataPtr data = m_redo_stack.back();
	m_redo_stack.pop_back();
	*frame = data->frame;
	*image = data->image;

	m_undo_stack.push_back(UndoDataPtr(new UndoData(
		data->frame, *titoon()->window()->animation()->image_at(data->frame))));

	return true;
}


//########################################################


Canvas::Canvas(const NRect& frame, const NRGBColor& color)
:	NView(frame, "Canvas", FOLLOW_ALL_SIDES, DEFAULT_FLAGS | WILL_DRAW | FULL_UPDATE_ON_RESIZE, ntk::transparent_color())
,	m_image(NULL)
,	m_prev_image(NULL)
,	m_next_image(NULL)
,	m_painting(false)
,	m_view_color(color)
{
	set_zoom(1.f);
}

void
Canvas::undo()
{
	Animation* anim = titoon()->window()->animation();

	if(anim == NULL)
		return;

	int frame;
	Image image(anim->width(), anim->height());

	bool ret = m_undo_buffer.undo(&frame, &image);
	if(ret == false)
		return;

	Image* current_image = anim->image_at(frame);
	*current_image = image;

	invalidate();
}

void
Canvas::redo()
{
	Animation* anim = titoon()->window()->animation();

	if(anim == NULL)
		return;

	int frame;
	Image image(anim->width(), anim->height());

	bool ret = m_undo_buffer.redo(&frame, &image);
	if(ret == false)
		return;

	Image* current_image = anim->image_at(frame);
	*current_image = image;

	invalidate();
}

void
Canvas::store_undo_data()
{
	m_undo_buffer.store_undo_data(
		titoon()->window()->time_control()->current_frame(), *image());
}

void
Canvas::set_image(Image* image, Image* prev_image, Image* next_image)
{
	if(image == m_image && prev_image == m_prev_image && m_next_image == next_image)
		return;

	m_image = image;
	m_prev_image = prev_image;
	m_next_image = next_image;

	set_zoom(zoom());
}

void
Canvas::set_zoom(float zoom)
{
	m_zoom = zoom;
	invalidate();
}

void
Canvas::mouse_down(const NPoint& point, uint buttons, bool dbl)
{
	if(m_image == NULL)
		return;

	begin_capture();

	m_brush_stroke = titoon()->window()->tool_box()->paint_box()->brush_stroke();
	if(m_brush_stroke == NULL)
		return;

	m_painting = true;
	m_brush_stroke->mouse_down(convert_point_(point), buttons, dbl);
}

void
Canvas::mouse_up(const NPoint& point, uint buttons)
{
	if(m_painting)
	{
		m_brush_stroke->mouse_up(convert_point_(point), buttons);
		m_brush_stroke = NULL;

		m_painting = false;

		end_capture();
	}
}

void
Canvas::mouse_moved(const NPoint& point, uint buttons, uint transit, const NMessage* data)
{
	if(m_painting)
		m_brush_stroke->mouse_moved(convert_point_(point), buttons);
}

void
Canvas::lost_capture()
{
	m_painting = false;
}

void
Canvas::draw(NPaintDC& dc)
{
	NMemoryDC mem_dc(dc);
	mem_dc.set_bitmap(NDDB(dc, bounds().width(), bounds().height()));

	mem_dc.push_brush(m_view_color);
	mem_dc.fill_rect(bounds());
	mem_dc.pop_brush();

	mem_dc.push_pen(ntk::white_color());
	mem_dc.stroke_rect(bounds());
	mem_dc.pop_pen();

	if(m_image)
	{
		NRect rect(0, 0, m_image->width() * m_zoom, m_image->height() * m_zoom);
		rect.offset_to(
			(bounds().width() - rect.width()) /2,
			(bounds().height() - rect.height()) /2);

		if(m_prev_image || m_next_image)
		{
			assert(
				(m_prev_image == NULL ||
					m_image->width()  == m_prev_image->width() &&
					m_image->height() == m_prev_image->height())
				&&
				(m_next_image == NULL ||
					m_image->width()  == m_next_image->width() &&
					m_image->height() == m_next_image->height())
				);

			Image buffer(m_image->width(), m_image->height());

			int w = buffer.width();
			for(int y=0; y<buffer.height(); ++y)
			{
				uint8* bufbits = (uint8*)buffer.bitmap().at(0, y);
				uint8* current = (uint8*)m_image->bitmap().at(0, y);
				uint8* prev    = m_prev_image ? (uint8*)m_prev_image->bitmap().at(0, y) : NULL;
				uint8* next    = m_next_image ? (uint8*)m_next_image->bitmap().at(0, y) : NULL;

				for(int x=0; x<w; ++x)
				{
					struct func {
						static uint8 saturate(int val)
						{
							return val > 255 ? 255 : val < 0 ? 0 : (uint8)val;
						}
					};

					static const int PREV_FRAME_CHANNEL[] = {1,2};
					static const int NEXT_FRAME_CHANNEL[] = {0,1};

					if(m_prev_image)
						for(int i=0; i<2; ++i)
							bufbits[PREV_FRAME_CHANNEL[i]] =
								func::saturate(
									(int)bufbits[PREV_FRAME_CHANNEL[i]] - (255 - prev[PREV_FRAME_CHANNEL[i]]));

					if(m_next_image)
						for(int i=0; i<2; ++i)
							bufbits[NEXT_FRAME_CHANNEL[i]] =
								func::saturate(
									(int)bufbits[NEXT_FRAME_CHANNEL[i]] - (255 - next[NEXT_FRAME_CHANNEL[i]]));

					static const float ALPHA = 0.2f;
					bufbits[0] = func::saturate((int)current[0] - (255 - bufbits[0]) * ALPHA);
					bufbits[1] = func::saturate((int)current[1] - (255 - bufbits[1]) * ALPHA);
					bufbits[2] = func::saturate((int)current[2] - (255 - bufbits[2]) * ALPHA);

					bufbits += 3;
					current += 3;
					prev    += 3;
					next    += 3;
				}
			}

			mem_dc.draw_bitmap(buffer.bitmap(), rect);
		}
		else
			mem_dc.draw_bitmap(m_image->bitmap(), rect);

		mem_dc.set_pen(NPen(m_view_color - 50, 0, PS_DOT));

		{
			Animation* anim = titoon()->window()->animation();
			int current_frame = titoon()->window()->time_control()->current_frame();

			{
				NRect r = rect;
				coord num_images = r.left / r.width() + 1;
				for(int i=1; i<=num_images; ++i)
				{
					Image* image = anim->image_at(current_frame - i);
					if(image)
					{
						r.offset_by(-(r.width() +1), 0);
						mem_dc.draw_bitmap(image->bitmap(), r);
						mem_dc.stroke_line(r.left -1, r.top, r.left -1, r.bottom);
					}
				}
			}

			{
				NRect r = rect;
				coord num_images = (bounds().right - r.right) / r.width() + 1;
				for(int i=1; i<=num_images; ++i)
				{
					Image* image = anim->image_at(current_frame + i);
					if(image)
					{
						r.offset_by(r.width() +1, 0);
						mem_dc.draw_bitmap(image->bitmap(), r);
						mem_dc.stroke_line(r.right, r.top, r.right, r.bottom);
					}
				}
			}
		}

		NRect bounds_ = bounds();
		bounds_.inset_by(1, 1);
		rect.left -= 1;
		rect.top -= 1;

		mem_dc.stroke_line(rect.left, bounds_.top, rect.left, bounds_.bottom);
		mem_dc.stroke_line(rect.right, bounds_.top, rect.right, bounds_.bottom);
		mem_dc.stroke_line(bounds_.left, rect.top, bounds_.right, rect.top);
		mem_dc.stroke_line(bounds_.left, rect.bottom, bounds_.right, rect.bottom);

		if(titoon()->window()->animation())
		{
			NString str = ntk::format_string("#%d / %d",
			titoon()->window()->time_control()->current_frame() +1,
				titoon()->window()->animation()->length());

			NRect str_rect((bounds().width() - mem_dc.string_width(str)) /2, rect.bottom + 5, 0, 0);
			str_rect.right = str_rect.left + mem_dc.string_width(str);
			str_rect.bottom = str_rect.top + mem_dc.string_height();

			NRect back_rect = str_rect;
			back_rect.inset_by(-10, -3);

			mem_dc.set_brush(view_color() - 32);
			mem_dc.fill_rect(back_rect);

			mem_dc.set_pen(ntk::outline_color());
			mem_dc.stroke_rect(back_rect);

			mem_dc.set_text_color(mem_dc.brush().color() + 100);
			mem_dc.draw_string(str, str_rect.left_top());
		}
	}

	dc.bit_blt(bounds(), mem_dc, NPoint(0, 0), NDC::OP_COPY);
}

void
Canvas::attached_to_window()
{
	NView::attached_to_window();
	set_zoom(zoom());
}

NPoint
Canvas::convert_point_(const NPoint& point)
{
	return NPoint(
		(point.x - (bounds().width()  - m_image->width()  * m_zoom) /2) / m_zoom,
		(point.y - (bounds().height() - m_image->height() * m_zoom) /2) / m_zoom);
}


//########################################################
