#ifndef __BRUSHSTROKE_H__
#define __BRUSHSTROKE_H__


#include <ntk/defs.h>
#include <ntk/interface/point.h>


class Canvas;
class Brush;


class BrushStroke {
public:
	BrushStroke();
	virtual ~BrushStroke() =0;

	bool painting() const {return m_painting;}
	void set_painting(bool flag);

	virtual void mouse_down(const NPoint& point, uint buttons, bool double_click);
	virtual void mouse_up(const NPoint& point, uint buttons);
	virtual void mouse_moved(const NPoint& point, uint buttons);

protected:
	void paint(const NPoint& point);
	virtual void begin_painting();
	virtual void end_painting();

private:
	bool m_painting, m_mouse_down_outside;

	Canvas* m_canvas;
	Brush* m_brush;

};// class BrushStroke


class DiscreteBrushStroke : public BrushStroke {
public:
	virtual void mouse_down(const NPoint& point, uint buttons, bool double_click);
	virtual void mouse_moved(const NPoint& point, uint buttons);

};// class DiscreteBrushStroke


class SequentialBrushStroke : public BrushStroke {
public:
	virtual void mouse_down(const NPoint& point, uint buttons, bool double_click);
	virtual void mouse_moved(const NPoint& point, uint buttons);

private:
	NPoint m_prev_point;

};// class SequentialBrushStroke


#endif//EOH