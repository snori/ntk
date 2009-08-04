#ifndef __TITOON_H__
#define __TITOON_H__


#include <ntk/application/application.h>
#include <ntk/support/setting.h>


class TitoonWindow;


class Titoon : public NApplication {
public:
	Titoon();
	~Titoon();

	TitoonWindow* window() const {return m_titoon_window;}

	NSetting& conf() {return m_conf;}
	const NSetting& conf() const {return m_conf;}

private:
	TitoonWindow* m_titoon_window;

	static NSetting m_conf;

};// class Titoon


inline Titoon* titoon() {return (Titoon*)ntk::application();}


#endif//EOH