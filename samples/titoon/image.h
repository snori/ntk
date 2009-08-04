#ifndef __IMAGE_H__
#define __IMAGE_H__


#include <ntk/interface/bitmap.h>


class Image {//: public NObservable {
public:
	Image(coord width, coord height);

	NBitmap& bitmap() {return m_bitmap;}
	const NBitmap& bitmap() const {return m_bitmap;}
	coord width() const {return m_bitmap.width();}
	coord height() const {return m_bitmap.height();}

private:
	NBitmap m_bitmap;

};// class Image


#endif//EOH