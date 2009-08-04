#include "animation.h"


#include <vector>
#include <algorithm>
#include "titoon.h"
#include "titoonwindow.h"
#include "image.h"


//########################################################


namespace {

const NString UNIQUE_ID("TTANM");

}// anonymous namespace


//########################################################


Animation::Frame::~Frame()
{
	delete image;
}


//########################################################


NString
untitled_doc_number()
{
	static int num = 1;
	return ntk::format_string("%d", num++);
}


Animation::Animation(coord width, coord height)
:	m_width(width)
,	m_height(height)
{
}

Animation::~Animation()
{
	for(FrameList::iterator it = m_frames.begin(); it != m_frames.end(); ++it)
		delete *it;
}

void
Animation::insert_frame(int index, Frame* frame)
{
	FrameList::iterator pos = m_frames.begin();

	if(index > m_frames.size() || index < 0)
		pos = m_frames.end();
	else
		std::advance(pos, index);

	if(frame == NULL)
		frame = new Frame(new Image(width(), height()));

	m_frames.insert(pos, frame);

	titoon()->window()->invalidate();
}

void
Animation::erase_frame(int index)
{
	if(index > m_frames.size() || m_frames.size() == 1)
		return;

	FrameList::iterator it = m_frames.begin();

	if(index < 0)
		it = m_frames.end();
	else
		std::advance(it, index);

	delete *it;
	m_frames.erase(it);

	titoon()->window()->invalidate();
}

Animation::Frame*
Animation::frame_at(int index) const
{
	if(index < 0 || m_frames.size() <= index)
		return NULL;

	FrameList::const_iterator it = m_frames.begin();
	std::advance(it, index);

	return *it;
}

Image*
Animation::image_at(int index) const
{
	Frame* frame = frame_at(index);

	if(frame == NULL)
		return NULL;

	return frame->image;
}

bool
Animation::save(NDataIO* io) const
{
	io->write(UNIQUE_ID.c_str(), UNIQUE_ID.size());

	titoon()->version().write(io, NVersion::INT8_TYPE);

	*io << width() << height();
	*io << m_frames.size();

	for(int i=0; i<m_frames.size(); ++i)
	{
		Frame* frame = frame_at(i);
		io->write(frame->image->bitmap().bits(), frame->image->bitmap().size());
		*io << frame->delay;
	}

	return true;
}

Animation*
Animation::load(NDataIO* io)
{
	std::vector<char> id(UNIQUE_ID.size() +1);
	io->read(&id[0], id.size() -1);
	id[id.size() -1] = '\0';

	if(&id[0] != UNIQUE_ID)
	{
		status_t(ntk::st::ERR, "Titoon の アニメーションファイルではありません").show_error();
		return NULL;
	}

	NVersion version;
	version.read(*io, NVersion::INT8_TYPE);

	if(version <= titoon()->version())
		return load_(io);
	else
	{
		status_t(ntk::st::ERR, "ファイルのバージョンが合いません").show_error();
		return NULL;
	}
}

Animation*
Animation::load_(NDataIO* io)
{
	coord w, h;
	*io >> w >> h;

	Animation* anim = new Animation(w, h);

	int num_frames;
	*io >> num_frames;

	for(int i=0; i<num_frames; ++i)
	{
		Frame* frame = new Frame(new Image(w, h));
		io->read(frame->image->bitmap().bits(), frame->image->bitmap().size());
		*io >> frame->delay;

		anim->insert_frame(i, frame);
	}

	return anim;
}


//########################################################
