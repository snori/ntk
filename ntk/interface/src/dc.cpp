/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dc.cpp,v 1.7 2003/11/17 09:59:30 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/dc.h"


#include <assert.h>
#include <stack>
#include <boost/scoped_ptr.hpp>
#include <ntk/interface/Bitmap.h>
#include <ntk/support/status.h>


namespace ntk {


//########################################################


static const DDB&
default_bitmap_()
{
	static boost::scoped_ptr<DDB> bitmap;

	if(! bitmap)
		bitmap.reset(new DDB(0, 0, 1, 8, NULL));

	assert(bitmap->handle());

	return *bitmap;
}


//########################################################


struct DCState {
	//
	// constants
	//
	enum Flag {
		NON_STACKABLE,
		FULL_STATE,

		ONLY_ONE_OBJECT_BEGIN,
		FONT_ONLY,
		PEN_ONLY,
		BRUSH_ONLY,
		BITMAP_ONLY,
		DRAWING_MODE_ONLY,
		TEXT_COLOR_ONLY,
		BACK_COLOR_ONLY,
		BACK_MODE_ONLY,
		TEXT_ALIGN_ONLY,
	};

	enum TextColor {TEXT_COLOR};
	enum BackColor {BACK_COLOR};
	enum BackMode  {BACK_MODE};
	enum TextAlign {TEXT_ALIGN};

	Flag flag;

	Font font;
	Pen pen;
	Brush brush;
	DDB bitmap;

	DC::DrawingMode drawing_mode;
	RGBColor text_color, back_color;
	int back_mode;
	uint text_align;

	DCState()
	:	flag(NON_STACKABLE)
	,	font(NULL, false)
	,	pen(NULL)
	,	brush(NULL)
	,	bitmap(NULL)
	,	drawing_mode(DC::OP_COPY)
	,	text_color(font_color())
	,	back_color(white_color())
	,	back_mode(0)
	,	text_align(0)// 最後の2つ、要再考
	{}
	DCState(
		Font f,
		Pen p,
		Brush b,
		DDB bmp,
		DC::DrawingMode draw_mode,
		const RGBColor& txt_col,
		const RGBColor& bak_col,
		int bk_mode,
		uint txt_align)
	:	flag(FULL_STATE)
	,	font(f)
	,	pen(p)
	,	brush(b)
	,	bitmap(bmp)
	,	drawing_mode(draw_mode)
	,	text_color(txt_col)
	,	back_color(bak_col)
	,	back_mode(bk_mode)
	,	text_align(txt_align)
	{}
	DCState(Font f)                           : flag(FONT_ONLY), font(f), bitmap(default_bitmap_()) {}
	DCState(Pen p)                            : flag(PEN_ONLY), pen(p), bitmap(default_bitmap_()) {}
	DCState(Brush b)                          : flag(BRUSH_ONLY), brush(b), bitmap(default_bitmap_()) {}
	DCState(GDIBitmap bmp)                    : flag(BITMAP_ONLY), bitmap(bmp) {}
	DCState(DC::DrawingMode mode)             : flag(DRAWING_MODE_ONLY), drawing_mode(mode), bitmap(default_bitmap_()) {}
	DCState(const RGBColor& color, TextColor) : flag(TEXT_COLOR_ONLY), text_color(color), bitmap(default_bitmap_()) {}
	DCState(const RGBColor& color, BackColor) : flag(BACK_COLOR_ONLY), back_color(color), bitmap(default_bitmap_()) {}
	DCState(int mode, BackMode)               : flag(BACK_MODE_ONLY), back_mode(mode), bitmap(default_bitmap_()) {}
	DCState(uint align, TextAlign)            : flag(TEXT_ALIGN_ONLY), text_align(align), bitmap(default_bitmap_()) {}

	bool restore_state(DCBuffer* dc_buf) const;

};// class DCBuffer


//########################################################


class DCBuffer : public RefCountBuffer {
public:
	//
	// types
	//
	typedef std::stack<DCState*> StateStack;

	//
	// methods
	//
	DCBuffer(HWND hwnd_, DC::DestructionMethod method);
	DCBuffer(HDC hdc_, DC::DestructionMethod method);
	virtual ~DCBuffer();

	void push_state();
	bool pop_state();

	void push_font() {state_stack()->push(new DCState(get_font()));}
	bool pop_font();

	void push_pen() {state_stack()->push(new DCState(get_pen()));}
	bool pop_pen();

	void push_brush() {state_stack()->push(new DCState(get_brush()));}
	bool pop_brush();

