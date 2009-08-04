/******************************************************************************

	The NTK Library : samples/FireSim
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: firesim.h,v 1.2 2003/08/04 12:01:20 nsuzuki Exp $

******************************************************************************/
#ifndef __FIRESIM_H__
#define __FIRESIM_H__

#include <ntk/application/application.h>

class Scene;
class PreviewPanel;

class App : public ntk::Application {
public:
	App();
	int message_loop();

	void idle();

	Scene* scene() const {return m_scene;}
	PreviewPanel* preview_panel() const {return m_preview_panel;}

private:
	Scene* m_scene;
	PreviewPanel* m_preview_panel;
};

inline App* app() {return (App*)ntk::application();}

#endif//EOH