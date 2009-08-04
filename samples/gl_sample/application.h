#ifndef __APPLICATION_H__
#define __APPLICATION_H__


#include <ntk/application/application.h>
#include <ntk/support/setting.h>
#include "geom.h"


class gl_application_t : public ntk_application {
public:
	gl_application_t();

	scene_t& scene() {return m_scene;}
	const scene_t& scene() const {return m_scene;}

	static ntk_setting& conf() {return g_conf;}

	void about_requested();

private:
	scene_t m_scene;

	static ntk_setting g_conf;
};


inline gl_application_t* app()
	{return reinterpret_cast<gl_application_t*>(ntk::application());}


#endif//EOH