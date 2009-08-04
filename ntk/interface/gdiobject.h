/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: gdiobject.h,v 1.11 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_GDIOBJECT_H__
#define __NTK_INTERFACE_GDIOBJECT_H__


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

#ifndef __NTK_INTERFACE_DEFS_H__
#include <ntk/interface/defs.h>
#endif

#ifndef __NTK_SUPPORT_UTILITY_H__
#include <ntk/support/utility.h>
#endif


namespace ntk {


class DC;


template<typename T>
class GDIObject {
public:
	//
	// types
	//
	typedef T Handle;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Handle handle_t;
	#endif

	//
	// methods
	//
	GDIObject()
	:	m_buffer(NULL)
	{}
	GDIObject(Handle obj, bool own = true)
	:	m_buffer(new Buffer(obj, own))
	{}
	virtual ~GDIObject() {}

	virtual bool is_valid() const {return handle() != NULL;}

	//
	// accessors
	//
	virtual Handle handle() const {return m_buffer->handle;}
	virtual bool is_owned() const {return m_buffer->own;}

	//
	// operators
	//
	friend bool operator==(const GDIObject& lhs, const GDIObject& rhs)
	{
		return lhs.m_buffer->handle == rhs.m_buffer->handle;
	}
	friend bool operator!=(const GDIObject& lhs, const GDIObject& rhs)
	{
		return !operator==(lhs, rhs);
	}

protected:
	//
	// types
	//
	struct Buffer : public RefCountBuffer {
		Handle handle;
		bool own;

		Buffer(Handle handle_, bool own_)
		: handle(handle_), own(own_)
		{}
		virtual ~Buffer()
		{
			if(handle && own)
				DeleteObject(handle);
		}

	};// class Buffer

	//
	// accessors and manipulators
	//
	Buffer* buffer() {return m_buffer.ref_count_buffer();}
	const Buffer* buffer() const {return m_buffer.ref_count_buffer();}

	void set_handle(Handle h, bool own = true)
	{
		m_buffer.set_ref_count_buffer(new Buffer(h, own));
	}

private:
	//
	// data
	//
	RefCounter<Buffer> m_buffer;

};// class GDIObject


class Font : public GDIObject<HFONT> {
public:
	//
	// constants
	//
	enum {
		NORMAL     = 0,
		BOLD       = NTK_BIT(0),
		ITALIC     = NTK_BIT(1),
		UNDER_LINE = NTK_BIT(2),
		STRIKE_OUT = NTK_BIT(3),
	};

	//
	// methods
	//
	NtkExport Font();
	NtkExport Font(const String& face, coord size, uint flags = NORMAL);
	NtkExport Font(const LOGFONT& logfont);
	NtkExport Font(HFONT font, bool owner);
	NtkExport Font(const Font& val);
	NtkExport virtual ~Font();
	NtkExport Font& operator=(const Font& rhs);

	NtkExport coord string_width(const DC& dc, const String& str) const;
	NtkExport coord string_height(const DC& dc) const;

	//
	// accessors
	//
	NtkExport virtual String face() const;
	NtkExport virtual void set_face(const String& face);

	NtkExport virtual coord size() const;
	NtkExport virtual void set_size(coord size);

	NtkExport virtual uint flags() const;
	NtkExport virtual void set_flags(uint flags);

	NtkExport virtual const LOGFONT& data() const;

	//
	// functions
	//
	NtkExport static int size2height(coord size);
	NtkExport static coord height2size(int height);

private:
	//
	// data
	//
	LOGFONT m_attr;
	coord m_size;

	//
	// functions
	//
	void set_up_flags(uint flags);

};// class Font


class Pen : public GDIObject<HPEN> {
public:
	//
	// methods
	//
	NtkExport Pen();
	NtkExport Pen(const RGBColor& color, coord width = 0, uint style = PS_SOLID);
	NtkExport Pen(const LOGPEN& logpen);
	NtkExport Pen(HPEN pen, bool own = true);
	NtkExport Pen(const Pen& val);
	NtkExport virtual ~Pen();
	NtkExport Pen& operator=(const Pen& rhs);

	NtkExport virtual RGBColor color() const;
	NtkExport virtual void set_color(const RGBColor& color);

