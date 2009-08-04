/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: message.h,v 1.8 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_MESSAGE_MESSAGE_H__
#define __NTK_MESSAGE_MESSAGE_H__


#include <stddef.h>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_REFCOUNT_H__
#include <ntk/support/refcount.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif

#ifndef __NTK_INTERFACE_POINT_H__
#include <ntk/interface/point.h>
#endif

#ifndef __NTK_INTERFACE_RECT_H__
#include <ntk/interface/rect.h>
#endif

#ifndef __NTK_INTERFACE_GDIOBJECT_H__
#include <ntk/interface/gdiobject.h>
#endif


namespace ntk {


class Handler;
class DataIO;
class MessageBuffer;


class Message : public RefCountable {
public:
	//
	// types
	//
	typedef size_t Size;

	union Value {
		bool  as_bool;
		int   as_int;
		int8  as_int8;
		int16 as_int16;
		int32 as_int32;
		float as_float;
		void* pointer;

		Value() {}
		Value(bool val)  : as_bool(val) {}
		Value(int val)   : as_int(val) {}
		Value(int8 val)  : as_int8(val) {}
		Value(int16 val) : as_int16(val) {}
		Value(int32 val) : as_int32(val) {}
		Value(float val) : as_float(val) {}
		Value(void* p)   : pointer(p) {}
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Size size_t;
		typedef Value value_t;
	#endif

	//
	// constants
	//
	enum ValueType {
		UNKNOWN_TYPE = 0,
		BOOL_TYPE,
		INT_TYPE,
		INT8_TYPE,
		INT16_TYPE,
		INT32_TYPE,
		INT64_TYPE,
		FLOAT_TYPE,
		DOUBLE_TYPE,
		STRING_TYPE,
		COLOR_TYPE,
		POINT_TYPE,
		RECT_TYPE,
		REGION_TYPE,
		MESSAGE_TYPE,
		POINTER_TYPE,

		TYPE_LAST,
	};

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef ValueType value_type_t;
	#endif

	//
	// data
	//
	/* user message */
	uint what;

	//
	// methods
	//
	NtkExport Message();
	NtkExport Message(uint command);
	NtkExport Message(uint sys_what, WPARAM wparam, LPARAM lparam);
	NtkExport Message(const MSG& rhs);
	NtkExport Message(const Message& rhs);
	NtkExport Message& operator=(const Message& rhs);
	NtkExport Message& operator=(const MSG& rhs);
	NtkExport virtual ~Message();

	// add_*
	NtkExport virtual status_t add_data(
		const String& name,
		ValueType type,
		void* data,
		size_t size,
		bool fixed_size = true,
		int num_items = 0);
	NtkExport virtual status_t add_bool(const String& name, bool data);
	NtkExport virtual status_t add_int(const String& name, int data);
	NtkExport virtual status_t add_int8(const String& name, int8 data);
	NtkExport virtual status_t add_int16(const String& name, int16 data);
	NtkExport virtual status_t add_int32(const String& name, int32 data);
	NtkExport virtual status_t add_int64(const String& name, int64 data);
	NtkExport virtual status_t add_float(const String& name, float data);
	NtkExport virtual status_t add_double(const String& name, double data);
	NtkExport virtual status_t add_string(const String& name, const String& data);
	NtkExport virtual status_t add_color(const String& name, const RGBColor& data);
	NtkExport virtual status_t add_point(const String& name, const Point& data);
	NtkExport virtual status_t add_rect(const String& name, const Rect& data);
	NtkExport virtual status_t add_region(const String& name, const Region& data);
	NtkExport virtual status_t add_message(const String& name, const Message& data);
	NtkExport virtual status_t add_pointer(const String& name, void* data);

	// replace_*
	NtkExport virtual status_t replace_data(
		const String& name,
		ValueType type,
		void* data,
		size_t size,
		int index = 0);
	NtkExport virtual status_t replace_bool(const String& name, bool data, int index = 0);
	NtkExport virtual status_t replace_int(const String& name, int data, int index = 0);
	NtkExport virtual status_t replace_int8(const String& name, int8 data, int index = 0);
	NtkExport virtual status_t replace_int16(const String& name, int16 data, int index = 0);
	NtkExport virtual status_t replace_int32(const String& name, int32 data, int index = 0);
	NtkExport virtual status_t replace_int64(const String& name, int64 data, int index = 0);
	NtkExport virtual status_t replace_float(const String& name, float data, int index = 0);
	NtkExport virtual status_t replace_double(const String& name, double data, int index = 0);
	NtkExport virtual status_t replace_string(const String& name, const String& data, int index = 0);
	NtkExport virtual status_t replace_color(const String& name, const RGBColor& data, int index = 0);
	NtkExport virtual status_t replace_point(const String& name, const Point& data, int index = 0);
	NtkExport virtual status_t replace_rect(const String& name, const Rect& data, int index = 0);
	NtkExport virtual status_t replace_region(const String& name, const Region& data, int index = 0);
	NtkExport virtual status_t replace_message(const String& name, const Message& data, int index = 0);
	NtkExport virtual status_t replace_pointer(const String& name, void* data, int index = 0);

