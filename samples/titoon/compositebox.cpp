#include "compositebox.h"


//########################################################


CompositeBox::CompositeBox(const NRect& frame, const NString& name)
:	NView(frame, name, FOLLOW_ALL_SIDES, DEFAULT_FLAGS, ntk::app_color())
{
	set_up_controls_();
}

void
CompositeBox::set_up_controls_()
{
}


//########################################################
