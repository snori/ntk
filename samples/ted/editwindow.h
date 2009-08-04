#ifndef __EDITWINDOW_H__
#define __EDITWINDOW_H__


#include <ntk/interface/window.h>
#include <ntk/interface/scintilla.h>


class edit_window_t : public ntk_window {
public:
	//
	// methods
	//
	edit_window_t();
	~edit_window_t();

	bool quit_requested();
	void message_received(const ntk_message& message);
	LRESULT system_command_received(uint id);

private:
	//
	// data
	//
	ntk_string m_file_name;
	ntk_scintilla* m_edit;

	//
	// functions
	//
	void load_file_(const ntk_string& file_name);
	void save_file_(const ntk_string& file_name, bool over_write = false);

};// class edit_window_t


#endif//EOH