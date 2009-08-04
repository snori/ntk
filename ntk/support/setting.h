/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: setting.h,v 1.4 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_SETTING_H__
#define __NTK_SUPPORT_SETTING_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_APPLICATION_MESSAGE_H__
#include <ntk/application/message.h>
#endif


namespace ntk {


class Setting : public Message {
public:
	//
	// methods
	//
	NtkExport Setting();
	NtkExport Setting(const String& file_name);// support auto save/load
	NtkExport ~Setting();

	// set_*
	NtkExport virtual status_t set_data(
		const String& name,
		ValueType type,
		void* data,
		size_t size,
		bool fixed_size = true,
		int num_items = 0);
	NtkExport virtual status_t set_bool(const String& name, bool data);
	NtkExport virtual status_t set_int(const String& name, int data);
	NtkExport virtual status_t set_int8(const String& name, int8 data);
	NtkExport virtual status_t set_int16(const String& name, int16 data);
	NtkExport virtual status_t set_int32(const String& name, int32 data);
	NtkExport virtual status_t set_int64(const String& name, int64 data);
	NtkExport virtual status_t set_float(const String& name, float data);
	NtkExport virtual status_t set_double(const String& name, double data);
	NtkExport virtual status_t set_string(const String& name, const String& data);
	NtkExport virtual status_t set_color(const String& name, const RGBColor& data);
	NtkExport virtual status_t set_point(const String& name, const Point& data);
	NtkExport virtual status_t set_rect(const String& name, const Rect& data);
	NtkExport virtual status_t set_region(const String& name, const Region& data);
	NtkExport virtual status_t set_message(const String& name, const Message& data);
	NtkExport virtual status_t set_setting(const String& name, const Setting& data);
	NtkExport virtual status_t set_pointer(const String& name, void* data);

	// get_*
	NtkExport virtual status_t get_data(
		const String& name,
		ValueType type,
		void** data,
		size_t* size,
		void* default_data,
		int index = 0) const;
	NtkExport virtual status_t get_bool(const String& name, bool* data, bool default_data = false, int index = 0) const;
	NtkExport virtual bool     get_bool(const String& name, bool default_data = false, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_int(const String& name, int* data, int default_data = 0, int index = 0) const;
	NtkExport virtual int      get_int(const String& name, int default_data = 0, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_int8(const String& name, int8* data, int8 default_data = 0, int index = 0) const;
	NtkExport virtual int8     get_int8(const String& name, int8 default_data = 0, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_int16(const String& name, int16* data, int16 default_data = 0, int index = 0) const;
	NtkExport virtual int16    get_int16(const String& name, int16 default_data = 0, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_int32(const String& name, int32* data, int32 default_data = 0, int index = 0) const;
	NtkExport virtual int32    get_int32(const String& name, int32 default_data = 0, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_int64(const String& name, int64* data, int64 default_data = 0, int index = 0) const;
	NtkExport virtual int64    get_int64(const String& name, int64 default_data = 0, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_float(const String& name, float* data, float default_data = 0.f, int index = 0) const;
	NtkExport virtual float    get_float(const String& name, float default_data = 0.f, int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_double(const String& name, double* data, double default_data = 0., int index = 0) const;
	NtkExport virtual double   get_double(const String& name, double default_data = 0., int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_string(const String& name, String* data, const String& default_data = "", int index = 0) const;
	NtkExport virtual String   get_string(const String& name, const String& default_data = "", int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_color(const String& name, RGBColor* data, const RGBColor& default_data = RGBColor(0, 0, 0, 0), int index = 0) const;
	NtkExport virtual RGBColor get_color(const String& name, const RGBColor& default_data = RGBColor(0, 0, 0, 0), int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_point(const String& name, Point* data, const Point& default_data = Point(0, 0), int index = 0) const;
	NtkExport virtual Point    get_point(const String& name, const Point& default_data = Point(0, 0), int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_rect(const String& name, Rect* data, const Rect& default_data = Rect(0, 0, 0, 0), int index = 0) const;
	NtkExport virtual Rect     get_rect(const String& name, const Rect& default_data = Rect(0, 0, 0, 0), int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_region(const String& name, Region* data, const Region& default_data = Region(), int index = 0) const;
	NtkExport virtual Region   get_region(const String& name, const Region& default_data = Region(), int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_message(const String& name, Message* data, const Message& default_data = Message(), int index = 0) const;
	NtkExport virtual Message  get_message(const String& name, const Message& default_data = Message(), int index = 0, status_t* status = NULL) const;
	NtkExport virtual status_t get_setting(const String& name, Setting* data, const Setting& default_data = Setting(), int index = 0) const;
	NtkExport virtual Setting  get_setting(const String& name, const Setting& default_data = Setting(), int index = 0, status_t* dtatus = NULL) const;
	NtkExport virtual status_t get_pointer(const String& name, void** data, void* default_data = NULL, int index = 0) const;
	NtkExport virtual void*    get_pointer(const String& name, void* default_data = NULL, int index = 0, status_t* status = NULL) const;

	//
	// accessors and manipulators
	//
	NtkExport String file_name() const;
	NtkExport void set_file_name(const String& file_name);
	NtkExport bool has_file_name() const;

private:
	//
	// data
	//
	String m_file_name;

};// class Setting


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef Setting setting_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::Setting NSetting;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::Setting ntk_setting;
	#endif
#endif


#endif//EOH
