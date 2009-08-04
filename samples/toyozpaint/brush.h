#ifndef __BRUSH_H__
#define __BRUSH_H__


#include <ntk/interface/point.h>
#include <ntk/interface/bitmap.h>


class brush_t {
public:
	brush_t(ntk_bitmap* bitmap, const ntk_point& point)
	:	m_bitmap(bitmap)
	,	m_point(point)
	{}
	virtual ~brush_t() {}

	virtual void exec() = 0;
	virtual bool undo() {return false;}
	virtual bool redo() {exec(); return true;}

	ntk_bitmap* bitmap() {return m_bitmap;}
	const ntk_bitmap* bitmap() const {return m_bitmap;}
	const ntk_point& point() const {return m_point;}

private:
	ntk_bitmap* m_bitmap;
	ntk_point m_point;

};// class brush_t


#endif//EOH