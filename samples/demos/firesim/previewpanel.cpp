/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: previewpanel.cpp,v 1.2 2003/05/02 19:04:51 snori Exp $

******************************************************************************/
#include "PreviewPanel.h"

#include <ntk/interface/window.h>
#include <ntk/interface/menubar.h>

class GDIPreview : public ntk::View {
public:
	GDIPreview()
	:	ntk::View(ntk::Rect(0, 0, 10, 10), "Preview", FOLLOW_ALL_SIDES, WILL_DRAW | FRAME_EVENTS | NAVIGABLE, ntk::transparent_color())
	,	m_blur(true)
	{
		m_back_screen = new ntk::Bitmap(100, 100, ntk::Bitmap::RGB_24);
	}
	~GDIPreview()
	{
		delete m_back_screen;
	}

	void draw(ntk::PaintDC& dc)
	{
		memset(m_back_screen->bits(), 0, m_back_screen->size());

		app()->scene()->next_frame();
		app()->scene()->draw(m_back_screen->dc());

		ntk::Bitmap new_bitmap(*m_back_screen);

		if(m_blur)
		{
			coord width = m_back_screen->width() -20;
			coord height = m_back_screen->height() - 20;

			enum{ BLUR_RADIUS = 5, BLUR_RADIUS_2 = BLUR_RADIUS*BLUR_RADIUS };

			for(int y=20; y<height; ++y)
			for(int x=20; x<width; ++x)
			{
				int val[3], num = 0;
				val[0] = val[1] = val[2] = 0;

				for(int yy=y; yy<y+BLUR_RADIUS; ++yy)
				{
					uint8* pp = (uint8*)m_back_screen->at(x-(BLUR_RADIUS*2), yy);
					for(int xx=x-BLUR_RADIUS; xx<x+BLUR_RADIUS; ++xx)
					{
		//				if((xx - x)*(xx - x)+(yy - y)*(yy - y) > BLUR_RADIUS_2)
						{
							val[0] += pp[0];
							val[1] += pp[1];
							val[2] += pp[2];

							++num;
						}

						pp += 3;
					}
				}

				uint8* p = (uint8*)new_bitmap.at(x, y);
				p[0] = val[0] / num;
				p[1] = val[1] / num;
				p[2] = val[2] / num;
			}
		}

		dc.draw_bitmap(new_bitmap);
	}

	void frame_resized(coord w, coord h)
	{
		delete m_back_screen;
		m_back_screen = new ntk::Bitmap(w, h, ntk::Bitmap::RGB_24);
	}

	void key_down(uint code, uint repeat)
	{
		if(code == VK_SPACE)
			m_blur = !m_blur;
		else
			View::key_down(code, repeat);
	}

	void attached_to_window()
	{
		make_focus();

		ntk::Rect parent_rect = parent()->bounds();
		if(window() && window()->panel() && window()->panel()->key_menu_bar())
			parent_rect.top += window()->panel()->key_menu_bar()->frame().height();

		resize_to(parent_rect.width(), parent_rect.height());
	}

private:
	ntk::Bitmap* m_back_screen;
	bool m_blur;
};

//########################################################

PreviewPanel::PreviewPanel()
:	ntk::Panel(ntk::Rect(0, 0, 10, 10), "PreviewPanel", FOLLOW_ALL_SIDES, COVER_WINDOW)
{
	add_child(m_preview_control = new GDIPreview());
}
