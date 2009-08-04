#ifndef __BRUSH_H__
#define __BRUSH_H__


#include <ntk/defs.h>
#include <ntk/interface/point.h>
#include <ntk/interface/bitmap.h>


class Brush {
public:
	Brush();
	virtual ~Brush();

	virtual void paint(NBitmap* bitmap, const NPoint& point) =0;

};// class Brush


class FlatBrush : public Brush {
public:
	FlatBrush(coord size = 1, const NRGBColor& color = NRGBColor(0, 0, 0));

	void paint(NBitmap* bitmap, const NPoint& point);

private:
	coord m_size;
	NRGBColor m_color;

};// class FlatBrush


class CloneBrush : public Brush {
public:
	CloneBrush(coord size = 1);
	void paint(NBitmap* bitmap, const NPoint& point);

private:
	coord m_size;

};// class CloneBrush


#endif//EOH