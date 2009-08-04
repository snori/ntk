#include "renderview.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <assert.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <ntk/interface/window.h>
#include "application.h"
#include "geom.h"


#define for if(false);else for


//########################################################


render_view_t::render_view_t()
:	ntk_gl_view(ntk_rect(0, 0, 100, 100), "GL-View", FOLLOW_ALL_SIDES, DEFAULT_FLAGS | DOUBLE_BUFFERING)
,	mouse_operation(NO_OP)
,	translation(0, 0, 0)
,	rotation(0, 0)
,	scale(1)
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, &vector4_t(1.f, 1.f, 1.f, 1.f)[0]);
	glMaterialf(GL_FRONT, GL_SHININESS, 10.f);
	glLightfv(GL_LIGHT0, GL_POSITION, &vector4_t(1.f, 1.f, 1.f, 1.f)[0]);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &vector4_t(0.3f, 0.3f, 0.3f, 1.0f)[0]);

//	glEnable(GL_SMOOTH);
//	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glDepthFunc(GL_LEQUAL);
}

void
render_view_t::draw_scene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();

	matrix_t m(matrix_t::UNIT_MATRIX);
	m.rotate_y(-rotation.x);
	m.rotate_x(rotation.y);

	vector3_t camera = vector3_t(0, 0, 10) * m;
	gluLookAt(camera.x, camera.y, camera.z, 0, 0, 0, 0, 1, 0);

	real_t zoom = scale >= 0.f ? 1 / scale : 0.0000000001;
	glScalef(zoom, zoom, zoom);

	vector4_t material_specular(0.2f, 0.2f, 0.2f, 1.0f);

	glPushAttrib(GL_ENABLE_BIT);
	{
		glDisable(GL_LIGHTING);
		draw_grid();
		draw_axis();
	}
	glPopAttrib();

	app()->scene().draw();

	glPopMatrix();
}

void
render_view_t::draw_axis()
{
	glPushAttrib(GL_CURRENT_BIT);

	const struct{
		GLfloat color[3];
		GLfloat p1[3], p2[3];
	}
	DATA[] = {
		{{1, 0.5f, 0.5f}, {-10, 0, 0}, {10, 0, 0}},
		{{0.5f, 1, 0.5f}, {0, -10, 0}, {0, 10, 0}},
		{{0.5f, 0.5f, 1}, {0, 0, -10}, {0, 0, 10}}
	};

	glBegin(GL_LINES);
	for(int i=0; i<3; ++i)
	{
		glColor3f(DATA[i].color[0], DATA[i].color[1], DATA[i].color[2]);
		glVertex3f(DATA[i].p1[0], DATA[i].p1[1], DATA[i].p1[2]);
		glVertex3f(DATA[i].p2[0], DATA[i].p2[1], DATA[i].p2[2]);
	}
	glEnd();

	glPopAttrib();
}

void
render_view_t::draw_grid()
{
	glPushAttrib(GL_CURRENT_BIT);

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	{
		for(int i=-10; i<=10; ++i)
		{
			if(i == 0)
				continue;

			glVertex3f(i, 0, 10);
			glVertex3f(i, 0, -10);
		}
		for(int i=-10; i<=10; ++i)
		{
			if(i == 0)
				continue;

			glVertex3f(10, 0, i);
			glVertex3f(-10, 0, i);
		}
	}
	glEnd();

	glPopAttrib();
}

void
render_view_t::attached_to_window()
{
	move_to(0, 0);
	resize_to(window()->frame().width(), window()->frame().height());
}

void
render_view_t::mouse_down(const ntk_point& point, uint buttons, bool)
{
	if(buttons == (ntk::RIGHT_MOUSE_BUTTON | ntk::MIDDLE_MOUSE_BUTTON))
		mouse_operation = SCALE_OP;
	else if(buttons == ntk::RIGHT_MOUSE_BUTTON)
		mouse_operation = ROTATE_OP;
	else if(buttons == ntk::MIDDLE_MOUSE_BUTTON)
		mouse_operation = TRANSLATE_OP;

	prev_point = point;
	begin_capture();
}

void
render_view_t::mouse_up(const ntk_point& point, uint buttons)
{
	mouse_operation = NO_OP;

	if(is_capturing())
		end_capture();
}

void
render_view_t::mouse_moved(const ntk_point& point, uint buttons, uint transit, const ntk_message* data)
{
	if(mouse_operation != NO_OP)
		camera_op_(point);
}

void
render_view_t::lost_capture()
{
//	mouse_operation = NO_OP;
}

void
render_view_t::camera_op_(const ntk_point& point)
{
	switch(mouse_operation)
	{
	case TRANSLATE_OP:
		translation.x += (point - prev_point).x;
		translation.y += (point - prev_point).y;
		translation.z = translation.z;
		break;

	case ROTATE_OP:
		rotation.x += (point - prev_point).x;
		rotation.y -= (point - prev_point).y;
		if(rotation.y >  90) rotation.y = 89.9999f;
		if(rotation.y < -90) rotation.y = -89.9999f;
		break;

	case SCALE_OP:
		scale += (point - prev_point).y / 10.f;
		break;
	}

	prev_point = point;
	invalidate();
}
