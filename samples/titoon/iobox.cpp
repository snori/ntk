#include "iobox.h"


#include <ntk/interface/button.h>
#include <ntk/interface/layout.h>
#include "resource.h"


//########################################################


IOBox::IOBox(const NRect& frame, const NString& name)
:	NView(frame, name, FOLLOW_ALL_SIDES, DEFAULT_FLAGS, ntk::app_color())
{
	set_up_controls_();
}

void
IOBox::set_up_controls_()
{
	enum{ SPACE = 3, WIDTH = 160, HEIGHT = 17 };

	NLayout layout_0(this, 0, -1);
	layout_0.rest_rect().inset_by(SPACE, SPACE);
}


//########################################################
