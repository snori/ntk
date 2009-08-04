/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: interfacedefs.cpp,v 1.9 2003/11/19 11:42:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/interfacedefs.h"


#include <ntk/windows/windows.h>
#include <ntk/interface/point.h>
#include <ntk/support/debug.h>
#include <ntk/support/utility.h>


namespace ntk {


//########################################################
// keybord function

uint
modifiers()
{
	static uchar state[257];// +1 はなんとなく。深い意味はない。

	bool result = to_bool(GetKeyboardState(state));
	if(result == false)
	{
		error("ntk::modifiers(): キーボードの状態取得に失敗しました\n");
		return 0;
	}

	return
		state[VK_SHIFT]    & (1<<7) ? SHIFT_KEY : 0 |
		state[VK_CONTROL]  & (1<<7) ? CONTROL_KEY : 0 |
		state[VK_MENU]     & (1<<7) ? ALT_KEY : 0 |

		state[VK_LSHIFT]   & (1<<7) ? LEFT_SHIFT_KEY : 0 |
		state[VK_RSHIFT]   & (1<<7) ? RIGHT_SHIFT_KEY : 0 |

		state[VK_LCONTROL] & (1<<7) ? LEFT_CONTROL_KEY : 0 |
		state[VK_RCONTROL] & (1<<7) ? RIGHT_CONTROL_KEY : 0 |

		state[VK_LMENU]    & (1<<7) ? LEFT_ALT_KEY : 0 |
		state[VK_RMENU]    & (1<<7) ? RIGHT_ALT_KEY : 0 |

		state[VK_CAPITAL]  & (1<<0) ? CAPS_LOCK : 0 |
		state[VK_SCROLL]   & (1<<0) ? SCROLL_LOCK : 0 |
		state[VK_NUMLOCK]  & (1<<0) ? NUM_LOCK : 0;
}

void
get_mouse(Point* point, uint* buttons)
{
	// 位置情報の設定
	if(point)
		GetCursorPos(point);

	// ボタン情報の設定
	if(buttons)
		*buttons =
			(GetAsyncKeyState(VK_LBUTTON) & 1<<15 ? LEFT_MOUSE_BUTTON : 0) |
			(GetAsyncKeyState(VK_MBUTTON) & 1<<15 ? MIDDLE_MOUSE_BUTTON : 0) |
			(GetAsyncKeyState(VK_RBUTTON) & 1<<15 ? RIGHT_MOUSE_BUTTON : 0);
}

#if 0
void
set_mouse_position(const Point& point)
{
	SetCursorPos(point.x, point.y);
}
#endif


//********************************************************
// system colors


#ifndef COLOR_GRADIENTACTIVECAPTION
#define COLOR_GRADIENTACTIVECAPTION 27
#endif

#ifndef COLOR_GRADIENTINACTIVECAPTION
#define COLOR_GRADIENTINACTIVECAPTION 28
#endif


RGBColor g_color_set[][NUM_COLORS] = {
	// [0] :app color
	// [1] :font color
	// [2] :view color
	// [3] :window frame color
	// [4] :outline color
	// [5] :active window title bar color
	// [6] :active window title bar font color
	// [7] :nonactive window title bar color
	// [8] :nonactive window title bar font color
	// [9] :container panel background color

	{// std color set
		RGBColor(GetSysColor(COLOR_3DFACE)),//RGBColor(190, 185, 190);//75, 100, 75);//125, 125, 150);//50, 50, 40);
		RGBColor(GetSysColor(COLOR_WINDOWTEXT)),//  0,   0,   0),//225, 255, 255);
		RGBColor(GetSysColor(COLOR_WINDOW)),//COLOR_3DFACE)),
		RGBColor(GetSysColor(COLOR_ACTIVEBORDER)),
		RGBColor(GetSysColor(COLOR_WINDOWFRAME)),//RGBColor(GetSysColor(COLOR_3DFACE)) - 20),//COLOR_3DFACE)) - 100),//RGBColor(50, 55, 50);//100, 100, 100);
		RGBColor(GetSysColor(COLOR_ACTIVECAPTION))/2 + RGBColor(GetSysColor(COLOR_GRADIENTACTIVECAPTION))/2,//RGBColor(140, 140, 220),//colors[APP_COLOR];// +20;//RGBColor(80, 80, 70);//
		RGBColor(GetSysColor(COLOR_CAPTIONTEXT)),//RGBColor(255, 255, 255),//colors[APP_COLOR] + 100;//RGBColor(80, 80, 70);//140, 140, 220);
		RGBColor(GetSysColor(COLOR_INACTIVECAPTION))/2 + RGBColor(GetSysColor(COLOR_GRADIENTINACTIVECAPTION))/2,//RGBColor(140, 140, 220) - 40),
		RGBColor(GetSysColor(COLOR_INACTIVECAPTIONTEXT)),//RGBColor(255, 255, 255) - 30),
		RGBColor(GetSysColor(COLOR_APPWORKSPACE)),//RGBColor(255, 255, 255) - 30),
	},
	{// light gray color set
		RGBColor(227, 231, 236),
		RGBColor(204, 204, 204),
		RGBColor(109, 117, 125),
		RGBColor(159, 164, 171),
		RGBColor( 97, 103, 109),
		RGBColor(249, 195, 125),
		RGBColor(140, 107,  62),
		RGBColor(201, 205, 210),
		RGBColor( 97, 103, 109),
		RGBColor(),// 97, 103, 109),
	},
};

RGBColor&
system_color_(SystemColorID id)
{
	static RGBColor colors[NUM_COLORS];

	static bool initialized = false;
	if(! initialized)
	{
		initialized = true;

		colors[WHITE_COLOR]       = RGBColor(255, 255, 255, 255);
		colors[BLACK_COLOR]       = RGBColor(0, 0, 0, 255);
		colors[GRAY_COLOR]        = RGBColor(127, 127, 127, 255);
		colors[RED_COLOR]         = RGBColor(255, 0, 0, 255);
		colors[GREEN_COLOR]       = RGBColor(0, 255, 0, 255);
		colors[BLUE_COLOR]        = RGBColor(0, 0, 255, 255);
		colors[YELLOW_COLOR]      = RGBColor(255, 255, 0, 255);
		colors[TRANSPARENT_COLOR] = RGBColor(255, 255, 255, 0);

		enum{ SET_NO = 0 };

		colors[APP_COLOR]                            = g_color_set[SET_NO][0];
		colors[FONT_COLOR]                           = g_color_set[SET_NO][1];
		colors[VIEW_COLOR]                           = g_color_set[SET_NO][2];
		colors[WINDOW_FRAME_COLOR]                   = g_color_set[SET_NO][3];
		colors[OUTLINE_COLOR]                        = g_color_set[SET_NO][4];
		colors[ACTIVE_WINDOW_TITLE_BAR_COLOR]        = g_color_set[SET_NO][5];
		colors[ACTIVE_WINDOW_TITLE_BAR_FONT_COLOR]   = g_color_set[SET_NO][6];
		colors[INACTIVE_WINDOW_TITLE_BAR_COLOR]      = g_color_set[SET_NO][7];
		colors[INACTIVE_WINDOW_TITLE_BAR_FONT_COLOR] = g_color_set[SET_NO][8];
		colors[CONTAINER_BACKGROUND_COLOR]           = g_color_set[SET_NO][9];
	}

	return colors[id];
}

const RGBColor&
system_color(SystemColorID color_id)
{
	if(color_id < 0 || NUM_COLORS <= color_id)
	{
		error("system_color(): 不正な色番号が指定されました\n");

		static const RGBColor error_color(255, 255, 255);
		return error_color;
	}

	return system_color_(color_id);
}

void
set_system_color(SystemColorID color_id, const RGBColor& color)
{
	if(color_id < 0 || NUM_COLORS <= color_id)
	{
		error("system_color(): 不正な SystemColorID が指定されました\n");
		return;
	}

	system_color_(color_id) = color;
}


//########################################################
}// namespace ntk