	// has_*
	NtkExport virtual bool has_data(const String& name, ValueType type, int index = 0) const;
	NtkExport virtual bool has_bool(const String& name, int index = 0) const;
	NtkExport virtual bool has_int(const String& name, int index = 0) const;
	NtkExport virtual bool has_int8(const String& name, int index = 0) const;
	NtkExport virtual bool has_int16(const String& name, int index = 0) const;
	NtkExport virtual bool has_int32(const String& name, int index = 0) const;
	NtkExport virtual bool has_int64(const String& name, int index = 0) const;
	NtkExport virtual bool has_float(const String& name, int index = 0) const;
	NtkExport virtual bool has_double(const String& name, int index = 0) const;
	NtkExport virtual bool has_string(const String& name, int index = 0) const;
	NtkExport virtual bool has_color(const String& name, int index = 0) const;
	NtkExport virtual bool has_point(const String& name, int index = 0) const;
	NtkExport virtual bool has_rect(const String& name, int index = 0) const;
	NtkExport virtual bool has_region(const String& name, int index = 0) const;
	NtkExport virtual bool has_message(const String& name, int index = 0) const;
	NtkExport virtual bool has_pointer(const String& name, int index = 0) const;

	// find_*
	NtkExport virtual status_t find_data(
		const String& name,
		ValueType type,
		void** data,
		size_t* size,
		int index = 0) const;
	NtkExport virtual status_t find_bool(const String& name, bool* data, int index = 0) const;
	NtkExport virtual bool     find_bool(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_int(const String& name, int* data, int index = 0) const;
	NtkExport virtual int      find_int(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_int8(const String& name, int8* data, int index = 0) const;
	NtkExport virtual int8     find_int8(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_int16(const String& name, int16* data, int index = 0) const;
	NtkExport virtual int16    find_int16(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_int32(const String& name, int32* data, int index = 0) const;
	NtkExport virtual int32    find_int32(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_int64(const String& name, int64* data, int index = 0) const;
	NtkExport virtual int64    find_int64(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_float(const String& name, float* data, int index = 0) const;
	NtkExport virtual float    find_float(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_double(const String& name, double* data, int index = 0) const;
	NtkExport virtual double   find_double(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_string(const String& name, String* data, int index = 0) const;
	NtkExport virtual String   find_string(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_color(const String& name, RGBColor* data, int index = 0) const;
	NtkExport virtual RGBColor find_color(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_point(const String& name, Point* data, int index = 0) const;
	NtkExport virtual Point    find_point(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_rect(const String& name, Rect* data, int index = 0) const;
	NtkExport virtual Rect     find_rect(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_region(const String& name, Region* data, int index = 0) const;
	NtkExport virtual Region   find_region(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_message(const String& name, Message* data, int index = 0) const;
	NtkExport virtual Message  find_message(const String& name, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t find_pointer(const String& name, void** data, int index = 0) const;
	NtkExport virtual void*    find_pointer(const String& name, int index = 0, status_t* status = NULL) const;

	NtkExport virtual status_t remove_name(const String& name);
	NtkExport virtual status_t remove_data(const String& name, int index = 0);

	NtkExport virtual Size flattened_size() const;
	NtkExport virtual status_t flatten(DataIO* io, Size* num_bytes = NULL) const;
	NtkExport virtual status_t flatten(void* buffer, Size* num_bytes = NULL) const;
	NtkExport virtual status_t unflatten(DataIO* io);
	NtkExport virtual status_t unflatten(const void* buffer);

	//
	// accessors and manipulators
	//
	Handler* target() const {return m_target;}
	Handler* reply_handler() const {return m_reply_handler;}
	NtkExport void set_target(Handler* target, Handler* repry_handler);

	const MSG& msg() const {return m_msg;}

	HWND hwnd() const {return m_msg.hwnd;}
	UINT sys_what() const {return m_msg.message;}
	WPARAM wparam() const {return m_msg.wParam;}
	LPARAM lparam() const {return m_msg.lParam;}
	DWORD time() const {return m_msg.time;}
	const POINT& point() const {return m_msg.pt;}

	void set_param(WPARAM wp, LPARAM lp) {m_msg.wParam = wp; m_msg.lParam = lp;}

	// as data container
	NtkExport virtual uint count_names() const;

	NtkExport virtual bool is_empty() const;
	NtkExport virtual status_t make_empty();

	// for debug
	String as_string() const;

	//
	// operators
	//
	NtkExport bool operator==(const Message& rhs) const;
	NtkExport bool operator!=(const Message& rhs) const;

private:
	//
	// constants
	//
	enum {
		READ_MODE,
		WRITE_MODE
	};

	//
	// data
	//
	MSG m_msg;
	Handler *m_target, *m_reply_handler;

	//
	// accessors
	//
	MessageBuffer* msg_buffer(uint mode = READ_MODE) const;

	//
	// friends
	//
	friend class Window;

//‚½‚Ö‚¢‚Ù
};// class Message


// win32 message utility //
NtkExport const char_t* get_win32_message_name(uint what);


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Message message_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Message NMessage;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Message ntk_message;
	#endif
#endif


#endif//EOH
