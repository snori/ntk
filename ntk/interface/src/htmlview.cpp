/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: htmlview.cpp,v 1.8 2003/11/11 12:07:08 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/interface/htmlview.h"


#include <exdispid.h>// DISPID_xxxx
#include <ntk/windows/windows.h>
#include <ntk/support/utility.h>

#include <ntk/interface/bitmap.h>
#include <ntk/translation/utility.h>


#define for if(false); else for


namespace ntk {


//########################################################


namespace {


enum {
	SINKID = '_HTS'// HTml Sink
};


String
atl_windowclass_name()
{
	initialize_atl();
	return "AtlAxWin";
}


}// anonymous namespace


class ATL_NO_VTABLE HTMLViewSink :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispEventImpl<SINKID, HTMLViewSink, &DIID_DWebBrowserEvents2> {
public:
	//
	// methods
	//
	HTMLViewSink() {}

	status_t attach_web_browser(CComPtr<IUnknown> web_browser)
	{
		m_web_browser = web_browser;

		AtlGetObjectSourceInterface(web_browser, &m_libid, &m_iid, &m_wMajorVerNum, &m_wMinorVerNum);

		HRESULT hresult = DispEventAdvise(web_browser);

		if(FAILED(hresult))
			return st::SYSTEM_ERROR;

		return st::OK;
	}

	void _stdcall on_before_navigate(
		IDispatch** dispatch,
		VARIANT* url,
		VARIANT*,
		VARIANT* target,
		VARIANT*,
		VARIANT*,
		VARIANT_BOOL* cancel)
	{
		#ifdef NTK_UNICODE
			String addr = url->bstrVal;
		#else
			String addr = wc2mb(url->bstrVal);
		#endif

		m_html_view->m_current_address = addr;

		Message message(HTMLView::ADDRESS_CHANGED);
		message.add_string("url", addr);

		m_html_view->invoke(message);
	}

	void _stdcall on_new_window(IDispatch** dispatch, VARIANT_BOOL* cancel)
	{
		*cancel = VARIANT_TRUE;
	}

	void _stdcall on_quit()
	{
		DispEventUnadvise(m_web_browser);
	}

	BEGIN_COM_MAP(HTMLViewSink)
		COM_INTERFACE_ENTRY_IID(DIID_DWebBrowserEvents2, HTMLViewSink)
	END_COM_MAP()

	BEGIN_SINK_MAP(HTMLViewSink)
		SINK_ENTRY_EX(SINKID, DIID_DWebBrowserEvents2, DISPID_BEFORENAVIGATE2, on_before_navigate)
		SINK_ENTRY_EX(SINKID, DIID_DWebBrowserEvents2, DISPID_NEWWINDOW2, on_new_window)
		SINK_ENTRY_EX(SINKID, DIID_DWebBrowserEvents2, DISPID_ONQUIT, on_quit)
	END_SINK_MAP()

private:
	//
	// data
	//
	CComPtr<IUnknown> m_web_browser;
	HTMLView* m_html_view;

