/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: gdiobject.cpp,v 1.8 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/gdiobject.h"


#include <assert.h>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <ntk/interface/dc.h>
#include <ntk/support/utility.h>


namespace ntk {


//########################################################

class TemporaryScreenDC {
public:
	TemporaryScreenDC()
	:	dc(CreateDC("DISPLAY", NULL, NULL, NULL))
	{
	}
	~TemporaryScreenDC()
	{
		DeleteDC(dc);
	}

	operator HDC() const
	{
		return dc;
	}

private:
	HDC dc;
};

//########################################################
// public methods

Font::Font()
:	m_size(0)
{
	memset(&m_attr, 0, sizeof(m_attr));
}

Font::Font(const String& face, coord size, uint flags)
:	m_size(size)
{
	memset(&m_attr, 0, sizeof(m_attr));
	m_attr.lfHeight         = size2height(size);
	m_attr.lfWidth          = 0;
	m_attr.lfEscapement     = 0;
	m_attr.lfOrientation    = 0;
	m_attr.lfCharSet        = SHIFTJIS_CHARSET;
	m_attr.lfOutPrecision   = OUT_DEFAULT_PRECIS;
	m_attr.lfClipPrecision  = CLIP_DEFAULT_PRECIS;
	m_attr.lfQuality        = DEFAULT_QUALITY;
	m_attr.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS;

	strcpy(m_attr.lfFaceName, face.c_str());
	set_up_flags(flags);

	set_handle(CreateFontIndirect(&m_attr));
}

Font::Font(const LOGFONT& logfont)
{
	m_attr = logfont;
	m_size = height2size(m_attr.lfHeight);

	set_handle(CreateFontIndirect(&m_attr));
}

Font::Font(HFONT font, bool own)
:	GDIObject<HFONT>(font, own)
{
	if(font == NULL)
		return;

	GetObject(font, sizeof(m_attr), &m_attr);
	m_size = height2size(m_attr.lfHeight);
}

Font::Font(const Font& val)
{
	operator=(val);
}

Font::~Font()
{
}

Font&
Font::operator=(const Font& rhs)
{
	if(&rhs == this)
		return *this;

	GDIObject<HFONT>::operator=(rhs);
	m_attr = rhs.m_attr;
	m_size = rhs.m_size;

	return *this;
}

coord
Font::string_width(const DC& dc, const String& str) const
{
	return dc.string_width(str, str.size());
}

coord
Font::string_height(const DC& dc) const
{
	return dc.string_height();
}

//--------------------------------------------------------
// public accessors

String
Font::face() const
{
	return m_attr.lfFaceName;
}

void
Font::set_face(const String& face)
{
	if(face == m_attr.lfFaceName)
		return;

	strcpy(m_attr.lfFaceName, face.c_str());

	set_handle(CreateFontIndirect(&m_attr));
}

coord
Font::size() const
{
	return m_size;
}

void
Font::set_size(coord size)
{
	if(size == m_size)
		return;

	m_size = size;
	m_attr.lfHeight = size2height(m_size);

	set_handle(CreateFontIndirect(&m_attr));
}

uint
Font::flags() const
{
	uint val = m_attr.lfWeight < FW_BOLD ? NORMAL : BOLD;

	if(m_attr.lfItalic)    val |= ITALIC;
	if(m_attr.lfUnderline) val |= UNDER_LINE;
	if(m_attr.lfStrikeOut) val |= STRIKE_OUT;

	return val;
}

void
Font::set_flags(uint flags_)
{
	if(flags_ == flags())
		return;

	set_up_flags(flags_);

	set_handle(CreateFontIndirect(&m_attr));
}

const LOGFONT&
Font::data() const
{
	return m_attr;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public functions

int
Font::size2height(coord size)
{
	return -MulDiv(size, GetDeviceCaps(TemporaryScreenDC(), LOGPIXELSY), 72);
}

coord
Font::height2size(int height)
{
	return MulDiv(abs(height), 72, GetDeviceCaps(TemporaryScreenDC(), LOGPIXELSY));
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// private functions

void
Font::set_up_flags(uint flags)
{
	m_attr.lfWeight    = flags & BOLD       ? FW_BOLD : FW_NORMAL;
	m_attr.lfItalic    = flags & ITALIC     ? TRUE : FALSE;
	m_attr.lfUnderline = flags & UNDER_LINE ? TRUE : FALSE;
	m_attr.lfStrikeOut = flags & STRIKE_OUT ? TRUE : FALSE;
}

//########################################################
// public methods

Pen::Pen()
{
//	m_attr.lopnStyle = PS_SOLID;
//	m_attr.lopnWidth = 0;
//	m_attr.lopnColor = black_color().ref();

//	set_handle(CreatePenIndirect(&m_attr);

	memset(&m_attr, 0, sizeof(m_attr));
}

Pen::Pen(const RGBColor& color, coord width, uint style)
{
	m_attr.lopnStyle = style;
	m_attr.lopnWidth = Point(width, 0);
	m_attr.lopnColor = color.ref();

	set_handle(CreatePenIndirect(&m_attr));
}

Pen::Pen(const LOGPEN& logpen)
{
	m_attr = logpen;

	set_handle(CreatePenIndirect(&m_attr));
}

Pen::Pen(HPEN hpen, bool own)
:	GDIObject<HPEN>(hpen, own)
{
	if(hpen == NULL)
		return;

	GetObject(hpen, sizeof(m_attr), &m_attr);
}

Pen::Pen(const Pen& val)
{
	operator=(val);
}

Pen::~Pen()
{
}

Pen&
Pen::operator=(const Pen& rhs)
{
	if(&rhs == this)
		return *this;

	GDIObject<HPEN>::operator=(rhs);
	m_attr = rhs.m_attr;

	return *this;
}

RGBColor
Pen::color() const
{
	return m_attr.lopnColor;
}

void
Pen::set_color(const RGBColor& color)
{
	if(color.ref() == m_attr.lopnColor)
		return;

	m_attr.lopnColor = color.ref();

	set_handle(CreatePenIndirect(&m_attr));
}

coord
Pen::width() const
{
	return m_attr.lopnWidth.x;
}

void
Pen::set_width(coord width)
{
	if(width == m_attr.lopnWidth.x)
		return;

	m_attr.lopnWidth = Point(width, 0);

	set_handle(CreatePenIndirect(&m_attr));
}

uint
Pen::style() const
{
	return m_attr.lopnStyle;
}

void
Pen::set_style(uint style)
{
	if(style == m_attr.lopnStyle)
		return;

	m_attr.lopnStyle = style;

	set_handle(CreatePenIndirect(&m_attr));
}

const LOGPEN&
Pen::data() const
{
	return m_attr;
}

//########################################################
// public methods

Brush::Brush()
{
//	m_attr.lbColor = black_color().ref();
//	m_attr.lbStyle = BS_SOLID;
//	m_attr.lbHatch = 0;

//	set_handle(CreateBrushIndirect(&m_attr));

	memset(&m_attr, 0, sizeof(m_attr));
}

Brush::Brush(const RGBColor& color, uint style, long hatch_style)
{
	m_attr.lbColor = color.ref();
	m_attr.lbStyle = style;
	m_attr.lbHatch = hatch_style;

	set_handle(CreateBrushIndirect(&m_attr));
}

Brush::Brush(HBRUSH hbrush, bool own)
:	GDIObject<HBRUSH>(hbrush, own)
{
	if(hbrush == NULL)
		return;

	GetObject(hbrush, sizeof(m_attr), &m_attr);
}

Brush::Brush(const Brush& val)
{
	operator=(val);
}

Brush::~Brush()
{
}

Brush&
Brush::operator=(const Brush& rhs)
{
	if(&rhs == this)
		return *this;

	GDIObject<HBRUSH>::operator=(rhs);
	m_attr = rhs.m_attr;

	return *this;
}

RGBColor
Brush::color() const
{
	return m_attr.lbColor;
}

void
Brush::set_color(const RGBColor& color)
{
	if(color.ref() == m_attr.lbColor)
		return;

	m_attr.lbColor = color.ref();

	set_handle(CreateBrushIndirect(&m_attr));
}

uint
Brush::style() const
{
	return m_attr.lbStyle;
}

void
Brush::set_style(uint style)
{
	if(style == m_attr.lbStyle)
		return;

	m_attr.lbStyle = style;

	set_handle(CreateBrushIndirect(&m_attr));
}

long
Brush::hatch_style() const
{
	return m_attr.lbHatch;
}

void
Brush::set_hatch_style(long hatch_style)
{
	if(hatch_style == m_attr.lbHatch)
		return;

	m_attr.lbHatch = hatch_style;

	set_handle(CreateBrushIndirect(&m_attr));
}

const LOGBRUSH&
Brush::data() const
{
	return m_attr;
}

//########################################################
// public methods

DDB::DDB(HDC hdc, coord width, coord height)
:	GDIObject<HBITMAP>(CreateCompatibleBitmap(hdc, width, height))
,	m_hdc(hdc)
,	m_width(width)
,	m_height(height)
,	m_num_planes(0)
,	m_bits_per_pixel(0)
{
}

DDB::DDB(coord width, coord height, uint num_planes, uint bits_per_pixel, const void* bits)
:	GDIObject<HBITMAP>(CreateBitmap(width, height, num_planes, bits_per_pixel, bits))
,	m_hdc(NULL)
,	m_width(width)
,	m_height(height)
,	m_num_planes(num_planes)
,	m_bits_per_pixel(bits_per_pixel)
{
}

DDB::DDB(HBITMAP hbitmap, bool own)
:	GDIObject<HBITMAP>(hbitmap, own)
,	m_hdc(NULL)
,	m_width(-1)
,	m_height(-1)
,	m_num_planes(0)
,	m_bits_per_pixel(0)
{
}

DDB::DDB(const DDB& val)
{
	operator=(val);
}

DDB::~DDB()
{
}

DDB&
DDB::operator=(const DDB& rhs)
{
	if(&rhs == this)
		return *this;

	GDIObject<HBITMAP>::operator=(rhs);
	m_hdc = rhs.m_hdc;
	m_width = rhs.m_width;
	m_height = rhs.m_height;
	m_num_planes = rhs.m_num_planes;
	m_bits_per_pixel = rhs.m_bits_per_pixel;

	return *this;
}

HDC
DDB::hdc() const
{
	return m_hdc;
}

coord
DDB::width() const
{
	return m_width;
}

coord
DDB::height() const
{
	return m_height;
}

uint
DDB::num_planes() const
{
	return m_num_planes;
}

uint
DDB::bits_per_pixel() const
{
	return m_bits_per_pixel;
}

//########################################################
// public methods

Region::Region()
:	GDIObject<HRGN>(CreateRectRgn(0, 0, 1, 1))
{
}

Region::Region(const Rect& rect, Shape shape)
:	GDIObject<HRGN>(shape == RECTANGLE ?
		CreateRectRgnIndirect(&rect) :
		CreateEllipticRgn(rect.left, rect.top, rect.right, rect.bottom))
{
}

Region::Region(coord l, coord t, coord r, coord b, Shape shape)
:	GDIObject<HRGN>(shape == RECTANGLE ?
		CreateRectRgn(l, t, r, b) :
		CreateEllipticRgn(l, t, r, b))
{
}

Region::Region(const Rect& rect, coord width, coord height)
:	GDIObject<HRGN>(CreateRoundRectRgn(
		rect.left, rect.top, rect.right, rect.bottom, width, height))
{
}

Region::Region(coord l, coord t, coord r, coord b, coord width, coord height)
:	GDIObject<HRGN>(CreateRoundRectRgn(l, t, r, b, width, height))
{
}

Region::Region(const Point* point_list, uint num_of_points, uint polygon_filling_mode)
:	GDIObject<HRGN>(CreatePolygonRgn(point_list, num_of_points, polygon_filling_mode))
{
}

Region::Region(HRGN hregion, bool own)
:	GDIObject<HRGN>(hregion, own)
{
}

Region::Region(const Region& val)
:	GDIObject<HRGN>(CreateRectRgn(0, 0, 1, 1))
{
	operator=(val);
}

Region&
Region::operator=(const Region& rhs)
{
	if(&rhs == this)
		return *this;

	// GDIObject<HRGN>::operator=(rhs);
	// Ç±ÇÃçsÇÕÇ¢ÇÁÇ»Ç¢ÅBÉnÉìÉhÉãÇÕã§óLÇπÇ∏Ç…ÉfÅ[É^ÇÉRÉsÅ[Ç∑ÇÈÅB
	// è⁄ÇµÇ≠ÇÕ RefCountable::operator=() ÇéQè∆ÇÃÇ±Ç∆

	CombineRgn(handle(), rhs.handle(), NULL, RGN_COPY);

	return *this;
}

Region::~Region()
{
}

void
Region::offset_by(coord horizontal, coord vertical)
{
	OffsetRgn(handle(), horizontal, vertical);
}

void
Region::include(const Region& region)
{
	CombineRgn(handle(), handle(), region.handle(), RGN_OR);
}

void
Region::include(const Rect& rect)
{
	include(Region(rect));
}

void
Region::exclude(const Region& region)
{
	CombineRgn(handle(), handle(), region.handle(), RGN_DIFF);
}

void
Region::exclude(const Rect& rect)
{
	exclude(Region(rect));
}

void
Region::intersect(const Region& region)
{
	CombineRgn(handle(), handle(), region.handle(), RGN_AND);
}

void
Region::intersect(const Rect& rect)
{
	intersect(Region(rect));
}

bool
Region::contains(coord x, coord y) const
{
	return to_bool(PtInRegion(handle(), x, y));
}

bool
Region::contains(const Rect& rect) const
{
	return to_bool(RectInRegion(handle(), &rect));
}

bool
Region::intersects(const Region& region)
{
	Region tmp(0, 0, 1, 1);
	int result = CombineRgn(tmp.handle(), handle(), region.handle(), RGN_AND);

	return result != NULLREGION;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

void
Region::reset(const Rect& rect)
{
	SetRectRgn(handle(), rect.left, rect.top, rect.right, rect.bottom);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// piblic operators

bool
operator==(const Region& lhs, const Region& rhs)
{
	return to_bool(EqualRgn(lhs.handle(), rhs.handle()));
}

const Region&
operator&=(Region& lhs, const Region& rhs)
{
	lhs.intersect(rhs);
	return lhs;
}

const Region&
operator-=(Region& lhs, const Region& rhs)
{
	lhs.exclude(rhs);
	return lhs;
}

const Region&
operator|=(Region& lhs, const Region& rhs)
{
	lhs.include(rhs);
	return lhs;
}

const Region&
operator^=(Region& lhs, const Region& rhs)
{
	CombineRgn(lhs.handle(), lhs.handle(), rhs.handle(), RGN_XOR);
	return lhs;
}


//========================================================
// font

typedef std::vector<String> FontFamilyList;
static FontFamilyList g_font_family_list;

static int CALLBACK
enum_font_families_proc_(ENUMLOGFONTEX* logical_font, NEWTEXTMETRICEX* physical_font, int font_type, LPARAM param)
{
	g_font_family_list.push_back(String(logical_font->elfLogFont.lfFaceName));

	if(font_type == TRUETYPE_FONTTYPE)
	{
	}else{
	}

	return 1;
}

static bool initialize_()
{
	LOGFONT logical_font;
	logical_font.lfCharSet				= DEFAULT_CHARSET;
	logical_font.lfFaceName[0]		= '\0';
	logical_font.lfPitchAndFamily	= 0;

	EnumFontFamiliesEx(ScreenDC(), &logical_font, reinterpret_cast<FONTENUMPROC>(enum_font_families_proc_), 0, 0);

	return true;
}
//static bool initializer_ = initialize_();


struct SystemFontList {
	Font* list[NUM_OF_SYSTEM_FONTS];

	SystemFontList()
	{
		list[PLAIN_FONT] = new Font((HFONT)GetStockObject(ANSI_VAR_FONT), false);//("ÇlÇr ÇoÉSÉVÉbÉN", 9);

		list[BOLD_FONT] = new Font(*list[PLAIN_FONT]);//"ÇlÇr ÇoÉSÉVÉbÉN", 9, Font::BOLD);
		list[BOLD_FONT]->set_flags(list[BOLD_FONT]->flags() | Font::BOLD);

		list[FIXED_FONT] = new Font((HFONT)GetStockObject(ANSI_FIXED_FONT), false);//("ÇlÇr ÇoÉSÉVÉbÉN", 9);
		list[FIXED_FONT]->set_size(list[PLAIN_FONT]->size());
	}
	~SystemFontList()
	{
		for(int i=0; i<NUM_OF_SYSTEM_FONTS; ++i)
			delete list[i];
	}
};

const Font&
system_font(SystemFontID font_id)
{
	assert(0 <= font_id && font_id < NUM_OF_SYSTEM_FONTS);

	static boost::shared_ptr<SystemFontList> fonts;
	if(! fonts)
		fonts.reset(new SystemFontList);

	return *fonts->list[font_id];
}


//========================================================
// system pens

struct SystemPenList {
	Pen* list[NUM_OF_SYSTEM_PENS];

	SystemPenList()
	{
		list[TRANSPARENT_PEN]	= new Pen(reinterpret_cast<HPEN>(GetStockObject(NULL_PEN)), false);
		list[OUTLINE_PEN]			= new Pen(system_color(OUTLINE_COLOR));
	}
	~SystemPenList()
	{
		for(int i=0; i<NUM_OF_SYSTEM_PENS; ++i)
			delete list[i];
	}
};

const Pen&
system_pen(SystemPenID pen_id)
{
	assert(0 <= pen_id && pen_id < NUM_OF_SYSTEM_PENS);

	static boost::shared_ptr<SystemPenList> pens;
	if(! pens)
		pens.reset(new SystemPenList);

	return *pens->list[pen_id];
}

//========================================================
// system brushs

struct SystemBrushList {
	Brush* list[NUM_OF_SYSTEM_BRUSHES];

	SystemBrushList()
	{
		list[TRANSPARENT_BRUSH]	= new Brush(reinterpret_cast<HBRUSH>(GetStockObject(NULL_BRUSH)), false);
		list[BACKGROUND_BRUSH]	= new Brush(system_color(VIEW_COLOR));
	}
	~SystemBrushList()
	{
		for(int i=0; i<NUM_OF_SYSTEM_BRUSHES; ++i)
			delete list[i];
	}
};

const Brush&
system_brush(SystemBrushID brush_id)
{
	assert(0 <= brush_id && brush_id < NUM_OF_SYSTEM_BRUSHES);

	static boost::shared_ptr<SystemBrushList> brushes;
	if(! brushes)
		brushes.reset(new SystemBrushList);

	return *brushes->list[brush_id];
}


//########################################################
}//namespace ntk
