#ifndef __CANVAS_H__
#define __CANVAS_H__


#include <ntk/interface/alert.h>
#include <ntk/interface/view.h>
#include <ntk/interface/bitmap.h>
#include <ntk/support/observable.h>
#include <ntk/application/application.h>
#include "image.h"


class canvas_t : public ntk_observable {
public:
	//
	// constants
	//
	enum {
		MODIFIED = 'MDFD'
	};

	//
	// methods
	//
	canvas_t(image_t* image)
	{
		if(image == NULL)
			image = new image_t(ntk_rect(0, 0, 640, 480), ntk_bitmap::RGB_24);

		m_image = image;
	}
	canvas_t(const ntk_entry& entry)
	{
		if(entry.init_check() == false)
		{
			m_image = NULL;
			ntk::alert("ファイルを開く", "画像ファイルの読み込みに失敗しました。", "OK");
		}
		else m_image = new image_t(entry, ntk_rect(0, 0, 100, 100));
	}
	~canvas_t()
	{
		delete m_image;
	}

	void mouse_down(const ntk_point& point, uint buttons)
	{
		paint_(point);
	}
	void mouse_up(const ntk_point& point, uint buttons)
	{
		paint_(point);
	}
	void mouse_moved(const ntk_point& point, uint buttons)
	{
		paint_(point);
	}

	void update_all_views()
	{
		send_message_to_observers(ntk_message(MODIFIED));
	}

	//
	// accessors
	//
	image_t* image() {return m_image;}
	const image_t* image() const {return m_image;}

	const ntk_entry& entry() const {return m_entry;}

private:
	//
	// data
	//
	image_t* m_image;
	ntk_entry m_entry;

	//
	// functions
	//
	void paint_(const ntk_point& point)// マウスポインタの位置を受け取り実際にペイントする処理
	{
		ntk_bitmap* bitmap = m_image->bitmap();

		if(bitmap == NULL)
			return;

		// マウスポインタが画像の大きさの範囲外にあるときは何もしない
		if(point.x < 0 || point.y < 0 || point.x >= bitmap->width() || point.y >= bitmap->height())
			return;

		// 座標(point.x, point.y) のピクセルを取り出す
		uint8* bits = (uint8*)bitmap->at(point);

		// ピクセルデータは RGB の各チャンネルが8ビットからなり、計24ビットで構成されています。
		// 簡単に書くと 0x[RR][GG][BB]（R と B は逆だったかな？）見たいな感じでしょうか。HTMLを書く時、#ffffff という風に
		// 色の指定をしますが、それと同じです。

		bits[0] = 0xff;
		bits[1] = 0xff;
		bits[2] = 0xff;

		// 画面を再描画する
		update_all_views();
	}

};//Canvas


class paint_view_t : public ntk_view {
public:
	paint_view_t(canvas_t* canvas, const ntk_rect& frame)
	:	ntk_view(frame, "PaintView", FOLLOW_ALL_SIDES, WILL_DRAW, ntk::transparent_color())
	,	m_canvas(NULL)
	,	m_painting(false)
	{
		m_canvas = canvas;
//		m_canvas->add_observer(this); -> attached_to_window()
	}
	~paint_view_t()
	{
		if(m_canvas == NULL) return;

		m_canvas->remove_observer(this);
		if(m_canvas->count_observers() == 0)
			delete m_canvas;
	}

	//
	// accessors
	//
	canvas_t* canvas() {return m_canvas;}
	const canvas_t* canvas() const {return m_canvas;}

	//
	//
	//
	void attached_to_window()
	{
		m_canvas->add_observer(this);
	}
	void draw(ntk_paint_dc& dc)
	{
		dc.draw_bitmap(*m_canvas->image()->bitmap(), ntk_point(0, 0));
	}

	void mouse_down(const ntk_point& point, uint buttons, bool double_click)
	{
		begin_capture();
		m_painting = true;
		m_canvas->mouse_down(point, buttons);
	}
	void mouse_up(const ntk_point& point, uint buttons)
	{
		m_canvas->mouse_up(point, buttons);
		m_painting = false;
		end_capture();
	}
	void mouse_moved(const ntk_point& point, uint buttons, uint transit, const ntk_message* data)
	{
		if(m_painting)
			m_canvas->mouse_moved(point, buttons);
	}
	void lost_capture()
	{
		m_painting = false;
	}

	void message_received(const ntk_message& message)
	{
		switch(message.what)
		{
		case canvas_t::MODIFIED:
			invalidate();
			break;

		default:
			ntk_view::message_received(message);
		}
	}

private:
	canvas_t* m_canvas;
	bool m_painting;// マウスでペイント中かどうかのフラグ
};


#endif//EOH