	NtkExport virtual coord width() const;
	NtkExport virtual void set_width(coord width);

	NtkExport virtual uint style() const;
	NtkExport virtual void set_style(uint style);

	NtkExport virtual const LOGPEN& data() const;

private:
	//
	// data
	//
	LOGPEN m_attr;

};// class Pen


class Brush : public GDIObject<HBRUSH> {
public:
	//
	// methods
	//
	NtkExport Brush();
	NtkExport Brush(const RGBColor& color, uint style = BS_SOLID, long hatch_style = 0);
	NtkExport Brush(const LOGBRUSH& logbrush);
	NtkExport Brush(HBRUSH hbrush, bool own = true);
	NtkExport Brush(const Brush& val);
	NtkExport virtual ~Brush();
	NtkExport Brush& operator=(const Brush& rhs);

	NtkExport virtual RGBColor color() const;
	NtkExport virtual void set_color(const RGBColor& color);

	NtkExport virtual uint style() const;
	NtkExport virtual void set_style(uint style);

	NtkExport virtual long hatch_style() const;
	NtkExport virtual void set_hatch_style(long hatch_style);

	NtkExport virtual const LOGBRUSH& data() const;

private:
	//
	// data
	//
	LOGBRUSH m_attr;

};// class Brush


class DDB : public GDIObject<HBITMAP> {
public:
	//
	// methods
	//
	NtkExport DDB(HDC hdc, coord width, coord height);// create compatible bitmap
	NtkExport DDB(
		coord width, coord height,
		uint num_planes, uint bits_per_pixel, const void* bits);// create monochrome bitmap
	NtkExport DDB(HBITMAP hbitmap, bool own = true);
	NtkExport DDB(const DDB& val);
	NtkExport virtual ~DDB();
	NtkExport DDB& operator=(const DDB& rhs);

	NtkExport virtual HDC hdc() const;
	NtkExport virtual coord width() const;
	NtkExport virtual coord height() const;
	NtkExport virtual uint num_planes() const;
	NtkExport virtual uint bits_per_pixel() const;

private:
	//
	// data
	//
	HDC m_hdc;
	coord m_width, m_height;
	uint m_num_planes, m_bits_per_pixel;

};// class DDB (Device Dependent Bitmap)


typedef DDB GDIBitmap;


class Region : public GDIObject<HRGN> {
public:
	//
	// constants
	//
	enum Shape {
		RECTANGLE,
		ELLIPTIC,
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Shape shape_t;
	#endif

	//
	// methods
	//
	NtkExport Region();// Region(0, 0, 1, 1) ‚Æ“¯‹`

	// rect region
	NtkExport Region(const Rect& rect, Shape shape = RECTANGLE);
	NtkExport Region(coord l, coord t, coord r, coord b, Shape shape = RECTANGLE);

	// round rect region
	NtkExport Region(const Rect& rect, coord horizontal, coord vertical);
	NtkExport Region(coord l, coord t, coord r, coord b, coord horizontal, coord vertical);

	// polygon region
	NtkExport Region(const Point* point_list, uint num_of_points, uint polygon_filling_mode = 0);

	NtkExport Region(HRGN region, bool own = true);
	NtkExport Region(const Region& val);
	NtkExport virtual Region& operator=(const Region& rhs);
	NtkExport virtual ~Region();

	NtkExport void offset_by(coord horizontal, coord vertical);
	void offset_by(const Point& point)
	{
		offset_by(point.x, point.y);
	}

	NtkExport void include(const Region& region);
	NtkExport void include(const Rect& rect);

	NtkExport void exclude(const Region& region);
	NtkExport void exclude(const Rect& rect);

	NtkExport void intersect(const Region& region);
	NtkExport void intersect(const Rect& rect);

	NtkExport bool contains(coord x, coord y) const;
	bool contains(const Point& point) const
	{
		return contains(point.x, point.y);
	}

	NtkExport bool contains(const Rect& rect) const;
	bool contains(coord l, coord t, coord r, coord b) const
	{
		return contains(Rect(l, t, r, b));
	}

	NtkExport bool intersects(const Region& region);

	//
	// accessors and manipurators
	//
//	uint data() const;

	NtkExport void reset(const Rect& rect);
	void reset(coord l, coord t, coord r, coord b)
	{
		reset(l, t, r, b);
	}

