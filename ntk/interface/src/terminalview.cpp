/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: terminalview.cpp,v 1.5 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/terminalview.h"


#include <tchar.h>
#include <vector>
#include <ntk/windows/windows.h>
#include <ntk/application/application.h>
#include <ntk/interface/window.h>
#include <ntk/storage/directory.h>
#include <ntk/support/debug.h>


namespace ntk {


//########################################################


namespace {

/*** messages ***/
enum {
	FLUSH_CURSOR = '_IFC',//Interface: Flush Cursor
	PROCESS_END = '_IPE'//Interface: Process End
};

}// anonymous namespace


//########################################################
// public methods

TerminalView::TerminalView(const Rect& frame, const String& name, uint mode, uint flags, const RGBColor& color)
:	Control(frame, name, name, NULL, mode, flags | WILL_DRAW, transparent_color())
,	m_buffer(1, '\0')
,	m_cursor(0)
,	m_num_lines(1)
,	m_max_line(0)
,	m_flash_interval(1000)
,	m_flash_timer(0)
,	m_thread_stop_flag(false)
,	m_flash_cursor_thread(NULL)
,	m_process_thread(NULL)
,	m_escape_sequence(true)
,	m_text_color(black_color())
,	m_cursor_color(black_color())
,	m_background_color(color)
{
	print_prompt();

	m_flash_cursor_thread = new Thread(flash_cursor_proc_, "FlashCursorThread", NORMAL_PRIORITY, this);
	m_flash_cursor_thread->run();
}

TerminalView::~TerminalView()
{
	m_thread_stop_flag = true;// 終了フラグ

	if(m_flash_cursor_thread->is_suspended())
		m_flash_cursor_thread->resume();

	m_flash_cursor_thread->wait_for();

	delete m_flash_cursor_thread;
}

void
TerminalView::execute_command_line(const String& command_line)
{
	if(command_line == "exit")
	{
		application()->quit();
	}
	else if(command_line != "")
	{
		put_char('\n');

		// 最初の空白以外の文字を見つける
		const char_t* first_char = command_line.c_str();
		while(*first_char == ' ' || *first_char == '\t')
			++first_char;

		if(*first_char == 'x' || *first_char == 'X')
		{
			++first_char;// 'x' を除く
			m_tmp_command_line = String("cmd /c ") + first_char;
		}
		else
			m_tmp_command_line = command_line;

		m_process_thread = new Thread(process_proc_, "Process", NORMAL_PRIORITY, this);
		m_process_thread->run();
	}
	else
	{
		put_char('\n');
		print_prompt();
	}
}

void
TerminalView::print(const char_t* str)
{
	Buffer buf;

	// 文字の最初のバックスペース文字は特別処理
	while(str[0] == '\b')
	{
		++str;
		back_char();
	}

	for(const char_t* ch=str; *ch != '\0'; ++ch)
	{
#if 0
		if(m_escape_sequence && *ch == '\\')
		{
			++ch;
			if(*ch == 'n' || *ch == 'N')
				buf.push_back('\n');
			else if(*ch == 't' || *ch == 'T')
				buf.push_back('\t');
			else if(*ch == 'b' || *ch == 'B')
				buf.pop_back();
			else if(*ch == '\\')
				buf.push_back('\\');
			else
			{
				error(String("\"\\") + *ch + "\": 不正なエスケープシーケンスです\n");
//				buf.push_back('\\');
//				buf.push_back(*ch);
			}
		}
		else buf.push_back(*ch);
#else
		if(*ch == '\b')
		{
			if(buf.size() > 0)
				buf.pop_back();
		}
		else buf.push_back(*ch);
#endif
	}

	put_string(&buf[0], buf.size());
}

void
TerminalView::put_string(const char_t* buf, int size)
{
	if(size < 0)
		size = _tcslen(buf);

	m_buffer.insert(m_buffer.begin() + m_cursor, buf, buf + size);

	m_cursor += size;

	if(m_max_line > 0)
	{
		const char_t* p = buf;
		while(*p++ != '\0')
			if(*p == '\n')
				++m_num_lines;

		if(m_num_lines > m_max_line)
			line_overflow_();
	}

	reset_flash_timer_(505);
}

void
TerminalView::put_char(char_t ch)
{
	m_buffer.insert(m_buffer.begin() + m_cursor, ch);

	m_cursor += 1;

	if(m_max_line > 0 && ch == '\n')
	{
		++m_num_lines;

		if(m_num_lines > m_max_line)
			line_overflow_();
	}

	reset_flash_timer_(505);
}

void
TerminalView::back_char()
{
	if(m_cursor <= 0) return;

	m_buffer.erase(m_buffer.begin() + m_cursor -1);

	m_cursor -= 1;

	reset_flash_timer_();
}

void
TerminalView::del_char()
{
	if(m_cursor >= m_buffer.size() -1) return;// -1: '\0'の分

	m_buffer.erase(m_buffer.begin() + m_cursor);

	reset_flash_timer_();
}

void
TerminalView::print_prompt()
{//m_buffer.clear();m_num_lines = 1;m_buffer.push_back('\0');m_cursor = 0;
//	print(Nlib::current_directory() + "\b>");
	print(">");
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

uint
TerminalView::num_lines() const
{
	return m_num_lines;
}

uint
TerminalView::max_line() const
{
	return m_max_line;
}

void
TerminalView::set_max_line(uint num)
{
	m_max_line = num;

	if(m_max_line > 0 && m_num_lines > m_max_line)
		line_overflow_();
}

const RGBColor&
TerminalView::text_color() const
{
	return m_text_color;
}

void
TerminalView::set_text_color(const RGBColor& color)
{
	m_text_color = color;
}

const RGBColor&
TerminalView::cursor_color() const
{
	return m_cursor_color;
}

void
TerminalView::set_cursor_color(const RGBColor& color)
{
	m_cursor_color = color;
}

const RGBColor&
TerminalView::view_color() const
{
	return m_background_color;
}

void
TerminalView::set_view_color(const RGBColor& color)
{
	m_background_color = color;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public message handlers

void
TerminalView::draw(PaintDC& dc)
{
	m_paint_locker.lock();

	Rect rect = bounds();

	MemoryDC mem_dc(dc);
	mem_dc.set_bitmap(DDB(dc, rect.width(), rect.height()));

	mem_dc.set_brush(m_background_color);
	mem_dc.fill_rect(rect);

	draw_content(mem_dc);

	static const Point ZERO_POINT(0, 0);
	dc.bit_blt(bounds(), mem_dc, ZERO_POINT, SRCCOPY);

	m_paint_locker.unlock();
}

void
TerminalView::draw_content(DC& dc)
{
	enum {LEFT_SPACE = 2};

	const coord LINE_HEIGHT = line_height(dc);
	coord cursor_x = 0;
	int32 cursor_line = 0;

	dc.set_text_color(m_text_color);

	int num_lines = 0, num_chars = 0;
	Buffer::iterator line_head = m_buffer.begin();
	for(Buffer::iterator it = m_buffer.begin(); it != m_buffer.end(); ++it)
	{
		if(*it == '\n' || *it == '\0')
		{
			// カーソルがこの行にあったらカーソルのｘ座標を得る
			if(line_head - m_buffer.begin() <= m_cursor && m_cursor <= it - m_buffer.begin())
			{
				Rect rect(0, 0, 0, 0);
				DrawText(
					dc.hdc(),
					reinterpret_cast<char_t*>(&*line_head),
					num_chars,
					&rect,
					DT_LEFT | DT_EXPANDTABS | DT_CALCRECT);

				cursor_x = rect.width() + LEFT_SPACE;
				cursor_line = num_lines;
			}

			if(num_chars > 0)
				DrawText(
					dc.hdc(),
					reinterpret_cast<char_t*>(&*line_head),
					num_chars,
					&Rect(
						LEFT_SPACE,
						num_lines * LINE_HEIGHT +1,
						frame().width(),
						32767),
					DT_LEFT | DT_EXPANDTABS);// tab width = 4 | DT_TABSTOP | (4<<8)

			++num_lines;
			num_chars = 0;
			line_head = it +1;
		}
		else ++num_chars;
	}

	// draw cursor
	m_flash_timer_locker.lock();
	bigtime_t time = m_flash_timer;
	m_flash_timer_locker.unlock();

	if(
		m_flash_interval /2 <= time &&
		time < m_flash_interval &&// もし、インターバルの後半だったら光らせる
		window()->is_active() && is_focus())// カーソルが光るのはフォーカスを持っている時のみ
	{
		dc.set_brush(m_cursor_color);
		dc.fill_rect(
			cursor_x,
			(cursor_line +1) * LINE_HEIGHT -2,
			cursor_x + dc.string_width("A"),
			(cursor_line +1) * LINE_HEIGHT);
	}
}

void
TerminalView::focus_changed(bool state)
{
	if(state && m_flash_cursor_thread->is_suspended())
		m_flash_cursor_thread->resume();
	else if(state == false && m_flash_cursor_thread->is_suspended() == false)
		m_flash_cursor_thread->suspend();

	reset_flash_timer_();
}

void
TerminalView::char_received(uint code, uint repeat)
{
	if(m_process_thread) return;

	if(
		code == '\n' ||
		code == '\r' ||
		code == '\t' ||
		code == '\b' ||
		code == '\v' ||
		code == 0x1b)// escape
		return;

	put_char((char_t)code);
}

void
TerminalView::key_down(uint code, uint repeat)
{
	if(m_process_thread) return;

	if(code == VK_RETURN)
	{
		char_t* command_head = NULL;
		for(int i=m_buffer.size() -1; m_buffer[i] != '\n' || i >= 0; --i)
			if(m_buffer[i] == '>')
			{
				command_head = &m_buffer[i +1];
				break;
			}

		execute_command_line(command_head);
	}
	else if(code == VK_BACK)
	{
		if(m_buffer[m_cursor -1] != '>')
			back_char();
	}
	else if(code == VK_DELETE)
		del_char();
	else if(code == VK_TAB)
		put_char('\t');
}

void
TerminalView::attached_to_window()
{
	Control::attached_to_window();

	make_focus();
}

void
TerminalView::message_received(const Message& message)
{
	switch(message.what)
	{
	case FLUSH_CURSOR:
		invalidate();
		break;

	case PROCESS_END:
		put_char('\n');
		print_prompt();
		break;

	default:
		Control::message_received(message);
	}
}

//########################################################

coord
TerminalView::line_height(DC& dc) const
{
	return dc.string_height() + 2;
}

//########################################################

void
TerminalView::reset_flash_timer_(bigtime_t new_time)
{
	m_flash_timer_locker.lock();
	m_flash_timer = new_time;
	m_flash_timer_locker.unlock();

	invalidate();
}

void
TerminalView::clean_up_process_()
{
	delete m_process_thread;
	m_process_thread = NULL;

	Messenger(this).send_message(Message(PROCESS_END));
}

void
TerminalView::line_overflow_()
{
	if(m_max_line <= 0)
		return;

	if(m_num_lines > m_max_line)
	{
		Buffer::iterator it = m_buffer.begin();

		int num_lines = m_num_lines - m_max_line;
		while(num_lines)
		{
			if(*it++ == '\n')
				--num_lines;
		}

		m_buffer.erase(m_buffer.begin(), it);
	}
}

//########################################################

uint
TerminalView::flash_cursor_proc_(void* data)
{
	TerminalView* self = reinterpret_cast<TerminalView*>(data);

	bigtime_t time = 0;
	while(! self->m_thread_stop_flag)
	{
		self->m_flash_timer_locker.lock();
		self->m_flash_timer += self->m_flash_interval /2;
		time = self->m_flash_timer;
		self->m_flash_timer_locker.unlock();

		if(time >= self->m_flash_interval)
		{
			self->m_flash_timer_locker.lock();
			self->m_flash_timer = 0;
			self->m_flash_timer_locker.unlock();
		}

		Messenger msger(self);
		if(msger.is_valid())
			msger.send_message(Message(FLUSH_CURSOR));

		self->m_flash_cursor_thread->snooze(self->m_flash_interval /2);
	}

	return 0;
}

uint
TerminalView::terminal_proc_(void* data)
{
	return  true;
}

uint
TerminalView::process_proc_(void* data)
{
	TerminalView* self = reinterpret_cast<TerminalView*>(data);

	HANDLE hread, hwrite;

	SECURITY_ATTRIBUTES security_attr;
	security_attr.nLength              = sizeof(SECURITY_ATTRIBUTES);
	security_attr.lpSecurityDescriptor = NULL;
	security_attr.bInheritHandle       = TRUE;

	if(! CreatePipe(&hread, &hwrite, &security_attr, 0))
	{
		self->print(last_error_message());
		self->clean_up_process_();
		return false;
	}

	PROCESS_INFORMATION process_info;
	STARTUPINFO start_info;
	memset(&start_info, 0, sizeof(STARTUPINFO));
	start_info.cb						= sizeof(STARTUPINFO);
	start_info.dwFlags			= STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
	start_info.wShowWindow	= SW_HIDE;
	start_info.hStdOutput		= hwrite;
	start_info.hStdError		= hwrite;

	if(!
		CreateProcess(
			NULL,
			const_cast<char*>(self->m_tmp_command_line.c_str()),
			NULL,
			NULL,
			true,
			NORMAL_PRIORITY_CLASS | DETACHED_PROCESS,
			NULL,
			NULL,
			&start_info,
			&process_info))
	{
		self->print(last_error_message());
		self->clean_up_process_();
		return false;
	}

	snooze(30);
	CloseHandle(hwrite);

	enum {BUF_SIZE = 1024};
	char buf[BUF_SIZE];
	ulong num_bytes;

	bool backup_es = self->m_escape_sequence;
	self->m_escape_sequence = false;

	while(1)
	{
		if(! ReadFile(hread, buf, BUF_SIZE -1, &num_bytes, NULL) || !num_bytes)
			break;
		else
		{
			buf[num_bytes] = '\0';
			self->print(buf);
		}
	}

	self->m_escape_sequence = backup_es;

	CloseHandle(hread);

	self->clean_up_process_();

	return true;
}


//########################################################
}// namespace ntk
