/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: bitmap.h,v 1.9 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_BITMAP_H__
#define __NTK_INTERFACE_BITMAP_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_POINT_H__
#include <ntk/interface/point.h>
#endif

#ifndef __NTK_INTERFACE_DC_H__
#include <ntk/interface/dc.h>
#endif


namespace ntk {


class Bitmap {
public:
	//
	// types
	//
	typedef uint8* iterator;
	typedef const uint8* const_iterator;
	typedef size_t size_type;

	#pragma pack(push, 1)
		struct BitmapInfoHeader {
			dword size;
			long width;
			long height;
			word planes;
			word bit_count;
			dword compression;
			dword size_image;
			long h_pixels_per_meter;
			long v_pixels_per_meter;
			dword color_used;
			dword color_important;
		};

		struct BitmapInfo {
			BitmapInfoHeader info_header;
			RGBQUAD palette[1];
		};
	#pragma pack(pop)

	typedef BitmapInfoHeader DIBInfoHeader;
	typedef BitmapInfo DIBInfo;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef size_type size_t;
		typedef BitmapInfoHeader bitmap_info_header_t;
		typedef BitmapInfo       bitmap_info_t;
		typedef DIBInfoHeader    dib_info_header_t;
		typedef DIBInfo          dib_info_t;
	#endif

	//
	// constants
	//
	enum ColorSpace {
		NO_COLOR_SPACE = 0,
		INDEX_8        = 8,
		RGB_16         = 16,
		RGB_24         = 24,
		RGBA_32        = 32,
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef ColorSpace color_space_t;
	#endif

	//
	// methods
	//
	NtkExport Bitmap(coord width, coord height, ColorSpace color_space);
	Bitmap(const Bitmap& bitmap) {operator=(bitmap);}
	NtkExport Bitmap& operator=(const Bitmap& rhs);
	NtkExport ~Bitmap();

	//
	// accessors
	//
	iterator begin() {return reinterpret_cast<iterator>(m_bits);}
	const_iterator begin() const {return reinterpret_cast<const_iterator>(m_bits);}

	iterator end() {return begin() + length();}
	const_iterator end() const {return begin() + length();}

	size_type size() const {return pitch()*height();}
	size_type length() const {return size();}

	void* bits() {return m_bits;}
	const void* bits() const {return m_bits;}

	void* at(coord x, coord y) {return begin() + pitch()*y + x*(bpp() >> 3);}
	const void* at(coord x, coord y) const {return begin() + pitch()*y + x*(bpp() >> 3);}
	void* at(const Point& point) {return at(point.x, point.y);}
	const void* at(const Point& point) const {return at(point.x, point.y);}

	ColorSpace color_space() const {return (ColorSpace)m_color_space;}
	coord width() const {return m_width;}
	coord height() const {return m_height;}
	uint bpp() const {return m_color_space;}// bits per pixel
	uint Bpp() const {return m_color_space/8;}// bytes per pixel
	uint bits_per_pixel() const {return bpp();}
	uint bytes_per_pixel() const {return Bpp();}
	coord pitch() const {return m_pitch;}

	Rect bounds() const {return Rect(0, 0, width(), height());}

	HBITMAP handle() const {return m_hbitmap;}
	HDC hdc() const {return m_hdc;}
	DC dc() const {return DC(m_hdc);}

	//
	// operators
	//
	operator HBITMAP() {return handle();}
	operator const HBITMAP() const {return handle();}

private:
	//
	// methods
	//
	void initialize_(coord width, coord height, ColorSpace color_space);
	void finalize_();

	//
	// data
	//
	void* m_bits;
	coord m_width, m_height, m_pitch;
	uint m_color_space;

	BitmapInfo m_bitmap_info;
	HDC m_hdc;
	HBITMAP m_hbitmap, m_old_hbitmap;

	//
	// friends
	//
	friend class TranslationUtils;

};// class Bitmap


typedef Bitmap DIB;


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Bitmap bitmap_t;
	typedef DIB    dib_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Bitmap NBitmap;
	typedef ntk::DIB    NDIB;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Bitmap ntk_bitmap;
	typedef ntk::DIB    ntk_dib;
	#endif
#endif


#endif//EOH
