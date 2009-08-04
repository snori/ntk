#include "image.h"


//########################################################


Image::Image(coord width, coord height)
:	m_bitmap(width, height, NBitmap::RGB_24)
{
	memset(m_bitmap.bits(), 255, m_bitmap.size());
}


//########################################################
