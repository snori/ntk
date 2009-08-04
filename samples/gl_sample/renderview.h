#ifndef __RENDER_VIEW_H__
#define __RENDER_VIEW_H__


#include <ntk/interface/glview.h>
#include "geom.h"


class render_view_t : public ntk_gl_view {
public:
	render_view_t();

	void draw_scene();
	void draw_axis();
	void draw_grid();

	void attached_to_window();
	void mouse_down(const ntk_point& point, uint buttons, bool);
	void mouse_up(const ntk_point& point, uint buttons);
	void mouse_moved(const ntk_point& point, uint buttons, uint transit, const ntk_message* data);
	void lost_capture();

private:
	enum mouse_operation_t {
		NO_OP,
		TRANSLATE_OP,
		ROTATE_OP,
		SCALE_OP,
	};

	mouse_operation_t mouse_operation;
	ntk_point prev_point;

	vector3_t translation;
	vector2_t rotation;
	real_t scale;

	void camera_op_(const ntk_point& point);

};// class render_view_t


#endif//EOH