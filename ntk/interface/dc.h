/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dc.h,v 1.9 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_DC_H__
#define __NTK_INTERFACE_DC_H__


#include <boost/utility.hpp>

#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_REFCOUNT_H__
#include <ntk/support/refcount.h>
#endif

#ifndef __NTK_INTERFACE_POINT_H__
#include <ntk/interface/point.h>
#endif

#ifndef __NTK_INTERFACE_RECT_H__
#include <ntk/interface/rect.h>
#endif

#ifndef __NTK_INTERFACE_GDIOBJECT_H__
#include <ntk/interface/gdiobject.h>
#endif


namespace ntk {


class Bitmap;
class DCBuffer;
class PaintDCBuffer;


class DC {
public:
	//
	// constants
	//
	enum DrawingMode {
		OP_COPY = SRCCOPY,
	#if 0
		OP_OVER,
		OP_ERASE,
	#endif
		OP_INVERT = SRCINVERT,
	#if 0
		OP_SELECT,
		OP_MIN,
		OP_MAX,
		OP_ADD,
		OP_SUBTRACT,
		OP_BLEND
	#endif
	};

	enum DestructionMethod {
		RELEASE_DC,
		DELETE_DC,
		DO_NOTHING
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef DrawingMode       drawint_mode_t;
		typedef DestructionMethod destruction_method_t;
	#endif

	//
	// methods
	//
	NtkExport DC(HWND hwnd, DestructionMethod method = RELEASE_DC);
	NtkExport DC(HDC hdc, DestructionMethod method = DO_NOTHING);
	NtkExport virtual ~DC();

	//
	// state manipulator
	//
	NtkExport virtual void push_state();
	NtkExport virtual bool pop_state();

	NtkExport virtual const Font& font() const;
	NtkExport virtual Font set_font(const Font& font);
	NtkExport virtual void push_font();
	NtkExport virtual void push_font(const Font& font);
	NtkExport virtual bool pop_font();

	NtkExport virtual const Pen& pen() const;
	NtkExport virtual Pen set_pen(const Pen& pen);
	NtkExport virtual void push_pen();
	NtkExport virtual void push_pen(const Pen& pen);
	NtkExport virtual bool pop_pen();

	NtkExport virtual const Brush& brush() const;
	NtkExport virtual Brush set_brush(const Brush& brush);
	NtkExport virtual void push_brush();
	NtkExport virtual void push_brush(const Brush& brush);
	NtkExport virtual bool pop_brush();

	NtkExport virtual const DDB& bitmap() const;
	NtkExport virtual DDB set_bitmap(const DDB& bitmap);
	NtkExport virtual void push_bitmap();
	NtkExport virtual void push_bitmap(const DDB& bitmap);
	NtkExport virtual bool pop_bitmap();

	NtkExport virtual DrawingMode drawing_mode() const;
	NtkExport virtual DrawingMode set_drawing_mode(DrawingMode mode);
	NtkExport virtual void push_drawing_mode();
	NtkExport virtual void push_drawing_mode(DrawingMode mode);
	NtkExport virtual bool pop_drawing_mode();

	NtkExport virtual const RGBColor& text_color() const;
	NtkExport virtual RGBColor set_text_color(const RGBColor& color);
	NtkExport virtual void push_text_color();
	NtkExport virtual void push_text_color(const RGBColor& color);
	NtkExport virtual bool pop_text_color();

	NtkExport virtual const RGBColor& back_color() const;
	NtkExport virtual RGBColor set_back_color(const RGBColor& color);
	NtkExport virtual void push_back_color();
	NtkExport virtual void push_back_color(const RGBColor& color);
	NtkExport virtual bool pop_back_color();

	NtkExport virtual int back_mode() const;
	NtkExport virtual int set_back_mode(int mode);
	NtkExport virtual void push_back_mode();
	NtkExport virtual void push_back_mode(int mode);
	NtkExport virtual bool pop_back_mode();

	NtkExport virtual uint text_align() const;
	NtkExport virtual uint set_text_align(uint text_align);
	NtkExport virtual void push_text_align();
	NtkExport virtual void push_text_align(uint text_align);
	NtkExport virtual bool pop_text_align();

	//NtkExport virtual void set_high_color(const NRGBColor& color);
	//NtkExport virtual void set_low_color();

	NtkExport virtual Region clipping_region() const;
	NtkExport virtual void constrain_clipping_region(const Region& region);
	NtkExport virtual void clear_clipping_region();

	//
	// drawing methods
	//
	NtkExport void draw_string(const String& str);
	NtkExport void draw_string(const String& str, uint length);
	NtkExport void draw_string(const String& str, const Point& point);
	NtkExport void draw_string(const String& str, uint length, const Point& point);

	NtkExport Point move_to(coord x, coord y);
	Point move_to(const Point& point)
	{
		return move_to(point.x, point.y);
	}

	void stroke_line(coord end_x, coord end_y) {line_to(end_x, end_y);}
	void stroke_line(const Point& end_point) {line_to(end_point);}

	NtkExport void stroke_line(coord x1, coord y1, coord x2, coord y2);
	void stroke_line(const Point& p1, const Point& p2) {stroke_line(p1.x, p1.y, p2.x, p2.y);}

