/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: previewpanel.h,v 1.1.1.1 2003/02/26 06:49:06 nsuzuki Exp $

******************************************************************************/
#ifndef __PREVIEWPANEL_H__
#define __PREVIEWPANEL_H__

#include <ntk/interface/bitmap.h>
#include <ntk/interface/panel.h>
#include "firesim.h"
#include "particle.h"

class PreviewPanel : public ntk::Panel {
public:
	PreviewPanel();

	void invalidate()
	{
		m_preview_control->invalidate();
	}

private:
	ntk::View* m_preview_control;
};

#endif//EOH