	void push_bitmap() {state_stack()->push(new DCState(get_bitmap()));}
	bool pop_bitmap();

	void push_drawing_mode() {state_stack()->push(new DCState(get_drawing_mode()));}
	bool pop_drawing_mode();

	void push_text_color() {state_stack()->push(new DCState(get_text_color(), DCState::TEXT_COLOR));}
	bool pop_text_color();

	void push_back_color() {state_stack()->push(new DCState(get_back_color(), DCState::BACK_COLOR));}
	bool pop_back_color();

	void push_back_mode() {state_stack()->push(new DCState(get_back_mode(), DCState::BACK_MODE));}
	bool pop_back_mode();

	void push_text_align() {state_stack()->push(new DCState(get_text_align(), DCState::TEXT_ALIGN));}
	bool pop_text_align();

	//
	// accessors and manipulators
	//
	HWND hwnd() const {return m_hwnd;}
	HDC hdc() const {return m_hdc;}

	const Font& get_font() const;
	Font set_font(const Font& font);

	const Pen& get_pen() const;
	Pen set_pen(const Pen& pen);

	const Brush& get_brush() const;
	Brush set_brush(const Brush& brush);

	const DDB& get_bitmap() const;
	DDB set_bitmap(const DDB& bitmap);

	DC::DrawingMode get_drawing_mode() const {return m_current_state->drawing_mode;}
	DC::DrawingMode set_drawing_mode(DC::DrawingMode mode)
	{
		if(mode == m_current_state->drawing_mode)
			return m_current_state->drawing_mode;

		DC::DrawingMode prev_mode = get_drawing_mode();
		m_current_state->drawing_mode = mode;

		uint op_code = 0;
		switch(m_current_state->drawing_mode)
		{
		case DC::OP_COPY:
			op_code = R2_COPYPEN;
			break;
		case DC::OP_INVERT:
			op_code = R2_XORPEN;
			break;
		}

		SetROP2(m_hdc, op_code);

		return prev_mode;
	}

	const RGBColor& get_text_color() const {return m_current_state->text_color;}
	RGBColor set_text_color(const RGBColor& color)
	{
		if(color == m_current_state->text_color)
			return m_current_state->text_color;

		RGBColor prev_color = get_text_color();
		m_current_state->text_color = color;

		SetTextColor(m_hdc, m_current_state->text_color.ref());

		return prev_color;
	}

	const RGBColor& get_back_color() const {return m_current_state->text_color;}
	RGBColor set_back_color(const RGBColor& color)
	{
		if(color == m_current_state->back_color)
			return m_current_state->back_color;

		RGBColor prev_color = get_back_color();
		m_current_state->back_color = color;

		SetBkColor(m_hdc, m_current_state->back_color.ref());

		return prev_color;
	}

	int get_back_mode() const {return m_current_state->back_mode;}
	int set_back_mode(int mode)
	{
		if(mode == m_current_state->back_mode)
			return m_current_state->back_mode;

		int prev_mode = get_back_mode();
		m_current_state->back_mode = mode;

		SetBkMode(m_hdc, m_current_state->back_mode);

		return prev_mode;
	}

	uint get_text_align() const {return m_current_state->text_align;}
	uint set_text_align(uint align)
	{
		if(align == m_current_state->text_align)
			return m_current_state->text_align;

		uint prev_align = get_text_align();
		m_current_state->text_align = align;

		SetTextAlign(m_hdc, m_current_state->text_align);

		return prev_align;
	}

	StateStack* state_stack() {return state_stack_();}
	const StateStack* state_stack() const {return m_state_stack;}

protected:
	//
	// methods
	//
	void finalize_gdi_objects();// all classes inherited from this class must call this method in thier destructor !!

	//
	// manipulators
	//
	void set_hdc(HDC hdc_)
	{
		m_hdc = hdc_;

		if(m_hdc == NULL)
			return;

		set_font(plain_font());
		set_text_color(font_color());
		set_back_color(white_color());
		set_back_mode(TRANSPARENT);
		set_text_align(GetTextAlign(m_hdc));
	}

private:
	//
	// data
	//
	HWND m_hwnd;
	HDC m_hdc;
	DC::DestructionMethod m_destruct_method;

	DCState* m_current_state;
	mutable StateStack* m_state_stack;

	HFONT m_default_hfont;
	HPEN m_default_hpen;
	HBRUSH m_default_hbrush;
	HBITMAP m_default_hbitmap;

