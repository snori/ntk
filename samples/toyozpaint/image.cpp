#include "Image.h"

#include <ntk/translation/translationutility.h>


//########################################################


image_t::image_t(const ntk_entry& entry, const ntk_rect& frame)
{
	if(entry.init_check() == false)
		return;

	m_frame = frame;
	m_bitmap = ntk_translation_utils::get_bitmap(entry.path().as_string());
}


//########################################################


void
average_image_compositor_t::compose(image_t* dest_image, const image_t& base_image, const image_t& over_image) const
{
	if(dest_image == NULL)
		return;
		
	if(! (
		dest_image->color_space() == base_image.color_space() &&
		dest_image->color_space() == over_image.color_space() ))
	{
		return;
	}

	uint8* dest       = (uint8*)dest_image->bitmap()->bits();
	const uint8* base = (uint8*)base_image.bitmap()->bits();
	const uint8* over = (uint8*)over_image.bitmap()->bits();

	int count = dest_image->bitmap()->size();
	while(count--)
	{
		*dest = (*base + *over) /2;
	}
}
