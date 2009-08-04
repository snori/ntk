#ifndef __POINTBRUSH_H__
#define __POINTBRUSH_H__


#include "brush.h"


class point_brush_t : public brush_t {
public:
	point_brush_t(ntk_bitmap* bitmap, const ntk_point& point)
	:	brush_t(bitmap, 
	,	m_point(point)
	{
	}
	virtual ~point_brush_t()
	{
	}

	virtual void do_it()
	{
		if(m_bitmap == NULL || bitmap->frame().contains(m_point) == false)
			return;

		uint8* pixel = reinterpret_cast<uint8*>(bitmap->at(m_point));
		m_prev_pixel[0] = pixel[0];
		m_prev_pixel[1] = pixel[1];
		m_prev_pixel[2] = pixel[2];// undo 用にバックアップ
		pixel[0] = 255;
		pixel[1] = 255;
		pixel[2] = 255;// ピクセルを塗る
	}
	virtual void undo()
	{
		uint8* pixel = reinterpret_cast<uint8*>(bitmap->at(m_point));
	}

private:
	ntk_point m_point;

};// class point_brush_t


#endif//EOH