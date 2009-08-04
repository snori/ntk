#ifndef __EDITBOX_H__
#define __EDITBOX_H__


#include <ntk/interface/view.h>


class EditBox : public NView {
public:
	EditBox(const NRect& frame, const NString& name);

private:
	void set_up_controls_();

};// class EditBox


#endif//EOH