#ifndef __TOYOZPAINT_H__
#define __TOYOZPAINT_H__


#include <ntk/application/application.h>


class application_t : public ntk_application {
public:
	//
	// methods
	//
	application_t();

	//
	// message handlers
	//
	void about_requested();
	void message_received(const ntk_message& message);

};// class application_t


inline application_t* app() {return static_cast<application_t*>(ntk::application());}


#endif//EOH