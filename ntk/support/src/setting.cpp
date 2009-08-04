/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: setting.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/setting.h"


#include <ntk/storage/file.h>


namespace ntk {


//########################################################
// public methods

Setting::Setting()
{
}

Setting::Setting(const String& file_name)
:	m_file_name(file_name)
{
	if(has_file_name())
	{
		FILE file(m_file_name, "rb");
		if(file.init_check())
			unflatten(&file);
	}
}

Setting::~Setting()
{
	if(has_file_name())
	{
		FILE file(m_file_name, "wb");
		if(file.init_check())
			flatten(&file);
	}
}

//--------------------------------------------------------
// set_*

status_t
Setting::set_data(const String& name, ValueType type, void* data, size_t size, bool fixed_size, int num_items)
{
	if(has_data(name, type))
		return replace_data(name, type, data, size);
	else
		return add_data(name, type, data, size, fixed_size);
}

status_t
Setting::set_bool(const String& name, bool data)
{
	if(has_bool(name))
		return replace_bool(name, data);
	else
		return add_bool(name, data);
}

status_t
Setting::set_int(const String& name, int data)
{
	if(has_int(name))
		return replace_int(name, data);
	else
		return add_int(name, data);
}

status_t
Setting::set_int8(const String& name, int8 data)
{
	if(has_int8(name))
		return replace_int8(name, data);
	else
		return add_int8(name, data);
}

status_t
Setting::set_int16(const String& name, int16 data)
{
	if(has_int16(name))
		return replace_int16(name, data);
	else
		return add_int16(name, data);
}

status_t
Setting::set_int32(const String& name, int32 data)
{
	if(has_int32(name))
		return replace_int32(name, data);
	else
		return add_int32(name, data);
}

status_t
Setting::set_int64(const String& name, int64 data)
{
	if(has_int64(name))
		return replace_int64(name, data);
	else
		return add_int64(name, data);
}

status_t
Setting::set_float(const String& name, float data)
{
	if(has_float(name))
		return replace_float(name, data);
	else
		return add_float(name, data);
}

status_t
Setting::set_double(const String& name, double data)
{
	if(has_double(name))
		return replace_double(name, data);
	else
		return add_double(name, data);
}

status_t
Setting::set_string(const String& name, const String& data)
{
	if(has_string(name))
		return replace_string(name, data);
	else
		return add_string(name, data);
}

status_t
Setting::set_color(const String& name, const RGBColor& data)
{
	if(has_color(name))
		return replace_color(name, data);
	else
		return add_color(name, data);
}

status_t
Setting::set_point(const String& name, const Point& data)
{
	if(has_point(name))
		return replace_point(name, data);
	else
		return add_point(name, data);
}

status_t
Setting::set_rect(const String& name, const Rect& data)
{
	if(has_rect(name))
		return replace_rect(name, data);
	else
		return add_rect(name, data);
}

status_t
Setting::set_region(const String& name, const Region& data)
{
	if(has_region(name))
		return replace_region(name, data);
	else
		return add_region(name, data);
}

status_t
Setting::set_message(const String& name, const Message& data)
{
	if(has_message(name))
		return replace_message(name, data);
	else
		return add_message(name, data);
}

status_t
Setting::set_setting(const String& name, const Setting& data)
{
	if(has_message(name))
		return replace_message(name, data);
	else
		return add_message(name, data);
}

status_t
Setting::set_pointer(const String& name, void* data)
{
	if(has_pointer(name))
		return replace_pointer(name, data);
	else
		return add_pointer(name, data);
}

//--------------------------------------------------------
// get_*

status_t
Setting::get_data(const String& name, ValueType type, void** data, size_t* size, void* default_data, int index) const
{
	if(has_data(name, type, index))
		return find_data(name, type, data, size, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

status_t
Setting::get_bool(const String& name, bool* data, bool default_data, int index) const
{
	if(has_bool(name, index))
		return find_bool(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

bool
Setting::get_bool(const String& name, bool default_data, int index, status_t* status) const
{
	if(has_bool(name, index))
		return find_bool(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_int(const String& name, int* data, int default_data, int index) const
{
	if(has_int(name, index))
		return find_int(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

int
Setting::get_int(const String& name, int default_data, int index, status_t* status) const
{
	if(has_int(name, index))
		return find_int(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_int8(const String& name, int8* data, int8 default_data, int index) const
{
	if(has_int8(name, index))
		return find_int8(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

int8
Setting::get_int8(const String& name, int8 default_data, int index, status_t* status) const
{
	if(has_int8(name, index))
		return find_int8(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_int16(const String& name, int16* data, int16 default_data, int index) const
{
	if(has_int16(name, index))
		return find_int16(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

int16
Setting::get_int16(const String& name, int16 default_data, int index, status_t* status) const
{
	if(has_int16(name, index))
		return find_int16(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_int32(const String& name, int32* data, int32 default_data, int index) const
{
	if(has_int32(name, index))
		return find_int32(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

int32
Setting::get_int32(const String& name, int32 default_data, int index, status_t* status) const
{
	if(has_int32(name, index))
		return find_int32(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_int64(const String& name, int64* data, int64 default_data, int index) const
{
	if(has_int64(name, index))
		return find_int64(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

int64
Setting::get_int64(const String& name, int64 default_data, int index, status_t* status) const
{
	if(has_int64(name, index))
		return find_int64(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_float(const String& name, float* data, float default_data, int index) const
{
	if(has_float(name, index))
		return find_float(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

float
Setting::get_float(const String& name, float default_data, int index, status_t* status) const
{
	if(has_float(name, index))
		return find_float(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_double(const String& name, double* data, double default_data, int index) const
{
	if(has_double(name, index))
		return find_double(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

double
Setting::get_double(const String& name, double default_data, int index, status_t* status) const
{
	if(has_double(name, index))
		return find_double(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_string(const String& name, String* data, const String& default_data, int index) const
{
	if(has_string(name, index))
		return find_string(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

String
Setting::get_string(const String& name, const String& default_data, int index, status_t* status) const
{
	if(has_string(name, index))
		return find_string(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_color(const String& name, RGBColor* data, const RGBColor& default_data, int index) const
{
	if(has_color(name, index))
		return find_color(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

RGBColor
Setting::get_color(const String& name, const RGBColor& default_data, int index, status_t* status) const
{
	if(has_color(name, index))
		return find_color(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_point(const String& name, Point* data, const Point& default_data, int index) const
{
	if(has_point(name, index))
		return find_point(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

Point
Setting::get_point(const String& name, const Point& default_data, int index, status_t* status) const
{
	if(has_point(name, index))
		return find_point(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_rect(const String& name, Rect* data, const Rect& default_data, int index) const
{
	if(has_rect(name, index))
		return find_rect(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

Rect
Setting::get_rect(const String& name, const Rect& default_data, int index, status_t* status) const
{
	if(has_rect(name, index))
		return find_rect(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_region(const String& name, Region* data, const Region& default_data, int index) const
{
	if(has_region(name, index))
		return find_region(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

Region
Setting::get_region(const String& name, const Region& default_data, int index, status_t* status) const
{
	if(has_region(name, index))
		return find_region(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_message(const String& name, Message* data, const Message& default_data, int index) const
{
	if(has_message(name, index))
		return find_message(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

Message
Setting::get_message(const String& name, const Message& default_data, int index, status_t* status) const
{
	if(has_message(name, index))
		return find_message(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_setting(const String& name, Setting* data, const Setting& default_data, int index) const
{
	if(has_message(name, index))
		return find_message(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

Setting
Setting::get_setting(const String& name, const Setting& default_data, int index, status_t* status) const
{
	if(has_message(name, index))
		return (Setting&)find_message(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}

status_t
Setting::get_pointer(const String& name, void** data, void* default_data, int index) const
{
	if(has_pointer(name, index))
		return find_pointer(name, data, index);
	else
	{
		*data = default_data;
		return st::OK;
	}
}

void*
Setting::get_pointer(const String& name, void* default_data, int index, status_t* status) const
{
	if(has_pointer(name, index))
		return find_pointer(name, index, status);
	else
	{
		if(status) status->reset(st::OK);
		return default_data;
	}
}


//--------------------------------------------------------
// public accessors and manipulators


String
Setting::file_name() const
{
	return m_file_name;
}

void
Setting::set_file_name(const String& file_name)
{
	m_file_name = file_name;
}

bool
Setting::has_file_name() const
{
	return m_file_name != String();
}


//########################################################
}// namespace ntk
