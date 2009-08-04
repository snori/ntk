#ifndef __TOOLBOX_H__
#define __TOOLBOX_H__


#include <ntk/interface/view.h>
#include <ntk/interface/tabbar.h>


class TimeLine;
class IOBox;
class EditBox;
class PaintBox;
class CompositeBox;
class EffectBox;


class ToolBox : public NView {
public:
	ToolBox(const NRect& frame);
	~ToolBox();

	IOBox* io_box() const {return m_io_box;}
	EditBox* edit_box() const {return m_edit_box;}
	PaintBox* paint_box() const {return m_paint_box;}
	CompositeBox* composite_box() const {return m_composite_box;}
	EffectBox* effect_box() const {return m_effect_box;}

	virtual void attached_to_window();
	virtual void message_received(const NMessage& message);

private:
	NTabBar* m_tab_bar;
	NView* m_active_box;

	IOBox* m_io_box;
	EditBox* m_edit_box;
	PaintBox* m_paint_box;
	CompositeBox* m_composite_box;
	EffectBox* m_effect_box;

	void set_up_controls_();
	void set_up_boxes_(const NRect& frame);

};// class ToolBox


#endif//EOH