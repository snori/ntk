#ifndef __TED_H__
#define __TED_H__


#include <ntk/application/application.h>
#include <ntk/support/setting.h>


class ted_application_t : public ntk_application {
public:
	//
	// methods
	//
	ted_application_t();
	~ted_application_t();

	ntk_setting& conf() {return m_conf;}
	const ntk_setting& conf() const {return m_conf;}

	//
	// message handlers
	//
	void about_requested();

private:
	//
	// data
	//
	static ntk_setting m_conf;

};// class ted_application_t


inline ted_application_t* ted()
	{return reinterpret_cast<ted_application_t*>(ntk::application());}


#endif//EOH