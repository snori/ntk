#ifndef __NEWDOCUMENTDIALOG_H__
#define __NEWDOCUMENTDIALOG_H__


#include <ntk/interface/window.h>


class NewDocumentDialog : public NWindow {
public:
	//
	// methods
	//
	NewDocumentDialog();
	LRESULT system_command_received(uint command);

};//class NewDocumentDialog


#endif//EOH