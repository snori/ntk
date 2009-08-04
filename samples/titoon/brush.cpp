#include "brush.h"


#include "titoon.h"
#include "titoonwindow.h"
#include "animation.h"
#include "image.h"
#include "timecontrol.h"


//########################################################


Brush::Brush()
{
}

Brush::~Brush()
{
}


//########################################################


FlatBrush::FlatBrush(coord size, const NRGBColor& color)
:	m_size(size)
,	m_color(color)
{
}

void
FlatBrush::paint(NBitmap* bitmap, const NPoint& point)
{
	coord x1 = point.x - m_size / 2;
	coord y1 = point.y - m_size / 2;
	coord x2 = x1 + m_size;
	coord y2 = y1 + m_size;

	coord width = bitmap->width(), height = bitmap->height();

	for(int y=y1; y<y2; ++y)
	for(int x=x1; x<x2; ++x)
	{
		uint8* p = (uint8*)bitmap->at(x, y);

		if(x >= 0 && y >= 0 && x < width && y < height)
		{
			p[0] = m_color.red;
			p[1] = m_color.green;
			p[2] = m_color.blue;
		}
	}
}


//########################################################


CloneBrush::CloneBrush(coord size)
:	m_size(size)
{
}

void
CloneBrush::paint(NBitmap* bitmap, const NPoint& point)
{
	Animation* anim = titoon()->window()->animation();
	if(anim == NULL)
		return;

	Image* image = anim->image_at(titoon()->window()->time_control()->current_frame() -1);
	if(image == NULL)
		return;

	coord x1 = point.x - m_size / 2;
	coord y1 = point.y - m_size / 2;
	coord x2 = x1 + m_size;
	coord y2 = y1 + m_size;

	coord width = bitmap->width(), height = bitmap->height();

	for(int y=y1; y<y2; ++y)
	for(int x=x1; x<x2; ++x)
	{
		uint8* p = (uint8*)bitmap->at(x, y);
		uint8* src = (uint8*)image->bitmap().at(x, y);

		if(x >= 0 && y >= 0 && x < width && y < height)
		{
			p[0] = src[0];
			p[1] = src[1];
			p[2] = src[2];
		}
	}
}


//########################################################
