/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: interfacedefs.h,v 1.9 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_INTERFACEDEFS_H__
#define __NTK_INTERFACE_INTERFACEDEFS_H__


#include <assert.h>

#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_DEFS_H__
#include <ntk/application/defs.h>
#endif


namespace ntk {


#pragma pack(push, 1)
class RGBColor {
public:
	//
	// types
	//
	typedef uint8 Value;

	//
	// constants
	//
	enum {
		RED		= 0,
		GREEN	= 2,
		BLUE	= 1,
		ALPHA	= 4,
	};//position

	enum {
		RED_CHANNEL   = 0,
		GREEN_CHANNEL = 2,
		BLUE_CHANNEL  = 1,
		DUMMY_CHANNEL = 3,
		ALPHA_CHANNEL = 4,

		CHANNEL_MAX = 5
	};//channel index

	//
	// data
	//
	Value red, green, blue, dummy, alpha;// dummy ÇÕ COLORREF Ç∆ÇÃå›ä∑ê´ÇÃÇΩÇﬂÅB

	//
	// macros
	//
	static inline Value saturate(int val)
	{
		return val < 0 ? 0 : (val > 255 ? 255 : static_cast<Value>(val));
	}

	//
	// methods
	//
	RGBColor()
	{}
	RGBColor(Value r, Value g, Value b)
	:	red(r), green(g), blue(b), dummy(0), alpha(255)
	{}
	RGBColor(Value r, Value g, Value b, Value alpha)
	:	red(r), green(g), blue(b), dummy(0), alpha(alpha)
	{}
	RGBColor(COLORREF colorref)
	{
		*((COLORREF*)this) = colorref;
		alpha = 255;
	}
	RGBColor(const RGBColor& rhs)
	:	red(rhs.red), green(rhs.green), blue(rhs.blue), dummy(rhs.dummy), alpha(rhs.alpha)
	{}
	RGBColor& operator=(const RGBColor& rhs)
	{
		red		= rhs.red;
		green	= rhs.green;
		blue	= rhs.blue;
		dummy	= rhs.dummy;
		alpha	= rhs.alpha;
		return *this;
	}
	RGBColor& operator=(COLORREF rhs)
	{
		*((COLORREF*)this) = rhs;
		alpha = 255;
		return *this;
	}

	//
	// accessors and manipulators
	//
	COLORREF* pref() {return reinterpret_cast<COLORREF*>(this);}
	const COLORREF* pref() const {return reinterpret_cast<const COLORREF*>(this);}

	COLORREF& ref() {return *pref();}
	const COLORREF& ref() const {return *pref();}

	void reset(Value r, Value g, Value b)
	{
		red		= r;
		green	= g;
		blue	= b;
	}
	void reset(Value r, Value g, Value b, Value a)
	{
		red		= r;
		green	= g;
		blue	= b;
		alpha	= a;
	}

	//
	// operators
	//
	Value& operator[](int index)
	{
		assert(0 <= index && index < CHANNEL_MAX);

		return index == DUMMY_CHANNEL
			? ((Value*)this)[ALPHA_CHANNEL]
			: ((Value*)this)[index];
	}
	const Value& operator[](int index) const
	{
		assert(0 <= index && index < CHANNEL_MAX);

		return index == DUMMY_CHANNEL
			? ((Value*)this)[ALPHA_CHANNEL]
			: ((Value*)this)[index];
	}

	operator COLORREF() const {return ref();}

	operator COLORREF*() {return pref();}
	operator const COLORREF*() const {return pref();}

	operator COLORREF&() {return ref();}
	operator const COLORREF&() const {return ref();}

	// RGBColor +/- RGBColor
	friend const RGBColor& operator+=(RGBColor& lhs, const RGBColor& rhs)
	{
		lhs.red		= saturate(lhs.red   + rhs.red);
		lhs.green	= saturate(lhs.green + rhs.green);
		lhs.blue	= saturate(lhs.blue  + rhs.blue);
		return lhs;
	}
	friend const RGBColor operator+(RGBColor lhs, const RGBColor& rhs)
	{
		return lhs += rhs;
	}

	friend const RGBColor& operator-=(RGBColor& lhs, const RGBColor& rhs)
	{
		lhs.red		= saturate(lhs.red   - rhs.red);
		lhs.green	= saturate(lhs.green - rhs.green);
		lhs.blue	= saturate(lhs.blue  - rhs.blue);
		return lhs;
	}
	friend const RGBColor operator-(RGBColor lhs, const RGBColor& rhs)
	{
		return lhs -= rhs;
	}

	// NRGBColor +/- int
	friend const RGBColor& operator+=(RGBColor& lhs, int rhs)
	{
		lhs.red		= saturate(lhs.red   + rhs);
		lhs.green	= saturate(lhs.green + rhs);
		lhs.blue	= saturate(lhs.blue  + rhs);
		return lhs;
	}
	friend const RGBColor operator+(RGBColor lhs, int rhs)
	{
		return lhs += rhs;
	}

	friend const RGBColor& operator-=(RGBColor& lhs, int rhs)
	{
		lhs.red		= saturate(lhs.red   - rhs);
		lhs.green	= saturate(lhs.green - rhs);
		lhs.blue	= saturate(lhs.blue  - rhs);
		return lhs;
	}
	friend const RGBColor operator-(RGBColor lhs, int rhs)
	{
		return lhs -= rhs;
	}

