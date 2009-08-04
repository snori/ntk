#ifndef __IOBOX_H__
#define __IOBOX_H__


#include <ntk/interface/view.h>


class IOBox : public NView {
public:
	IOBox(const NRect& frame, const NString& name);

private:
	void set_up_controls_();

};// class IOBox


#endif//EOH