/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: alert.h,v 1.6 2003/11/11 12:07:06 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_INTERFACE_ALERT_H__
#define __NTK_INTERFACE_ALERT_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_INTERFACE_WINDOW_H__
#include <ntk/interface/window.h>
#endif


namespace ntk {


class Invoker;
class Button;
class StringView;

class Alert : public Window {
public:
	//
	// constants
	//
	enum ButtonWidth {
		WIDTH_AS_USUAL,
		WIDTH_FROM_LABEL,
		WIDTH_FROM_WIDEST,
	};

	enum AlertType {
		EMPTY_ALERT,
		INFO_ALERT,
		IDEA_ALERT,
		WARNING_ALERT,
		ATOP_ALERT,
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef ButtonWidth button_width_t;
		typedef AlertType alert_type_t;
	#endif

	//
	// methods
	//
	NtkExport Alert(
		const String& title,
		const String& text,
		const String& button0_label = "OK",
		const String& button1_label = "",
		const String& button2_label = "",
		ButtonWidth button_width = WIDTH_AS_USUAL,
		AlertType alert_type = INFO_ALERT);
	NtkExport virtual ~Alert();

	NtkExport virtual int go();
	NtkExport virtual status_t go(Invoker* invoker);

	//
	// accessors
	//
	NtkExport virtual Button* button0();
	NtkExport virtual const Button* button0() const;
	NtkExport virtual Button* button1();
	NtkExport virtual const Button* button1() const;
	NtkExport virtual Button* button2();
	NtkExport virtual const Button* button2() const;

	//
	// message handlers
	//
	NtkExport virtual bool quit_requested();
	NtkExport virtual void message_received(const Message& message);

private:
	//
	// data
	//
	StringView* m_text;
	Button *m_button0, *m_button1, *m_button2;
	Invoker* m_invoker;
	int* m_result_which;

	//
	// functions
	//
	void button_pushed_(int which);

};// class Alert


NtkExport int alert(
	const String& title,
	const String& text,
	const String& button0_label = "OK",
	const String& button1_label = "",
	const String& button2_label = "",
	Alert::ButtonWidth button_width = Alert::WIDTH_AS_USUAL,
	Alert::AlertType alert_type = Alert::INFO_ALERT);


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Alert alert_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Alert NAlert;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Alert ntk_alert;
	#endif
#endif


#endif//EOH