	//
	// operators
	//
	NtkExport friend bool operator==(const Region& lhs, const Region& rhs);
	friend bool operator!=(const Region& lhs, const Region& rhs) {return !operator==(lhs, rhs);}

	// RGN_AND
	NtkExport friend const Region& operator&=(Region& lhs, const Region& rhs);
	friend const Region operator&(Region lhs, const Region& rhs)
	{
		return lhs &= rhs;
	}

	// RGN_DIFF
	NtkExport friend const Region& operator-=(Region& lhs, const Region& rhs);
	friend const Region operator-(Region lhs, const Region& rhs)
	{
		return lhs -= rhs;
	}

	// RGN_OR
	NtkExport friend const Region& operator|=(Region& lhs, const Region& rhs);
	friend const Region operator|(Region lhs, const Region& rhs)
	{
		return lhs |= rhs;
	}

	// RGN_XOR
	NtkExport friend const Region& operator^=(Region& lhs, const Region& rhs);
	friend const Region operator^(Region lhs, const Region& rhs)
	{
		return lhs ^= rhs;
	}

};// c;ass Region


//--------------------------------------------------------
// system fonts

enum SystemFontID {
	PLAIN_FONT,
	BOLD_FONT,
	FIXED_FONT,

	NUM_OF_SYSTEM_FONTS
};

NtkExport const Font& system_font(SystemFontID font_id);


// font utility functions //
inline const Font& plain_font() {return system_font(PLAIN_FONT);}
inline const Font& bold_font() {return system_font(BOLD_FONT);}
inline const Font& fixed_font() {return system_font(FIXED_FONT);}


//--------------------------------------------------------
// system pens

enum SystemPenID {
	TRANSPARENT_PEN,

	// system pens
	OUTLINE_PEN,

	NUM_OF_SYSTEM_PENS
};

NtkExport const Pen& system_pen(SystemPenID pen_id);


// pen utility functions //
inline const Pen& transparent_pen() {return system_pen(TRANSPARENT_PEN);}
inline const Pen& outline_pen() {return system_pen(OUTLINE_PEN);}


//--------------------------------------------------------
// system brushes

enum SystemBrushID {
	TRANSPARENT_BRUSH,

	// system brushes
	BACKGROUND_BRUSH,

	NUM_OF_SYSTEM_BRUSHES
};

NtkExport const Brush& system_brush(SystemBrushID brush_id);


// brush utility functions //
inline const Brush& transparent_brush() {return system_brush(TRANSPARENT_BRUSH);}
inline const Brush& background_brush() {return system_brush(BACKGROUND_BRUSH);}


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	#define gdi_object_t  GDIObject;
	typedef Font          font_t;
	typedef Pen           pen_t;
	typedef Brush         brush_t;
	typedef DDB           ddb_t;
	typedef GDIBitmap     gdi_bitmap_t;
	typedef Region        region_t;
	typedef SystemFontID  system_font_id_t;
	typedef SystemPenID   system_pen_id_t;
	typedef SystemBrushID system_brush_id_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	#define NGDIObject         ntk::GDIObject
	typedef ntk::Font          NFont;
	typedef ntk::Pen           NPen;
	typedef ntk::Brush         NBrush;
	typedef ntk::DDB           NDDB;
	typedef ntk::GDIBitmap     NGDIBitmap;
	typedef ntk::Region        NRegion;
	typedef ntk::SystemFontID  NSystemFontID;
	typedef ntk::SystemPenID   NSystemPenID;
	typedef ntk::SystemBrushID NSystemBrushID;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	#define ntk_gdi_object     ntk::GDIObject
	typedef ntk::Font          ntk_font;
	typedef ntk::Pen           ntk_pen;
	typedef ntk::Brush         ntk_brush;
	typedef ntk::DDB           ntk_ddb;
	typedef ntk::GDIBitmap     ntk_gid_bitmap;
	typedef ntk::Region        ntk_region;
	typedef ntk::SystemFontID  ntk_system_font_id;
	typedef ntk::SystemPenID   ntk_system_pen_id;
	typedef ntk::SystemBrushID ntk_system_brush_id;
	#endif
#endif


#endif//EOH
