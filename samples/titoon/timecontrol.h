#ifndef __TIMECONTROL_H__
#define __TIMECONTROL_H__


#include <ntk/interface/view.h>
#include <ntk/interface/scintilla.h>


class TimeControl : public NView {
public:
	TimeControl(const NRect& frame);

	int current_frame() const;
	void set_current_frame(int frame);

private:
	int m_current_frame;
	NScintilla *m_min_frame_text, *m_max_frame_text;

	void set_up_controls_();

};// class TimeControl


#endif//EOH