	// RGBColor */'/' int
	friend const RGBColor& operator*=(RGBColor& lhs, int rhs)
	{
		lhs.red		= saturate(lhs.red   * rhs);
		lhs.green	= saturate(lhs.green * rhs);
		lhs.blue	= saturate(lhs.blue  * rhs);
		return lhs;
	}
	friend const RGBColor operator*(RGBColor lhs, int rhs)
	{
		return lhs *= rhs;
	}

	friend const RGBColor& operator/=(RGBColor& lhs, int rhs)
	{
		assert(rhs != 0);

		lhs.red		= saturate(lhs.red   / rhs);
		lhs.green	= saturate(lhs.green / rhs);
		lhs.blue	= saturate(lhs.blue  / rhs);
		return lhs;
	}
	friend const RGBColor operator/(RGBColor lhs, int rhs)
	{
		return lhs /= rhs;
	}

	#if 0
		friend const RGBColor operator-(RGBColor col)
		{
			col.red		= -col.red;
			col.green	= -col.green;
			col.blue	= -col.blue;

			return col;
		}
	#endif

};// class RGBColor
#pragma pack(pop)


class DC;
class Point;
class Rect;
class GDIPen;

class Wnd;
class View;
class Window;


//--------------------------------------------------------
// interface messages

enum {
	INVALIDATE        = '_IIV',// Interface:InValidate
	ZOOM              = '_IZM',// Interface:ZooM
	MINIMIZE          = '_IMN',// Interface:MiNimize
	WINDOW_ACTIVATED  = '_IWA',// Interface:Window Activated
	FONT_CHANGED      = '_IFC',// Interface:Font Changed

	// internal messages //
	//__DELETE_THIS__   = '_IDT',//Interface:Delete This
};


//--------------------------------------------------------
// ntk control identifier

enum {
	SYSTEM_CONTROL = 0xffff
};


//--------------------------------------------------------
// keybord and mouse

enum {
	LEFT_MOUSE_BUTTON		= MK_LBUTTON,
	MIDDLE_MOUSE_BUTTON	= MK_MBUTTON,
	RIGHT_MOUSE_BUTTON	= MK_RBUTTON,

	SHIFT_KEY					= MK_SHIFT,
	CONTROL_KEY				= MK_CONTROL,
	ALT_KEY						= (1<<8),

	LEFT_SHIFT_KEY		= (1<<6),
	LEFT_CONTROL_KEY	= (1<<7),
	LEFT_ALT_KEY			= (1<<8),

	RIGHT_SHIFT_KEY		= (1<<9),
	RIGHT_CONTROL_KEY	= (1<<10),
	RIGHT_ALT_KEY			= (1<<11),

	CAPS_LOCK					= (1<<12),
	SCROLL_LOCK				= (1<<13),
	NUM_LOCK					= (1<<14),
};//modifier keys


NtkExport uint modifiers();
NtkExport void get_mouse(Point* point, uint* buttons);
//NtkExport void set_mouse_position(const Point& point);


//--------------------------------------------------------
// system colors

enum SystemColorID {
	// basic colors
	WHITE_COLOR,
	BLACK_COLOR,
	GRAY_COLOR,
	RED_COLOR,
	GREEN_COLOR,
	BLUE_COLOR,
	YELLOW_COLOR,
	TRANSPARENT_COLOR,

	// system colors
	APP_COLOR,
	FONT_COLOR,

	// control colors
	VIEW_COLOR,
	OUTLINE_COLOR,
	WINDOW_FRAME_COLOR,
	ACTIVE_WINDOW_TITLE_BAR_COLOR,
	ACTIVE_WINDOW_TITLE_BAR_FONT_COLOR,
	INACTIVE_WINDOW_TITLE_BAR_COLOR,
	INACTIVE_WINDOW_TITLE_BAR_FONT_COLOR,

	CONTAINER_BACKGROUND_COLOR,

	// num of colors
	NUM_COLORS
};//SystemColorID

NtkExport const RGBColor& system_color(SystemColorID color_id);
NtkExport void set_system_color(SystemColorID color_id, const RGBColor& color);


//
// utility color functions
//
inline const RGBColor& white_color()       {return system_color(WHITE_COLOR);}
inline const RGBColor& black_color()       {return system_color(BLACK_COLOR);}
inline const RGBColor& gray_color()        {return system_color(GRAY_COLOR);}
inline const RGBColor& red_color()         {return system_color(RED_COLOR);}
inline const RGBColor& green_color()       {return system_color(GREEN_COLOR);}
inline const RGBColor& blue_color()        {return system_color(BLUE_COLOR);}
inline const RGBColor& yellow_color()      {return system_color(YELLOW_COLOR);}
inline const RGBColor& transparent_color() {return system_color(TRANSPARENT_COLOR);}

inline const RGBColor& app_color()         {return system_color(APP_COLOR);}
inline const RGBColor& font_color()        {return system_color(FONT_COLOR);}
inline const RGBColor& view_color()        {return system_color(VIEW_COLOR);}
inline const RGBColor& outline_color()     {return system_color(OUTLINE_COLOR);}


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef RGBColor      rgb_color_t;
	typedef SystemColorID system_color_id_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::RGBColor      NRGBColor;
	typedef ntk::SystemColorID NSystemColorID;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::RGBColor      ntk_rgb_color;
	typedef ntk::SystemColorID ntk_system_color_id;
	#endif
#endif


#endif//EOH
