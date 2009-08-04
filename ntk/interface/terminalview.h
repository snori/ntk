/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: terminalview.h,v 1.4 2003/11/11 12:07:07 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_TERMINALVIEW_H__
#define __NTK_INTERFACE_TERMINALVIEW_H__


#include <vector>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_CONTROL_H__
#include <ntk/interface/control.h>
#endif

#ifndef __NTK_INTERFACE_DC_H__
#include <ntk/interface/dc.h>
#endif

#ifndef __NTK_KERNEL_THREAD_H__
#include <ntk/kernel/thread.h>
#endif

#ifndef __NTK_KERNEL_LOCKER_H__
#include <ntk/kernel/locker.h>
#endif


namespace ntk {


class TerminalView : public Control {
public:
	//
	// constants
	//
	enum {
//		CHARA_FLAG = Control::FLAG_LAST << 1,
		FLAG_LAST	= Control::FLAG_LAST << 2,
	};

	enum {
		DEFAULT_RESIZING_MODE = FOLLOW_LEFT | FOLLOW_TOP,
		DEFAULT_FLAGS = NAVIGABLE /* | CHARA_FLAG*/,
	};

	//
	// methods
	//
	NtkExport TerminalView(
		const Rect& frame,
		const String& name,
		uint mode = DEFAULT_RESIZING_MODE,
		uint flags = DEFAULT_FLAGS,
		const RGBColor& color = ntk::view_color());
	NtkExport virtual ~TerminalView();

	NtkExport virtual void execute_command_line(const String& command_line);

	void print(const String& str) {print(str.c_str());}
	NtkExport virtual void print(const char_t* str);
	NtkExport virtual void put_string(const char_t* buf, int size = -1);// -1: '\0'‚ÅI‚í‚é•¶Žš—ñ
	NtkExport virtual void put_char(char_t ch);
	NtkExport virtual void back_char();
	NtkExport virtual void del_char();

	NtkExport virtual void print_prompt();

	//
	// accessors and manipulators
	//
	NtkExport virtual uint num_lines() const;
	NtkExport virtual uint max_line() const;
	NtkExport virtual void set_max_line(uint num = 0);// 0: Å‘å’l‚ðÝ’è‚µ‚È‚¢

	NtkExport virtual const RGBColor& text_color() const;
	NtkExport virtual void set_text_color(const RGBColor& color);

	NtkExport virtual const RGBColor& cursor_color() const;
	NtkExport virtual void set_cursor_color(const RGBColor& color);

	NtkExport virtual const RGBColor& view_color() const;
	NtkExport virtual void set_view_color(const RGBColor& color);

	//
	// message handlers
	//
	NtkExport virtual void draw(PaintDC& dc);
	NtkExport virtual void draw_content(DC& dc);
	NtkExport virtual void focus_changed(bool state);
	NtkExport virtual void char_received(uint char_code, uint repeat);
	NtkExport virtual void key_down(uint key_code, uint repeat);
	NtkExport virtual void attached_to_window();
	NtkExport virtual void message_received(const Message& message);

protected:
	//
	// functions
	//
	coord line_height(DC& dc) const;

private:
	//
	// types
	//
	typedef std::vector<char_t> Buffer;

	//
	// data
	//
	Buffer m_buffer;
	uint m_cursor;
	uint m_num_lines, m_max_line;

	bigtime_t m_flash_interval, m_flash_timer;
	Locker m_flash_timer_locker, m_paint_locker;

	bool m_thread_stop_flag;
	Thread* m_flash_cursor_thread;

	Thread* m_process_thread;
	String m_tmp_command_line;

	bool m_escape_sequence;

	// drawing states
	RGBColor m_text_color, m_cursor_color, m_background_color;

	//
	// functions
	//
	void reset_flash_timer_(bigtime_t new_time = 0);
	void clean_up_process_();
	void line_overflow_();

	static uint flash_cursor_proc_(void*);
	static uint terminal_proc_(void*);
	static uint process_proc_(void*);

};// class TerminalView


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef TerminalView terminal_view_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::TerminalView NTerminalView;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::TerminalView ntk_terminal_view;
	#endif
#endif


#endif//EOH
