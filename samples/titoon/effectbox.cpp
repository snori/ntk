#include "effectbox.h"


//########################################################


EffectBox::EffectBox(const NRect& frame, const NString& name)
:	NView(frame, name, FOLLOW_ALL_SIDES, DEFAULT_FLAGS, ntk::app_color())
{
	set_up_controls_();
}

void
EffectBox::set_up_controls_()
{
}


//########################################################
