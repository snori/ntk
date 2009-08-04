/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: interfaceutility.h,v 1.4 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_INTERFACEUTILITY_H__
#define __NTK_INTERFACE_INTERFACEUTILITY_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif


namespace ntk {


class DC;
class Rect;
class Pen;


// drawing utilities //
NtkExport int light_edge_color_difference(int level = 0);
NtkExport int dark_edge_color_difference(int level = 0);
NtkExport void stroke_edge(DC& dc, const Rect& rect, const Pen& light_pen, const Pen& dark_pen);
NtkExport void draw_string_center(DC& dc, const Rect& rect, const String& str);


} namespace Ntk = ntk;


#endif//EOH
