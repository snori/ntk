#ifndef __COMPOSITEBOX_H__
#define __COMPOSITEBOX_H__


#include <ntk/interface/view.h>


class CompositeBox : public NView {
public:
	CompositeBox(const NRect& frame, const NString& name);

private:
	void set_up_controls_();

};// class CompositeBox


#endif//EOH