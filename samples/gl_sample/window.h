#ifndef __WINDOW_H__
#define __WINDOW_H__


#include <ntk/interface/window.h>


class gl_window_t : public ntk_window {
public:
	gl_window_t();
	~gl_window_t();

	bool quit_requested();
	void message_received(const ntk_message& message);
	LRESULT system_command_received(uint command);
};


#endif//EOH