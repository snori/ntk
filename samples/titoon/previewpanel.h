#ifndef __PREVIEWPANEL_H__
#define __PREVIEWPANEL_H__


#include <ntk/interface/window.h>
#include <ntk/interface/panel.h>


class Animation;


class PreviewPanel : public NPanel {
public:
	//
	// methods
	//
	PreviewPanel(const NString& name);

	void play(Animation* animation);
	void stop();
	void pause();

private:
	HANDLE m_play_thread;
	bool m_stop, m_pause;

	Animation* m_animation;

	static uint play_anim_(void*);

};// class PreviewPanel


class PreviewWindow : public NWindow {
public:
	PreviewWindow(const NRect& frame);
	virtual ~PreviewWindow();

	void play(Animation* animation);

	virtual bool quit_requested();

private:
	PreviewPanel* m_preview;

};// class PreviewWindow


#endif//EOH