	//
	// methods
	//
	HGDIOBJ select_object_(HGDIOBJ obj)
	{
		return SelectObject(m_hdc, obj);
	}
	StateStack* state_stack_() const
	{
		if(m_state_stack == NULL)
			m_state_stack = new StateStack;
		return m_state_stack;
	}

};// class DCBuffer


class PaintDCBuffer : public DCBuffer {
public:
	//
	// methods
	//
	PaintDCBuffer(HWND hwnd_)
	:	DCBuffer((HDC)NULL, DC::DO_NOTHING),
		m_hwnd(hwnd_)
	{
		BeginPaint(m_hwnd, &m_paint_struct);
		set_hdc(m_paint_struct.hdc);
	}
	virtual ~PaintDCBuffer()
	{
		EndPaint(m_hwnd, &m_paint_struct);
	}

	const PAINTSTRUCT& paint_struct() const {return m_paint_struct;}

private:
	//
	// data
	//
	HWND m_hwnd;
	PAINTSTRUCT m_paint_struct;

};// class PaintDCBuffer


//########################################################
// public methods

bool
DCState::restore_state(DCBuffer* dc_buf) const
{
	assert(dc_buf);
	assert(flag != NON_STACKABLE);

	switch(flag)
	{
	case FULL_STATE:
		dc_buf->set_font(font);
		dc_buf->set_pen(pen);
		dc_buf->set_brush(brush);
		dc_buf->set_bitmap(bitmap);

		dc_buf->set_drawing_mode(drawing_mode);
		dc_buf->set_text_color(text_color);
		dc_buf->set_back_color(back_color);
		dc_buf->set_back_mode(back_mode);
		dc_buf->set_text_align(text_align);
		break;

	case FONT_ONLY:
		dc_buf->set_font(font);
		break;

	case PEN_ONLY:
		dc_buf->set_pen(pen);
		break;

	case BRUSH_ONLY:
		dc_buf->set_brush(brush);
		break;

	case BITMAP_ONLY:
		dc_buf->set_bitmap(bitmap);
		break;

	case DRAWING_MODE_ONLY:
		dc_buf->set_drawing_mode(drawing_mode);
		break;

	case TEXT_COLOR_ONLY:
		dc_buf->set_text_color(text_color);
		break;

	case BACK_COLOR_ONLY:
		dc_buf->set_back_color(back_color);
		break;

	case BACK_MODE_ONLY:
		dc_buf->set_back_mode(back_mode);
		break;

	case TEXT_ALIGN_ONLY:
		dc_buf->set_text_align(text_align);
		break;
	}

	return true;
}


//########################################################
// public methods

inline
DCBuffer::DCBuffer(HWND hwnd_, DC::DestructionMethod method_)
:	m_hwnd(hwnd_)
,	m_hdc(NULL)
,	m_destruct_method(method_)
,	m_current_state(new DCState)
,	m_state_stack(NULL)
,	m_default_hfont(NULL)
,	m_default_hpen(NULL)
,	m_default_hbrush(NULL)
,	m_default_hbitmap(NULL)
{
	set_hdc(GetDC(hwnd_));
}

inline
DCBuffer::DCBuffer(HDC hdc_, DC::DestructionMethod method_)
:	m_hwnd(NULL)
,	m_hdc(NULL)
,	m_destruct_method(method_)
,	m_current_state(new DCState)
,	m_state_stack(NULL)
,	m_default_hfont(NULL)
,	m_default_hpen(NULL)
,	m_default_hbrush(NULL)
,	m_default_hbitmap(NULL)
{
	set_hdc(hdc_);
}

DCBuffer::~DCBuffer()
{
	// 派生先のデストラクタでも呼ぼう！一度実行されたら二度目からは何もしないので安心していい。
	finalize_gdi_objects();

	// DESTRUCTION_METHOD によって DC の後処理の仕方を変える
	// DO_NOTHING の時は何もしない
	if(m_hdc)
	{
		if(m_destruct_method == DC::RELEASE_DC)
			ReleaseDC(m_hwnd, m_hdc);
		else if(m_destruct_method == DC::DELETE_DC)
			DeleteDC(m_hdc);

		m_hwnd = NULL;
		m_hdc = NULL;
	}

	if(m_state_stack)
	{
		if(! m_state_stack->empty())
		{
			status_t(st::ERR,
				"デバイスコンテキストのステートスタックが空ではありません。DC::pop_xxx() を忘れていませんか？\n"
				).show_error();

			while(! m_state_stack->empty())
			{
				delete m_state_stack->top();
				m_state_stack->pop();
			}
		}

		delete m_state_stack;
	}

	delete m_current_state;
}

inline void
DCBuffer::finalize_gdi_objects()
{
	// 変更されていたら元に戻す
	if(m_default_hfont)
	{
		select_object_(m_default_hfont);
		m_default_hfont = NULL;
	}

	if(m_default_hpen)
	{
		select_object_(m_default_hpen);
		m_default_hpen = NULL;
	}

	if(m_default_hbrush)
	{
		select_object_(m_default_hbrush);
		m_default_hbrush = NULL;
	}

	if(m_default_hbitmap)
	{
		select_object_(m_default_hbitmap);
		m_default_hbitmap = NULL;
	}
}

inline void
DCBuffer::push_state()
{
	state_stack()->push(
		new DCState(
			get_font(),
			get_pen(),
			get_brush(),
			get_bitmap(),
			get_drawing_mode(),
			get_text_color(),
			get_back_color(),
			get_back_mode(),
			get_text_align()));
}

inline bool
DCBuffer::pop_state()
{
	if(m_state_stack == NULL || state_stack()->empty())
	{
		status_t(st::ERR, "DC::pop_state(): ステートスタックは空です。\n").show_error();
		return false;
	}

	StateStack* stack = state_stack();
	DCState* state;
	do
	{
		state = stack->top();
		stack->pop();

		bool ret = state->restore_state(this);
		delete state;

		if(ret == false)
			return false;

	} while(stack->empty() == false && state->flag > DCState::ONLY_ONE_OBJECT_BEGIN);

	return true;
}

inline bool
DCBuffer::pop_font()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::FONT_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_pen()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::PEN_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_brush()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::BRUSH_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_bitmap()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::BITMAP_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_drawing_mode()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::DRAWING_MODE_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_text_color()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::TEXT_COLOR_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_back_color()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::BACK_COLOR_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_back_mode()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::BACK_MODE_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

inline bool
DCBuffer::pop_text_align()
{
	DCState* state = state_stack()->top();

	if(state->flag != DCState::TEXT_ALIGN_ONLY)
		return false;

	state_stack()->pop();

	bool ret = state->restore_state(this);
	delete state;

	return ret;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

inline const Font&
DCBuffer::get_font() const
{
	// 初めて呼ばれたとき
	if(m_current_state->font.handle() == NULL)
	{
		DCBuffer* self = const_cast<DCBuffer*>(this);

		// デフォルトハンドルを得る
		HFONT default_handle = reinterpret_cast<HFONT>(
			self->select_object_(plain_font().handle()));

		// デフォルトハンドルを持った Font （ハンドルの所有権無し）を設定
		self->m_current_state->font = Font(default_handle, false);

		// HFONT を元に戻す
		self->select_object_(default_handle);
	}

	return m_current_state->font;
}

inline Font
DCBuffer::set_font(const Font& font_)
{
	if(font_.handle() == NULL)
		return m_current_state->font;

	if(font_ == m_current_state->font)
		return m_current_state->font;

	// m_current_state->font がユーザーによって設定されたオブジェクトなら
	// 安全に削除できるようにデフォルトオブジェクトに戻しておく
	if(m_default_hfont)
	{
		select_object_(m_default_hfont);
		m_default_hfont = NULL;
	}

	Font prev_font = get_font();

	// m_current_state->font に代入した時点で 元の m_current_state->font::handle()
	// の所有者はいなくなり、ハンドルは削除される
	if(font_.handle() == m_default_hfont)
	{
		m_current_state->font = Font(m_default_hfont, false);
	}else{
		m_current_state->font = font_;
		m_default_hfont = reinterpret_cast<HFONT>(
			select_object_(m_current_state->font.handle()));
	}

	return prev_font;
}

inline const Pen&
DCBuffer::get_pen() const
{
	if(m_current_state->pen.handle() == NULL)
	{
		DCBuffer* self = const_cast<DCBuffer*>(this);

		HPEN default_handle = reinterpret_cast<HPEN>(
			self->select_object_(transparent_pen().handle()));
		self->m_current_state->pen = Pen(default_handle, false);
		self->select_object_(default_handle);
	}

	return m_current_state->pen;
}

inline Pen
DCBuffer::set_pen(const Pen& pen_)
{
	if(pen_.handle() == NULL)
		return m_current_state->pen;

	if(pen_ == m_current_state->pen)
		return m_current_state->pen;

	if(m_default_hpen)
	{
		select_object_(m_default_hpen);
		m_default_hpen = NULL;
	}

	Pen prev_pen = get_pen();

	if(pen_.handle() == m_default_hpen)
	{
		m_current_state->pen = Pen(m_default_hpen, false);
	}else{
		m_current_state->pen = pen_;
		m_default_hpen = reinterpret_cast<HPEN>(
			select_object_(m_current_state->pen.handle()));
	}

	return prev_pen;
}

inline const Brush&
DCBuffer::get_brush() const
{
	if(m_current_state->brush.handle() == NULL)
	{
		DCBuffer* self = const_cast<DCBuffer*>(this);

		HBRUSH default_handle = reinterpret_cast<HBRUSH>(
			self->select_object_(transparent_brush().handle()));
		self->m_current_state->brush = Brush(default_handle, false);
		self->select_object_(default_handle);
	}

	return m_current_state->brush;
}

inline Brush
DCBuffer::set_brush(const Brush& brush_)
{
	if(brush_.handle() == NULL)
		return m_current_state->brush;

	if(brush_ == m_current_state->brush)
		return m_current_state->brush;

	if(m_default_hbrush)
	{
		select_object_(m_default_hbrush);
		m_default_hbrush = NULL;
	}

	Brush prev_brush = get_brush();

	if(brush_.handle() == m_default_hbrush)
	{
		m_current_state->brush = Brush(m_default_hbrush, false);
	}else{
		m_current_state->brush = brush_;
		m_default_hbrush = reinterpret_cast<HBRUSH>(
			select_object_(m_current_state->brush.handle()));
	}

	return prev_brush;
}

inline const DDB&
DCBuffer::get_bitmap() const
{
	if(m_current_state->bitmap.handle() == NULL)
	{
		DCBuffer* self = const_cast<DCBuffer*>(this);

		HBITMAP default_handle = reinterpret_cast<HBITMAP>(
			self->select_object_(default_bitmap_().handle()));
		self->m_current_state->bitmap = DDB(default_handle, false);
		self->select_object_(default_handle);
	}

	return m_current_state->bitmap;
}

inline DDB
DCBuffer::set_bitmap(const DDB& bitmap_)
{
	if(bitmap_.handle() == NULL)
		return m_current_state->bitmap;

	if(bitmap_ == m_current_state->bitmap)
		return m_current_state->bitmap;

	if(m_default_hbitmap)
	{
		select_object_(m_default_hbitmap);
		m_default_hbitmap = NULL;
	}

	DDB prev_bitmap = get_bitmap();

	if(bitmap_.handle() == m_default_hbitmap)
	{
		m_current_state->bitmap = DDB(m_default_hbitmap, false);
	}else{
		m_current_state->bitmap = bitmap_;
		m_default_hbitmap = reinterpret_cast<HBITMAP>(
			select_object_(m_current_state->bitmap.handle()));
	}

	return prev_bitmap;
}


//########################################################
// public methods

DC::DC(HWND hwnd_, DC::DestructionMethod method_)
:	m_buffer(new DCBuffer(hwnd_, method_))
{
}

DC::DC(HDC hdc_, DC::DestructionMethod method_)
:	m_buffer(new DCBuffer(hdc_, method_))
{
}

DC::DC(DCBuffer* buf)
:	m_buffer(buf)
{
}

DC::~DC()
{
}

//--------------------------------------------------------
// state manipulators

void
DC::push_state()
{
	m_buffer->push_state();
}

bool
DC::pop_state()
{
	return m_buffer->pop_state();
}

const Font&
DC::font() const
{
	return m_buffer->get_font();
}

Font
DC::set_font(const Font& font_)
{
	return m_buffer->set_font(font_);
}

void
DC::push_font()
{
	m_buffer->push_font();
}

void
DC::push_font(const Font& font_)
{
	m_buffer->push_font();
	m_buffer->set_font(font_);
}

bool
DC::pop_font()
{
	return m_buffer->pop_font();
}

const Pen&
DC::pen() const
{
	return m_buffer->get_pen();
}

Pen
DC::set_pen(const Pen& pen_)
{
	return m_buffer->set_pen(pen_);
}

void
DC::push_pen()
{
	m_buffer->push_pen();
}

void
DC::push_pen(const Pen& pen_)
{
	m_buffer->push_pen();
	m_buffer->set_pen(pen_);
}

bool
DC::pop_pen()
{
	return m_buffer->pop_pen();
}

const Brush&
DC::brush() const
{
	return m_buffer->get_brush();
}

Brush
DC::set_brush(const Brush& brush_)
{
	return m_buffer->set_brush(brush_);
}

void
DC::push_brush()
{
	m_buffer->push_brush();
}

void
DC::push_brush(const Brush& brush_)
{
	m_buffer->push_brush();
	m_buffer->set_brush(brush_);
}

bool
DC::pop_brush()
{
	return m_buffer->pop_brush();
}

const DDB&
DC::bitmap() const
{
	return m_buffer->get_bitmap();
}

DDB
DC::set_bitmap(const DDB& bitmap_)
{
	return m_buffer->set_bitmap(bitmap_);
}

void
DC::push_bitmap()
{
	m_buffer->push_bitmap();
}

void
DC::push_bitmap(const DDB& bitmap_)
{
	m_buffer->push_bitmap();
	m_buffer->set_bitmap(bitmap_);
}

bool
DC::pop_bitmap()
{
	return m_buffer->pop_bitmap();
}

DC::DrawingMode
DC::drawing_mode() const
{
	return m_buffer->get_drawing_mode();
}

DC::DrawingMode
DC::set_drawing_mode(DrawingMode mode)
{
	return m_buffer->set_drawing_mode(mode);
}

void
DC::push_drawing_mode()
{
	m_buffer->push_drawing_mode();
}

void
DC::push_drawing_mode(DC::DrawingMode mode_)
{
	m_buffer->push_drawing_mode();
	m_buffer->set_drawing_mode(mode_);
}

bool
DC::pop_drawing_mode()
{
	return m_buffer->pop_drawing_mode();
}

const RGBColor&
DC::text_color() const
{
//	assert(false);
	return m_buffer->get_text_color();
}

RGBColor
DC::set_text_color(const RGBColor& color)
{
//	assert(false);
	return m_buffer->set_text_color(color);
}

void
DC::push_text_color()
{
	m_buffer->push_text_color();
}

void
DC::push_text_color(const RGBColor& color_)
{
	m_buffer->push_text_color();
	m_buffer->set_text_color(color_);
}

bool
DC::pop_text_color()
{
	return m_buffer->pop_text_color();
}

const RGBColor&
DC::back_color() const
{
	return m_buffer->get_back_color();
}

RGBColor
DC::set_back_color(const RGBColor& color)
{
	return m_buffer->set_back_color(color);
}

void
DC::push_back_color()
{
	m_buffer->push_back_color();
}

void
DC::push_back_color(const RGBColor& color_)
{
	m_buffer->push_back_color();
	m_buffer->set_back_color(color_);
}

bool
DC::pop_back_color()
{
	return m_buffer->pop_back_color();
}

int
DC::back_mode() const
{
	return m_buffer->get_back_mode();
}

int
DC::set_back_mode(int mode)
{
	return m_buffer->set_back_mode(mode);
}

void
DC::push_back_mode()
{
	m_buffer->push_back_mode();
}

void
DC::push_back_mode(int mode_)
{
	m_buffer->push_back_mode();
	m_buffer->set_back_mode(mode_);
}

bool
DC::pop_back_mode()
{
	return m_buffer->pop_back_mode();
}

uint
DC::text_align() const
{
	return m_buffer->get_text_align();
}

uint
DC::set_text_align(uint val)
{
	return m_buffer->set_text_align(val);
}

void
DC::push_text_align()
{
	m_buffer->push_text_align();
}

void
DC::push_text_align(uint align_)
{
	m_buffer->push_text_align();
	m_buffer->set_text_align(align_);
}

bool
DC::pop_text_align()
{
	return m_buffer->pop_text_align();
}

Region
DC::clipping_region() const
{
	Region rgn;
	GetClipRgn(hdc(), rgn.handle());

	return rgn;
}

void
DC::constrain_clipping_region(const Region& region)
{
	SelectClipRgn(hdc(), region.handle());
}

void
DC::clear_clipping_region()
{
	SelectClipRgn(hdc(), NULL);
}

//--------------------------------------------------------
// drawing methods

Point
DC::move_to(coord x, coord y)
{
	Point prev_point;

	MoveToEx(hdc(), x, y, &prev_point);

	return prev_point;
}

void
DC::draw_string(const String& str)
{
	draw_string(str, current_point());
}

void
DC::draw_string(const String& str, uint length)
{
	String tmp(str.c_str(), length);
	draw_string(tmp, current_point());
}

void
DC::draw_string(const String& str, const Point& point)
{
#if 1
	text_out(point, str);
#else
	coord str_height = string_height();

	String tmp(str.c_str());// c_str()で渡してコピーを作る。そうしないと参照カウンタが増えるだけ
	Point current_pnt = point;

	char_t *p, *begin = tmp.begin(), *end = tmp.end();// char
	p = begin;

	for(; p != end; ++p)
		if(*p == '\n')
		{
			*p = '\0';// いったん文字列を切る
			text_out(current_pnt, begin);

			*p = '\n';// 元に戻す
			begin = p +1;// 次の行の最初
			current_pnt.y += str_height;
		}

	// 改行文字はなかった
	if(begin == tmp.begin())
		text_out(current_pnt, tmp);
#endif
}

void
DC::draw_string(const String& str, uint length, const Point& point)
{
	String tmp(str.c_str(), length);
	draw_string(tmp, point);
}

void
DC::stroke_line(coord x1, coord y1, coord x2, coord y2)
{
	move_to(x1, y1);
	line_to(x2, y2);
}

void
DC::stroke_rect(coord l, coord t, coord r, coord b)
{
	Brush prev_brush = set_brush(transparent_brush());

	Rectangle(hdc(), l, t, r, b);

	set_brush(prev_brush);
}

void
DC::stroke_round_rect(coord l, coord t, coord r, coord b, coord width, coord height)
{
	Brush prev_brush = set_brush(transparent_brush());

	RoundRect(hdc(), l, t, r, b, width, height);

	set_brush(prev_brush);
}

void
DC::fill_rect(coord l, coord t, coord r, coord b)
{
	Pen prev_pen = set_pen(transparent_pen());

	Rectangle(hdc(), l, t, r +1, b +1);

	set_pen(prev_pen);
}

void
DC::fill_round_rect(coord l, coord t, coord r, coord b, coord width, coord height)
{
	Pen prev_pen = set_pen(transparent_pen());

	RoundRect(hdc(), l, t, r +1, b +1, width, height);

	set_pen(prev_pen);
}

void
DC::stroke_ellipse(const Point& center, coord h_radius, coord v_radius)
{
	Brush prev_brush = set_brush(transparent_brush());

	Ellipse(
		hdc(),
		center.x - h_radius,
		center.y - v_radius,
		center.x + h_radius,
		center.y + v_radius);

	set_brush(prev_brush);
}

void
DC::stroke_ellipse(coord l, coord t, coord r, coord b)
{
	Brush prev_brush = set_brush(transparent_brush());

	Ellipse(hdc(), l, t, r, b);

	set_brush(prev_brush);
}

void
DC::fill_ellipse(const Point& center, coord h_radius, coord v_radius)
{
	Pen prev_pen = set_pen(transparent_pen());

	Ellipse(
		hdc(),
		center.x - h_radius,
		center.y - v_radius,
		center.x + h_radius,
		center.y + v_radius);

	set_pen(prev_pen);
}

void
DC::fill_ellipse(coord l, coord t, coord r, coord b)
{
	Pen prev_pen = set_pen(transparent_pen());

	Ellipse(hdc(), l, t, r, b);

	set_pen(prev_pen);
}

void
DC::stroke_polygon(const Point* point_list, uint num_of_points)
{
//	Brush prev_brush = set_brush(transparent_brush());

	Polyline(hdc(), point_list, num_of_points);

//	set_brush(prev_brush);
}

void
DC::fill_polygon(const Point* point_list, uint num_of_points)
{
	Pen prev_pen = set_pen(transparent_pen());

	Polygon(hdc(), point_list, num_of_points);

	set_pen(prev_pen);
}

void
DC::draw_bitmap(const Bitmap& bitmap)
{
	Point point = current_point();
	BitBlt(
		hdc(),
		point.x,
		point.y,
		bitmap.width(),
		bitmap.height(),
		bitmap.dc(),
		0,
		0,
		SRCCOPY);
}

void
DC::draw_bitmap(const Bitmap& bitmap, const Point& point)
{
	BitBlt(
		hdc(),
		point.x,
		point.y,
		bitmap.width(),
		bitmap.height(),
		bitmap.dc(),
		0,
		0,
		SRCCOPY);
}

void
DC::draw_bitmap(const Bitmap& bitmap, const Rect& rect)
{
	if(rect.width() == bitmap.width() && rect.height() == bitmap.height())
		BitBlt(
			hdc(),
			rect.left,
			rect.top,
			rect.width(),
			rect.height(),
			bitmap.dc(),
			0,
			0,
			SRCCOPY);
	else
		StretchBlt(
			hdc(),
			rect.left,
			rect.top,
			rect.width(),
			rect.height(),
			bitmap.dc(),
			0,
			0,
			bitmap.width(),
			bitmap.height(),
			SRCCOPY);
}

void
DC::draw_bitmap(const Bitmap& bitmap, const Rect& dest, const Rect& src)
{
	if(dest.width() == src.width() && dest.height() == src.height())
		BitBlt(
			hdc(),
			dest.left,
			dest.top,
			dest.width(),
			dest.height(),
			bitmap.dc(),
			src.left,
			src.top,
			SRCCOPY);
	else
		StretchBlt(
			hdc(),
			dest.left,
			dest.top,
			dest.right,
			dest.bottom,
			bitmap.dc(),
			src.left,
			src.top,
			src.width(),
			src.height(),
			SRCCOPY);
}

//--------------------------------------------------------
// win32 drawing methods

void
DC::text_out(coord x, coord y, const String& text)
{
	TextOut(
		hdc(),
		x,
		y,
		text.c_str(),
		text.length());
}

void
DC::draw_text(const Rect& rect, const String& text, uint format)
{
	DrawText(
		hdc(),
		text.c_str(),
		text.length(),
		static_cast<RECT*>(const_cast<Rect*>(&rect)),// これはどうなんだろ、普通に (RECT*)&rect, とやったほうがよいかも…
		format);
}

Rect
DC::calculate_rect(const String& text)
{
	Rect rect;

	DrawText(
		hdc(),
		text.c_str(),
		text.length(),
		&rect,
		DT_CALCRECT);

	return rect;
}

void
DC::line_to(coord x, coord y)
{
	LineTo(hdc(), x, y);
}

void
DC::bit_blt(coord dest_x, coord dest_y, coord dest_width, coord dest_height, DC& src_dc, coord src_x, coord src_y, uint op_code)
{
	BitBlt(
		hdc(),
		dest_x,
		dest_y,
		dest_width,
		dest_height,
		src_dc,
		src_x,
		src_y,
		op_code);
}

void
DC::bit_blt(const Rect& dest_rect, DC& src_dc, const Point& src_point, uint op_code)
{
	BitBlt(
		hdc(),
		dest_rect.left,
		dest_rect.top,
		dest_rect.width(),
		dest_rect.height(),
		src_dc,
		src_point.x,
		src_point.y,
		op_code);
}

void
DC::pat_blt(coord dest_x, coord dest_y, coord dest_width, coord dest_height, uint op_code)
{
	PatBlt(
		hdc(),
		dest_x,
		dest_y,
		dest_width,
		dest_height,
		op_code);
}

void
DC::pat_blt(const Rect& dest_rect, uint op_code)
{
	PatBlt(
		hdc(),
		dest_rect.left,
		dest_rect.top,
		dest_rect.width(),
		dest_rect.height(),
		op_code);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public aacessors

HWND
DC::hwnd() const
{
	return m_buffer->hwnd();
}

HDC
DC::hdc() const
{
	return m_buffer->hdc();
}

Point
DC::current_point() const
{
	Point point;

	MoveToEx(hdc(), 0, 0, &point);
	MoveToEx(hdc(), point.x, point.y, NULL);

	return point;
}

coord
DC::string_width(const String& str, int length) const
{
	if(length < 0)
		length = str.length();

	SIZE size;
	GetTextExtentPoint32(hdc(), str.c_str(), length, &size);

	return size.cx;
}

coord
DC::string_height() const
{
#if 1
	SIZE size;
	GetTextExtentPoint32(hdc(), "lj", 2, &size);

	return size.cy;
#else
	TEXTMETRIC tm;
	GetTextMetrics(hdc(), &tm);

	return tm.tmAscent;
#endif
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public operators

DC::operator HDC() const
{
	return hdc();
}


//########################################################
// public methods

PaintDC::PaintDC(HWND hwnd)
:	DC(new PaintDCBuffer(hwnd))
{
}

PaintDC::~PaintDC()
{
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors

Rect
PaintDC::update_rect() const
{
	return static_cast<const PaintDCBuffer*>(buffer())->paint_struct().rcPaint;
}

bool
PaintDC::erase() const
{
	return to_bool(static_cast<const PaintDCBuffer*>(buffer())->paint_struct().fErase);
}


//########################################################
// public methods

MemoryDC::MemoryDC(HDC hdc)
:	DC(CreateCompatibleDC(hdc), DC::DELETE_DC)
{
}

MemoryDC::~MemoryDC()
{
}


//########################################################
// public methods

ScreenDC::ScreenDC()
:	DC(CreateDC("DISPLAY", NULL, NULL, NULL), DC::DELETE_DC)
{
}

ScreenDC::~ScreenDC()
{
}


//########################################################
}// namespace ntk