	//
	// friends
	//
	friend class HTMLView;

};// ckass HTMLViewSink


//########################################################
// public methods


HTMLView::HTMLView(const Rect& frame, const String& name, uint mode, uint flags, uint style, uint ex_style)
:	View(atl_windowclass_name(), frame, "Shell.Explorer.2", mode, flags, ntk::transparent_color(), style, ex_style)
,	Invoker(NULL, NULL, NULL)
,	m_name(name)
{
	CComPtr<IUnknown> control_interface;

	HRESULT ret = AtlAxGetControl(hwnd(), &control_interface);
	if(ret == S_OK)
	{
		m_web_browser = control_interface;
		if(! m_web_browser)
		{
			status_t(st::SYSTEM_ERROR).show_error();
			return;
		}

		CComObject<HTMLViewSink>* sink;
		CComObject<HTMLViewSink>::CreateInstance(&sink);

		sink->m_html_view = this;

		status_t sts = sink->attach_web_browser(control_interface);
		if(! sts)
		{
			sts.show_error();
			return;
		}
	}
}

HTMLView::~HTMLView()
{
}

void
HTMLView::go(const String& url)
{
	if(url == "http://home")
		m_web_browser->GoHome();
	else
	{
		CComVariant url_var(url.c_str()), dummy;
		m_web_browser->Navigate2(&url_var, &dummy, &dummy, &dummy, &dummy);
	}
}

void
HTMLView::refresh(uint refresh_level) const
{
	CComVariant level((int)refresh_level);
	m_web_browser->Refresh2(&level);
}

void
HTMLView::stop() const
{
	m_web_browser->Stop();
}

void
HTMLView::go_back()
{
	m_web_browser->GoBack();
}

void
HTMLView::go_forward()
{
	m_web_browser->GoForward();
}

//--------------------------------------------------------
// public accessors and manipulators

String
HTMLView::name() const
{
	return m_name;
}

void
HTMLView::set_name(const String& name)
{
	m_name = name;
}

String
HTMLView::current_address() const
{
	return m_current_address;
}

String
HTMLView::html() const
{
#if 0
	OutputDebugString("###########<<<<<<<<<<<\n");

	CComPtr<IDispatch> dispatch;
	m_web_browser->get_Document(&dispatch);
	CComQIPtr<IHTMLDocument2> document = dispatch;

	CComPtr<IHTMLElementCollection> collection;
	document->get_all(&collection);
	long size;
	collection->get_length(&size);

	if(size == 0)
		return "";

	USES_CONVERSION;
	for(long i = 0; i < size; ++i)
	{
		CComVariant name(i), index(0);
		CComPtr<IDispatch> dispatch;
		collection->item(name, index, &dispatch);

		CComQIPtr<IHTMLElement> element;
		element = dispatch;

		BSTR str;
		element->get_outerHTML(&str);

		OutputDebugString("<<<<<<<<<<\n");
		OutputDebugString(wc2mb(str).c_str());
		OutputDebugString(">>>>>>>>>>\n");
	}

	OutputDebugString("###########>>>>>>>>>>>>>>\n");
	return wc2mb(L"");

#if 0
	CComPtr<IDispatch> dispatch;
	m_web_browser->get_Document(&dispatch);
	CComQIPtr<IHTMLDocument2> document = dispatch;

	CComPtr<IHTMLElement> element;
	document->get_body(&element);

	BSTR str;
	element->get_outerHTML(&str);

	return wc2mb(str);
#endif
#else
	return "";
#endif
}

void
HTMLView::set_html(const String& source)
{
	CComPtr<IDispatch> dispatch;
	m_web_browser->get_Document(&dispatch);
	CComQIPtr<IHTMLDocument2> document = dispatch;

	CComPtr<IHTMLElement> element;
	document->get_body(&element);

	element->put_outerHTML((BSTR)mb2wc(source).c_str());
}

Rect
HTMLView::page_bounds() const
{
#if 0
	CComPtr<IDispatch> dispatch;
	m_web_browser->get_Document(&dispatch);
	CComQIPtr<IHTMLDocument2> html_doc = dispatch;

	CComPtr<IHTMLElement> body;
	html_doc->get_body(&body);

	CComPtr<IHTMLElement2> body2;
	body->QueryInterface(IID_IHTMLElement2, (void**)&body2);

	// COM は自信無し。解放とかって、これでちゃんと出来てるのだろうか

	long width, height;
	body2->get_scrollWidth(&width);
	body2->get_scrollHeight(&height);

	return Rect(0, 0, width, height);
#else
	return Rect(0, 0, 0, 0);
#endif
}

//--------------------------------------------------------
// public message handlers

void
HTMLView::attached_to_window()
{
	View::attached_to_window();

	if(target())
		set_target(target());
	else
		set_target(NULL, looper());
}

void
HTMLView::message_received(const Message& message)
{
	switch(message.what)
	{
	case CAPTURE_PAGE:
		{
#if 0
			Rect rect = page_bounds();
			Bitmap bmp(rect.width(), rect.height(), Bitmap::RGB_24);

			long control_width, control_height;
			m_web_browser->get_Width(&control_width);
			m_web_browser->get_Height(&control_height);
			m_web_browser->put_Width(bmp.width());
			m_web_browser->put_Height(bmp.height());
return;
			#if 1
				OleDraw(m_web_browser, DVASPECT_CONTENT, bmp.hdc(), &bmp.bounds());
			#else
				Bitmap tmp_bmp(width, control_height, Bitmap::RGB_24);

				int scroll_count = bmp.height() / control_height;
				if(bmp.height() % control_height > 0)
					scroll_count++;

				for(int i=0; i<scroll_count; ++i)
				{
					Rect rect(0, 0, bmp.width(), control_height);

					if(i == scroll_count -1)
						rect.bottom = rect.top + control_height % bmp.height();

					OleDraw(m_web_browser, DVASPECT_CONTENT, tmp_bmp.dc().hdc(), &rect);

					CComVariant operation("pageDown");
					body2->doScroll(operation);

					coord y = control_height * i;
					bmp.dc().bit_blt(
						0, y, bmp.width(), y + rect.height(),
						tmp_bmp.dc(), 0, 0, DC::OP_COPY);
				}
			#endif

			m_web_browser->put_Width(control_width);
			m_web_browser->put_Height(control_height);

			TranslationUtils::store_bitmap("c:\\ie.bmp", bmp);
#elif 0
			HDC dc;
			status_t sts = message.find_pointer("hdc", (void**)&dc);

			if(sts == false)
				return;

			Rect rect;
			sts = message.find_rect("bounds", &rect);

			if(sts == false)
				rect.reset(0, 0, 500, 500);

			OleDraw(m_web_browser, DVASPECT_DOCPRINT, dc, &rect);
#endif
		}
		break;

	default:
		View::message_received(message);
	}
}

LRESULT
HTMLView::system_message_received(const Message& message)
{
	switch(message.what)
	{
	case WM_DESTROY:
		m_web_browser.Release();
		return View::system_message_received(message);

	default:
		return View::system_message_received(message);
	}
}


//########################################################
}// namespace ntk
