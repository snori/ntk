/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: statusbar.cpp,v 1.1 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
/**
 * @file StatusBar.cpp
 * @brief Win32 スタンダードのステータスバーを実装するクラス
 *
 */
#define NTK_BUILD
#include "ntk/interface/statusbar.h"


#include <commctrl.h>


namespace ntk {


//########################################################
// public methods

/**
 * ステータスバーを生成する
 *
 * @param name View オブジェクトの識別名
 * @param flags フラグ
 * @param color View の色
 */
StatusBar::StatusBar(const String& name, uint num_partitions, const int width_list[], uint flags, const RGBColor& color)
:	View(STATUSCLASSNAME, Rect(0, 0, 0, 0), name, FOLLOW_LEFT_RIGHT | FOLLOW_BOTTOM, flags, color, DEFAULT_STYLE | CCS_BOTTOM)// | SBARS_SIZEGRIP)
{
	setup_partitions_(num_partitions, width_list);
}

String
StatusBar::text(uint partition_id) const
{
	char buf[512];
	SendMessage(hwnd(), SB_GETTEXT, partition_id, (LPARAM)buf);

	return buf;
}

void
StatusBar::set_text(uint partition_id, const String& text, uint flags)
{
	if(m_partition_widths.size())
		SendMessage(hwnd(), SB_SETTEXT, partition_id | flags, (LPARAM)text.c_str());
	else
		SendMessage(hwnd(), SB_SETTEXT, 255, (LPARAM)text.c_str());
}

void
StatusBar::attached_to_window()
{
	resize_to(0, 0);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// functions

void
StatusBar::setup_partitions_(uint num_partitions, const int width_list[])
{
	if(num_partitions <= 1 || width_list == NULL)
	{
		m_partition_widths.clear();
		SendMessage(hwnd(), SB_SIMPLE, TRUE, 0L);
	}else{
		for(int i=0; i<num_partitions; ++i)
			m_partition_widths.push_back(width_list[i]);

		SendMessage(hwnd(), SB_SETPARTS, m_partition_widths.size(), (LPARAM)&m_partition_widths[0]);
	}
}


//########################################################
}// namespace ntk
