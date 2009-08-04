#ifndef __TITOONWINDOW_H__
#define __TITOONWINDOW_H__


#include <ntk/interface/window.h>


class Animation;
class Canvas;
class TimeControl;
class ToolBox;


class TitoonWindow : public NWindow {
public:
	TitoonWindow(const NRect& frame);
	virtual ~TitoonWindow();

	Animation* animation() const {return m_animation;}
	void set_animation(Animation* anim);

	Canvas* canvas() const {return m_canvas;}
	TimeControl* time_control() const {return m_time_control;}
	ToolBox* tool_box() const {return m_tool_box;}

	void invalidate();

	void zoom() {NWindow::zoom();}
	virtual void zoom(const NPoint& origin, coord w, coord h);
	virtual bool quit_requested();
	virtual void message_received(const NMessage& message);
	virtual LRESULT system_command_received(uint command);

private:
	Animation* m_animation;

	Canvas* m_canvas;
	TimeControl* m_time_control;
	ToolBox* m_tool_box;

	NRect m_frame_before_zoom;

	void set_up_controls_();
	void reset_title_();

	void cut_(int frame);
	void copy_(int frame);
	void paste_(int frame);

};// class TitoonWindow


#endif//EOH