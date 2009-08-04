#ifndef __BRUSHSTROKE_H__
#define __BRUSHSTROKE_H__


class brush_stroke_t {
public:
	//
	// methds
	//
	brush_stroke_t() {}

	void append_point(cosnt ntk_point& point)
	{
		m_point_list.push_back(point);
	}

private:
	//
	// types
	//
	typedef std::vector<point_brush_t*> point_list_t;

	//
	// data
	//
	point_list_t m_point_list;

};// class brush_stroke_t


#endif//EOH