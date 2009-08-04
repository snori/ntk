#ifndef __EFFECTBOX_H__
#define __EFFECTBOX_H__


#include <ntk/interface/view.h>


class EffectBox : public NView {
public:
	EffectBox(const NRect& frame, const NString& name);

private:
	void set_up_controls_();

};// class EffectBox


#endif//EOH