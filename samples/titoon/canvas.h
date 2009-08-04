#ifndef __CANVAS_H__
#define __CANVAS_H__


#include <deque>
#include <boost/shared_ptr.hpp>
#include <ntk/interface/view.h>
#include <ntk/interface/bitmap.h>
#include "image.h"


class Image;
class Brush;
class BrushStroke;


class Canvas : public NView {
public:
	enum {
		DEFAULT_FLAGS = NView::DEFAULT_FLAGS | WILL_DRAW,
	};

	Canvas(
		const NRect& frame,
		const NRGBColor& color = ntk::app_color() + 25);

	void undo();
	void redo();
	void store_undo_data();

	Image* image() const {return m_image;}
	Image* prev_image() const {return m_prev_image;}
	Image* next_image() const {return m_next_image;}
	void set_image(Image* image, Image* prev_image = NULL, Image* next_image = NULL);

	float zoom() const {return m_zoom;}
	void set_zoom(float zoom);

	const NRGBColor& view_color() const {return m_view_color;}
	void set_view_color(const NRGBColor& color) {m_view_color = color; invalidate();}

	virtual void mouse_down(const NPoint& point, uint buttons, bool dbl);
	virtual void mouse_up(const NPoint& point, uint buttons);
	virtual void mouse_moved(const NPoint& point, uint buttons, uint transit, const NMessage* data);
	virtual void lost_capture();
	virtual void draw(NPaintDC& dc);
	virtual void attached_to_window();

private:
	class UndoBuffer {
	public:
		void store_undo_data(int frame, const Image& image);
		bool undo(int* frame, Image* image);
		bool redo(int* frame, Image* image);

	private:
		struct UndoData {
			int frame;
			Image image;

			UndoData(int frm, const Image& img) : frame(frm), image(img) {}
		};
		typedef boost::shared_ptr<UndoData> UndoDataPtr;
		typedef std::deque<UndoDataPtr> UndoStack;

		UndoStack m_undo_stack, m_redo_stack;
	};

	Image *m_image, *m_prev_image, *m_next_image;
	bool m_painting;
	float m_zoom;
	NRGBColor m_view_color;
	UndoBuffer m_undo_buffer;

	Brush* m_bursh;
	BrushStroke* m_brush_stroke;

	void paint_(const NPoint& point);
	NPoint convert_point_(const NPoint& point);

};// class Canvas


#endif//EOH