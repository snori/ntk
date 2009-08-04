/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: statusbar.h,v 1.5 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_STATUSBAR_H__
#define __NTK_INTERFACE_STATUSBAR_H__


#include <vector>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_VIEW_H__
#include <ntk/interface/view.h>
#endif


namespace ntk {


class StatusBar : public View {
public:
	//
	// methods
	//
	NtkExport StatusBar(
		const String& name,
		uint num_partitions = 0,
		const int width_list[] = NULL,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = ntk::app_color());

	NtkExport String text(uint partition_id) const;
	NtkExport void set_text(uint partition_id, const String& text, uint flags = 0);

	//
	// message handlers
	//
	NtkExport void attached_to_window();

private:
	//
	// types
	//
	typedef std::vector<uint> PartitionWidthList;

	//
	// data
	//
	PartitionWidthList m_partition_widths;

	//
	// functions
	//
	void setup_partitions_(uint num_partitions, const int widths[]);

};// class StatusBar


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef StatusBar status_bar_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::StatusBar NStatusBar;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::StatusBar ntk_status_bar;
	#endif
#endif


#endif//EOH
