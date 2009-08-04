#ifndef __IMAGE_H__
#define __IMAGE_H__


#include <ntk/storage/entry.h>
#include <ntk/interface/bitmap.h>


class image_t {
public:
	//
	// methods
	//
	image_t(const ntk_rect& frame, NBitmap::ColorSpace color_space_)
	:	m_frame(frame)
	,	m_bitmap(new ntk_bitmap(frame.width(), frame.height(), color_space_))
	{}
	image_t(const ntk_entry& entry, const ntk_rect& frame);
	~image_t()
	{
		delete m_bitmap;
	}

	//
	// accessors and manipulators
	//
	ntk_bitmap* bitmap() {return m_bitmap;}
	const ntk_bitmap* bitmap() const {return m_bitmap;}

	const ntk_rect& frame() const {return m_frame;}
	ntk_rect bounds() const {return m_frame.offset_to_copy(0, 0);}
	ntk_bitmap::color_space_t color_space() const
		{return m_bitmap ? m_bitmap->color_space() : ntk_bitmap::NO_COLOR_SPACE;}

private:
	//
	// data
	//
	ntk_rect m_frame;
	ntk_bitmap* m_bitmap;

};// class image_t

template<
	typename VALUE,
	int NUM_CHANNELS,
	bool HAS_ALPHA_CHANNEL = false,
	int ALPHA_CHANNEL = 0,
	int RED_CHANNEL   = 0,
	int GREEN_CHANNEL = 1,
	int BLUE_CHANNEL  = 2>
class pixel_t {
public:
	//
	// types
	//
	typedef VALUE value_t;

	//
	// accessors and manipulators
	//
	bool has_alpha() const {return HAS_ALPHA_CHANNEL;}
	value_t alpha() const {return m_value[ALPHA_CHANNEL];}

private:
	//
	// data
	//
	value_t m_value[NUM_CHANNELS];

};// class pixel_t

// 8 bits per channel pixels
typedef pixel_t<uint8, 1>                    Gray_8;
typedef pixel_t<uint8, 1, true, 0>           Alpha_8;
typedef pixel_t<uint8, 2, true, 1>           GrayAlpha_88;
typedef pixel_t<uint8, 3>                    RGB_888;
typedef pixel_t<uint8, 3, false, 0, 2, 1, 0> BGR_888;
typedef pixel_t<uint8, 4, true, 3>           RGBA_8888;
typedef pixel_t<uint8, 4, true, 3, 2, 1, 0>  BGRA_8888;

// 16 bits per channel pixels
typedef pixel_t<uint16, 1>                    Gray_16;
typedef pixel_t<uint16, 1, true, 0>           Alpha_16;
typedef pixel_t<uint16, 2, true, 1>           GrayAlpha_1616;
typedef pixel_t<uint16, 3>                    RGB_161616;
typedef pixel_t<uint16, 3, false, 0, 2, 1, 0> BGR_161616;
typedef pixel_t<uint16, 4, true, 3>           RGBA_16161616;
typedef pixel_t<uint16, 4, true, 3, 2, 1, 0>  BGRA_16161616;

//########################################################

class image_compositor_t {
public:
	virtual void compose(image_t* dest, const image_t& base, const image_t& over) const =0;
};// class image_compositor

class average_image_compositor_t : public image_compositor_t {
public:
	void compose(image_t* dest, const image_t& base, const image_t& over) const;
};// class average_image_compositor_t

#endif//EOH
