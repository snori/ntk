/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: glview.cpp,v 1.7 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/glview.h"


#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#include <tchar.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>
#include <ntk/support/debug.h>


namespace ntk {


//########################################################


String
GLView::status::default_message(int what)
{
	switch(what)
	{
	case ALREADY_INITIALIZED:
		return _T("OpenGL はすでに初期化されています\n");

	case FAILED_TO_SET_PIXEL_FORMAT:
		return _T("SetPixelFormat に失敗しました\n");

	default:
		return st::default_message(what);
	}
}


//########################################################
// public methods

GLView::GLView(const Rect& frame, const String& name, uint mode, uint flags, const RGBColor& color, uint style, uint ex_style)
:	View(frame, name, mode, flags, color, style, ex_style)
,	m_dc(NULL)
,	m_rc(NULL)
{
	initialize_();
}

GLView::~GLView()
{
	finalize_();
}

void
GLView::draw(PaintDC& dc)
{
	if(m_rc == NULL) return;

	wglMakeCurrent(*m_dc, m_rc);

//	glPushMatrix();
	draw_scene();
//	glPopMatrix();

	glFinish();

	if(flags() & DOUBLE_BUFFERING)
		SwapBuffers(dc);
}

void
GLView::draw_scene()
{
}

void
GLView::attached_to_window()
{
	resize_by(0, 1);
	resize_by(0, -1);
}

void
GLView::frame_resized(coord width, coord height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(40.f, (float)width / (float)height, .1f, 2000.f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	invalidate();// ?
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

HGLRC
GLView::rc() const
{
	return m_rc;
}

//********************************************************
// private functions

GLView::status_t
GLView::initialize_()
{
	finalize_();

	if(m_rc)
		return status::ALREADY_INITIALIZED;

	PIXELFORMATDESCRIPTOR pixel_format_desc = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	if(flags() & DOUBLE_BUFFERING)
		pixel_format_desc.dwFlags |= PFD_DOUBLEBUFFER;
//	if(flags() & 

	m_dc = new DC(hwnd());
	int pixel_format = ChoosePixelFormat(*m_dc, &pixel_format_desc);
	int status = SetPixelFormat(*m_dc, pixel_format, &pixel_format_desc);

	if(! status)
	{
		delete m_dc;
		m_dc = NULL;

		return status::FAILED_TO_SET_PIXEL_FORMAT;
	}

	m_rc = wglCreateContext(*m_dc);
	wglMakeCurrent(*m_dc, m_rc);

//	glFlush();

	return st::NO_ERR;
}

void
GLView::finalize_()
{
	if(m_dc == NULL || m_rc == NULL) return;

	wglMakeCurrent(*m_dc, NULL);
	wglDeleteContext(m_rc);

	delete m_dc;

	m_dc = NULL;
	m_rc = NULL;
}


//###############################################################
}// namespace ntk


#if 0

/*
 *　ワイヤフレーム（OpenGL 関数の呼び出し例）
 */
void
OpenGL::render_example()
{
	static float _point[][3] =
	{
		{ -1.F, -1.F, -1.F },
		{ -1.F, -1.F,  1.F },
		{  1.F, -1.F,  1.F },
		{  1.F, -1.F, -1.F },
		{ -1.F,  1.F, -1.F },
		{ -1.F,  1.F,  1.F },
		{  1.F,  1.F,  1.F },
		{  1.F,  1.F, -1.F }
	};

	static int _ipoint[] =
	{
			2, 1, 0, 3, -1,
			7, 4, 5, 6, -1,
			6, 5, 1, 2, -1,
			4, 7, 3, 0, -1,
			5, 4, 0, 1, -1,
			7, 6, 2, 3, -1
	};

	m_is_wire = 1;// redraw() のときのフラグ

	wglMakeCurrent(m_image->dc(), m_rc);

	// 一般的な設定
	glClearColor(0., 0., 0., 0.);
	glClear(GL_COLOR_BUFFER_BIT);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// カメラの設定
	glViewport(0, 0, 640, 480);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// OpenGL ではスクリーンと前方クリッピング面が一致する
	float pers_length = 5.;// 視点からスクリーンまでの距離
	float near_plane = 0.01F;// 視点から前方クリッピング面までの距離
	float f = near_plane / pers_length;
	glFrustum(f * -1.33, f * 1.33, f * -1., f * 1., near_plane, 100.);
	gluLookAt(3., 2., 10., 0., 0., 0., 0., 1., 0.);

	// 形状の設定
	glMatrixMode(GL_MODELVIEW);

	// ポリゴンの描画
	glPushMatrix();
	glTranslatef(-1.5, 0., 0.);
	glColor3f(1., 1., 1.);
	int iipoint = 0;

	for(int ipolygon = 0; ipolygon < 6; ++ipolygon)
	{
		glBegin(GL_POLYGON);

		for(int ivertex = 0; ivertex < 4; ivertex++)
		{
			glVertex3fv(_point[_ipoint[iipoint++]]);
		}

		glEnd();
		iipoint++;		// -1 の分を飛ばす．
	}

	glPopMatrix();

	// 球の描画
	glPushMatrix();
	glTranslatef(1.5, 0., 0.);
	glRotatef(90., 1., 0., 0.);
	glColor3f(1., 1., 1.);
	GLUquadricObj* qobj = gluNewQuadric();
	gluSphere(qobj, 1., 36, 18);
	gluDeleteQuadric(qobj);
	glPopMatrix();

	glFinish();

//	BitBlt(m_image->dc(), 0, 0, m_image->width(), m_image->height(), _hdc, 0, 0, SRCCOPY);
}

#endif
