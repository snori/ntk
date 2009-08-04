#ifndef __ANIMATION_H__
#define __ANIMATION_H__


#include <vector>
#include <ntk/support/dataio.h>
#include "document.h"


class Image;


class Animation : public Document {
public:
	struct Frame {
		Image* image;
		float delay;

		Frame(Image* img, float dly = -1) : image(img), delay(dly) {}
		~Frame();
	};

	Animation(coord width, coord height);
	~Animation();

	void insert_frame(int index = -1, Frame* frame = NULL);// -1 : insert to last pos
	void erase_frame(int index);

	Frame* frame_at(int index) const;
	Image* image_at(int index) const;

	coord width() const {return m_width;}
	coord height() const {return m_height;}
	uint length() const {return m_frames.size();}

	bool save(NDataIO* io) const;
	static Animation* load(NDataIO* io);

private:
	typedef std::vector<Frame*> FrameList;

	coord m_width, m_height;
	FrameList m_frames;

	static Animation* load_(NDataIO* io);

};// class Animation


#endif//EOH