	NtkExport void stroke_rect(coord l, coord t, coord r, coord b);
	NtkExport void stroke_round_rect(coord l, coord t, coord r, coord b, coord width, coord height);
	NtkExport void fill_rect(coord l, coord t, coord r, coord b);
	NtkExport void fill_round_rect(coord l, coord t, coord r, coord b, coord width, coord height);
	void stroke_rect(const Rect& rect) {stroke_rect(rect.left, rect.top, rect.right, rect.bottom);}
	void stroke_round_rect(const Rect& rect, coord width, coord height) {stroke_round_rect(rect.left, rect.top, rect.right, rect.bottom, width, height);}
	void fill_rect(const Rect& rect) {fill_rect(rect.left, rect.top, rect.right, rect.bottom);}
	void fill_round_rect(const Rect& rect, coord width, coord height) {fill_round_rect(rect.left, rect.top, rect.right, rect.bottom, width, height);}
	void stroke_rect(const Point& p1, const Point& p2) {stroke_rect(p1.x, p1.y, p2.x, p2.y);}
	void stroke_round_rect(const Point& p1, const Point& p2, coord width, coord height) {stroke_round_rect(p1.x, p1.y, p2.x, p2.y, width, height);}
	void fill_rect(const Point& p1, const Point& p2) {fill_rect(p1.x, p1.y, p2.x, p2.y);}
	void fill_round_rect(const Point& p1, const Point& p2, coord width, coord height) {fill_round_rect(p1.x, p1.y, p2.x, p2.y, width, height);}

	NtkExport void stroke_ellipse(const Point& center, coord horizontal_radius, coord vertical_radius);
	NtkExport void stroke_ellipse(coord left, coord top, coord right, coord bottom);
	NtkExport void fill_ellipse(const Point& center, coord horizontal_radius, coord vertical_radius);
	NtkExport void fill_ellipse(coord left, coord top, coord right, coord bottom);
	void stroke_ellipse(const Rect& rect) {stroke_ellipse(rect.left, rect.top, rect.right, rect.bottom);}
	void fill_ellipse(const Rect& rect) {fill_ellipse(rect.left, rect.top, rect.right, rect.bottom);}

	NtkExport void stroke_polygon(const Point* point_list, uint num_of_points);
	NtkExport void fill_polygon(const Point* point_list, uint num_of_points);

	NtkExport void draw_bitmap(const Bitmap& bitmap);
	NtkExport void draw_bitmap(const Bitmap& bitmap, const Point& point);
	NtkExport void draw_bitmap(const Bitmap& bitmap, const Rect& rect);
	NtkExport void draw_bitmap(const Bitmap& bitmap, const Rect& dest, const Rect& src);

	//
	// win32 compatible drawing methods
	//
	NtkExport void text_out(coord x, coord y, const String& text);
	void text_out(const Point& point, const String& text)
	{
		text_out(point.x, point.y, text);
	}

	NtkExport void draw_text(
		const Rect& rect,
		const String& text,
		uint format = DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_TABSTOP | 0x40);// TAB WIDTH 4
	NtkExport Rect calculate_rect(const String& text);

	NtkExport void line_to(coord end_x, coord end_y);
	void line_to(const Point& end_point)
	{
		line_to(end_point.x, end_point.y);
	}

	NtkExport void bit_blt(
		coord dest_x,
		coord dest_y,
		coord dest_width,
		coord dest_height,
		DC& src_dc,
		coord src_x,
		coord src_y,
		uint op_code = OP_COPY);
	NtkExport void bit_blt(
		const Rect& dest_rect,
		DC& src_dc,
		const Point& src_point,
		uint op_code = OP_COPY);

	NtkExport void pat_blt(
		coord dest_x,
		coord dest_y,
		coord dest_width,
		coord dest_height,
		uint op_code = PATCOPY);
	NtkExport void pat_blt(
		const Rect& dest_rect,
		uint op_code = PATCOPY);

	//
	// accessors and manipulators
	//
	NtkExport virtual HWND hwnd() const;
	NtkExport virtual HDC hdc() const;

	NtkExport virtual Point current_point() const;

	NtkExport coord string_width(const String& str, int length = -1) const;// 'length = -1' equals to str.length()
	NtkExport coord string_height() const;

	//
	// operators
	//
	NtkExport virtual operator HDC() const;

	//
	// methods
	//
	#ifdef SWIG
		DC() {}
	#endif

protected:
	//
	// methods
	//
	NtkExport DC(DCBuffer* buf);

	DCBuffer* buffer() {return m_buffer.ref_count_buffer();}
	const DCBuffer* buffer() const {return m_buffer.ref_count_buffer();}

private:
	//
	// data
	//
	RefCounter<DCBuffer> m_buffer;

};// class DC


class PaintDC : public DC, boost::noncopyable {
public:
	//
	// methods
	//
	NtkExport PaintDC(HWND hwnd);
	NtkExport virtual ~PaintDC();

	//
	// accessors
	//
	NtkExport virtual Rect update_rect() const;
	NtkExport virtual bool erase() const;

};// class PaintDC


class MemoryDC : public DC {
public:
	//
	// methods
	//
	NtkExport MemoryDC(HDC hdc = NULL);
	NtkExport virtual ~MemoryDC();

};// class MemoryDC


class ScreenDC : public DC {
public:
	//
	// methods
	//
	NtkExport ScreenDC();
	NtkExport virtual ~ScreenDC();

};// class ScreenDC


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef DC       dc_t;
	typedef PaintDC  paint_dc_t;
	typedef MemoryDC memory_dc_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::DC       NDC;
	typedef ntk::PaintDC  NPaintDC;
	typedef ntk::MemoryDC NMemoryDC;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::DC       ntk_dc;
	typedef ntk::PaintDC  ntk_paint_dc;
	typedef ntk::MemoryDC ntk_memory_dc;
	#endif
#endif


#endif//EOH
