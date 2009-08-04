#ifndef __DEMO_H__
#define __DEMO_H__


#include <ntk/application/application.h>


class demo_window_t;
class demo_screen_t;


class demo_t : public ntk_application {
public:
	demo_t(const ntk_string& name, const ntk_version& version = ntk_version(1));

	virtual HANDLE run();

	bool quit_flag() const;
	demo_screen_t* screen() const {return m_screen;}

	static demo_t* instance() {return (demo_t*)ntk::application();}

private:
	demo_window_t* m_window;
	demo_screen_t* m_screen;
};


#endif//EOH