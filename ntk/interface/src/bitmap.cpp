/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: bitmap.cpp,v 1.4 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/bitmap.h"


#include <math.h>


namespace ntk {


//########################################################
// public methods

Bitmap::Bitmap(coord width, coord height, ColorSpace color_space_)
:	m_bits(NULL),
	m_hdc(NULL),
	m_hbitmap(NULL),
	m_old_hbitmap(NULL)
{
	initialize_(width, height, color_space_);
}

Bitmap&
Bitmap::operator=(const Bitmap& rhs)
{
	initialize_(rhs.width(), rhs.height(), rhs.color_space());
	memcpy(bits(), rhs.bits(), length());

	return *this;
}

Bitmap::~Bitmap()
{
	finalize_();
}

//********************************************************
// private methods

void
Bitmap::initialize_(coord width, coord height, ColorSpace color_space_)
{
	// 今のデータは破棄する
	finalize_();

	m_width				= abs(width);
	m_height			= abs(height);
	m_color_space	= color_space_;

	m_pitch		= m_width * (m_color_space / 8);
	m_pitch		= (m_pitch +3) & ~3;

	memset(&m_bitmap_info, 0, sizeof(m_bitmap_info));
	m_bitmap_info.info_header.size        = sizeof(m_bitmap_info.info_header);
	m_bitmap_info.info_header.width       = m_width;
	m_bitmap_info.info_header.height      = height < 0 ? m_height : -m_height;// top-down bitmap. 普通とは指定が逆。マイナスの時さかさま
	m_bitmap_info.info_header.planes      = 1;
	m_bitmap_info.info_header.bit_count   = (word)m_color_space;
	m_bitmap_info.info_header.compression = BI_RGB;

	class initialize_exception {};
	try
	{
		m_hdc = CreateCompatibleDC(NULL);
		if(m_hdc == NULL)
			throw initialize_exception();

		m_hbitmap = CreateDIBSection(
			m_hdc,
			reinterpret_cast<BITMAPINFO*>(&m_bitmap_info),
			DIB_RGB_COLORS,
			&m_bits,
			NULL,
			0);
		if(m_hbitmap == NULL)
			throw initialize_exception();

		m_old_hbitmap = (HBITMAP)SelectObject(m_hdc, m_hbitmap);
	}
	catch(const initialize_exception&)
	{
		finalize_();
	}
	catch(...) {throw;}
}

void
Bitmap::finalize_()
{
	if(m_bits == NULL) return;

	if(m_hbitmap)
	{
		SelectObject(m_hdc, m_old_hbitmap);

		DeleteObject(m_hbitmap);
		m_hbitmap = NULL;
	}

	if(m_hdc)
	{
		DeleteDC(m_hdc);
		m_hdc = NULL;
	}

	m_bits = NULL;
	m_width = m_height = m_pitch = 0;
	m_color_space = NO_COLOR_SPACE;
}


//########################################################
}// namespace ntk
