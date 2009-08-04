#ifndef __PAINTBOX_H__
#define __PAINTBOX_H__


#include <ntk/interface/view.h>


class Brush;
class BrushStroke;


class PaintBox : public NView {
public:
	PaintBox(const NRect& frame, const NString& name);
	virtual ~PaintBox();

	Brush* brush() const {return m_brush;}
	void set_brush(Brush* brush);

	BrushStroke* brush_stroke() const {return m_brush_stroke;}
	void set_brush_stroke(BrushStroke* brush_stroke);

	virtual void message_received(const NMessage& message);

private:
	Brush* m_brush;
	BrushStroke* m_brush_stroke;

	void set_up_controls_();

};// class PaintBox


#endif//EOH