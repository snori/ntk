/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: message.cpp,v 1.11 2003/11/11 12:07:05 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/application/message.h"


#include <math.h>// for abs()
#include <tchar.h>
#include <ntk/windows/windows.h>
#include <ntk/interface/point.h>
#include <ntk/interface/rect.h>
#include <ntk/support/dataio.h>
#include <ntk/application/defs.h>
#include <ntk/support/version.h>
#include <ntk/support/hash.h>


namespace ntk {


//########################################################


//#define MSG_BUFFER ((MessageBuffer*)ref_count_buffer())

struct MessageData {
	//
	// data
	//
	Message::ValueType type;
	Message::Value data;
	Message::Size size;

	//
	// methods
	//
	MessageData()
	:	type(Message::UNKNOWN_TYPE)
	,	size(0)
	{
		data.as_int32 = 0;
	}
	MessageData(const MessageData& val)
	{
		switch(type)
		{
		case Message::BOOL_TYPE:
			data.as_bool = val.data.as_bool;
			break;

		case Message::INT_TYPE:
			data.as_int = val.data.as_int;
			break;

		case Message::INT8_TYPE:
			data.as_int8 = val.data.as_int8;
			break;

		case Message::INT16_TYPE:
			data.as_int16 = val.data.as_int16;
			break;

		case Message::INT32_TYPE:
			data.as_int32 = val.data.as_int32;
			break;

		case Message::FLOAT_TYPE:
			data.as_float = val.data.as_float;
			break;

		case Message::INT64_TYPE:
			data.pointer = new int64(*reinterpret_cast<int64*>(val.data.pointer));
			break;

		case Message::DOUBLE_TYPE:
			data.pointer = new double(*reinterpret_cast<double*>(val.data.pointer));
			break;

		case Message::STRING_TYPE:
			{
				char_t* str = reinterpret_cast<char_t*>(val.data.pointer);
				char_t* buf = new char_t[strlen(str)];
				strcpy(buf, str);
				data.pointer = buf;
			}
			break;

		case Message::POINT_TYPE:
			data.pointer = new Point(*reinterpret_cast<Point*>(val.data.pointer));
			break;

		case Message::COLOR_TYPE:
			data.pointer = new RGBColor(*reinterpret_cast<RGBColor*>(val.data.pointer));
			break;

		case Message::RECT_TYPE:
			data.pointer = new Rect(*reinterpret_cast<Rect*>(val.data.pointer));
			break;

		case Message::MESSAGE_TYPE:
			data.pointer = new Message(*reinterpret_cast<Message*>(val.data.pointer));
			break;
		}
	}
	~MessageData()
	{
		delete_data_();
	}

	Message::Size flatten(DataIO* pio, status_t* status = NULL) const
	{
		assert(pio);

		status_t sts_, &sts = status ? *status : sts_;
		sts = st::OK;

		if(type == Message::UNKNOWN_TYPE)
		{
			sts.reset(st::BAD_VALUE_ERROR, "未初期化のデータを flattening することは出来ません\n");
			return 0;
		}

		DataIO& io = *pio;
		DataIO::Size total_size = 0, additional_size = 0;

		total_size += io.write(&type, sizeof(type));
		additional_size += sizeof(type);

		switch(type)
		{
		case Message::BOOL_TYPE:
			total_size += io.write(&data.as_bool, size);
			break;

		case Message::INT_TYPE:
			total_size += io.write(&data.as_int, size);
			break;

		case Message::INT8_TYPE:
			total_size += io.write(&data.as_int8, size);
			break;

		case Message::INT16_TYPE:
			total_size += io.write(&data.as_int16, size);
			break;

		case Message::INT32_TYPE:
			total_size += io.write(&data.as_int32, size);
			break;

		case Message::INT64_TYPE:
			total_size += io.write(data.pointer, size);
			break;

		case Message::FLOAT_TYPE:
			total_size += io.write(&data.as_float, size);
			break;

		case Message::DOUBLE_TYPE:
			total_size += io.write(data.pointer, size);
			break;

		case Message::STRING_TYPE:
			total_size += io.write(&size, sizeof(size));
			additional_size += sizeof(size);

			total_size += io.write(data.pointer, size);
			break;

		case Message::COLOR_TYPE:
			total_size += io.write(data.pointer, size);
			break;

		case Message::POINT_TYPE:
			total_size += io.write(data.pointer, size);
			break;

		case Message::RECT_TYPE:
			total_size += io.write(data.pointer, size);
			break;

		case Message::MESSAGE_TYPE:
			{
				total_size += io.write(&size, sizeof(size));
				additional_size += sizeof(size);

				DataIO::Size flattened_size = 0;
				sts = reinterpret_cast<Message*>(data.pointer)->flatten(&io, &flattened_size);
				total_size += flattened_size;
			}
			break;

		case Message::POINTER_TYPE:
			sts.reset(st::ERR, "POINTER_TYPE の flattening はサポートされていません\n");
			total_size += size;// エラーメッセージ抑制のため
			break;
		}

		if(total_size != size + additional_size)
			sts.reset(st::FAILED, "Message::flatten が正しく処理されませんでした\n");

		return total_size;
	}
	Message::Size unflatten(DataIO* pio, status_t* status = NULL)
	{
		assert(pio);

		status_t sts_, &sts = status ? *status : sts_;
		sts = st::OK;

		if(type != Message::UNKNOWN_TYPE)
		{
			sts.reset(st::ERR, "初期化済みのデータに unflattening することは出来ません\n");
			return 0;
		}

		DataIO& io = *pio;
		DataIO::Size total_size = 0, additional_size = 0;

		total_size += io.read(&type, sizeof(type));
		additional_size += sizeof(type);

		switch(type)
		{
		case Message::BOOL_TYPE:
			size = sizeof(bool);
			total_size += io.read(&data.as_bool, size);
			break;

		case Message::INT_TYPE:
			size = sizeof(int);
			total_size += io.read(&data.as_int, size);
			break;

		case Message::INT8_TYPE:
			size = sizeof(int8);
			total_size += io.read(&data.as_int8, size);
			break;

		case Message::INT16_TYPE:
			size = sizeof(int16);
			total_size += io.read(&data.as_int16, size);
			break;

		case Message::INT32_TYPE:
			size = sizeof(int32);
			total_size += io.read(&data.as_int32, size);
			break;

		case Message::INT64_TYPE:
			size = sizeof(int64);

			data.pointer = new int64;
			total_size += io.read(data.pointer, size);
			break;

		case Message::FLOAT_TYPE:
			size = sizeof(float);
			total_size += io.read(&data.as_float, size);
			break;

		case Message::DOUBLE_TYPE:
			size = sizeof(double);

			data.pointer = new double;
			total_size += io.read(data.pointer, size);
			break;

		case Message::STRING_TYPE:
			{
				total_size += io.read(&size, sizeof(size));
				additional_size += sizeof(size);

				char_t* buf = new char_t[size +1];
				total_size += io.read(buf, size);
				buf[size] = '\0';
				data.pointer = buf;
			}
			break;

		case Message::COLOR_TYPE:
			size = sizeof(RGBColor);
			data.pointer = new RGBColor;
			total_size += io.read(data.pointer, size);
			break;

		case Message::POINT_TYPE:
			size = sizeof(Point);
			data.pointer = new Point;
			total_size += io.read(data.pointer, size);
			break;

		case Message::RECT_TYPE:
			size = sizeof(Rect);
			data.pointer = new Rect;
			total_size += io.read(data.pointer, size);
			break;

		case Message::MESSAGE_TYPE:
			{
				total_size += io.read(&size, sizeof(size));
				additional_size += sizeof(size);

				Message* val = new Message;
				val->unflatten(&io);
				total_size += val->flattened_size();
				data.pointer = val;
			}
			break;

		case Message::POINTER_TYPE:
			sts.reset(st::ERR, "POINTER_TYPE の flattening はサポートされていません\n");

			size = sizeof(void*);
			total_size += size;// エラーメッセージ抑制のため
			break;
		}

		if(total_size != size + additional_size)
			sts.reset(st::FAILED, "Message::unflatten が正しく処理されませんでした\n");

		return total_size;
	}

private:
	//
	// functions
	//
	operator=(MessageData&);

	void delete_data_()
	{
		switch(type)
		{
		case Message::BOOL_TYPE:
		case Message::INT_TYPE:
		case Message::INT8_TYPE:
		case Message::INT16_TYPE:
		case Message::INT32_TYPE:
		case Message::FLOAT_TYPE:
		case Message::POINTER_TYPE:
			// do nothing
			break;

		case Message::INT64_TYPE:
			delete reinterpret_cast<int64*>(data.pointer);
			break;

		case Message::DOUBLE_TYPE:
			delete reinterpret_cast<double*>(data.pointer);
			break;

		case Message::STRING_TYPE:
			delete [] reinterpret_cast<char_t*>(data.pointer);
			break;

		case Message::POINT_TYPE:
			delete reinterpret_cast<Point*>(data.pointer);
			break;

		case Message::COLOR_TYPE:
			delete reinterpret_cast<RGBColor*>(data.pointer);
			break;

		case Message::RECT_TYPE:
			delete reinterpret_cast<Rect*>(data.pointer);
			break;

		case Message::MESSAGE_TYPE:
			delete reinterpret_cast<Message*>(data.pointer);
			break;
		}
	}

};// struct MessageData


struct MessageDataList : public std::vector<MessageData*> {
	//
	// data
	//
	String name;

	//
	// methods
	//
	MessageDataList(const String& name_, int num_items = 0)
	:	std::vector<MessageData*>(num_items, NULL),
		name(name_)
	{
//		reserve(num_items);
	}
	~MessageDataList()
	{
		for(iterator it = begin(); it != end(); ++it)
			delete *it;
	}

};// struct MessageDataList


class MessageBuffer : public RefCountBuffer {
private:
	//
	// types
	//
	typedef HashTable<MessageDataList*> DataTable;

public:
	//
	// types
	//
	typedef DataTable::size_type size_t;
	typedef DataTable::iterator iterator;
	typedef const iterator const_iterator;

	//
	// methods
	//
	MessageBuffer(uint table_size = 4)
	:	RefCountBuffer()
	,	m_data_table(table_size)
	{}
	MessageBuffer(const MessageBuffer& buf)
	:	RefCountBuffer()
	{
		for(DataTable::iterator list_it = buf.m_data_table.begin(); list_it != buf.m_data_table.end(); ++list_it)
		for(MessageDataList::iterator data_it = (*list_it)->begin(); data_it != (*list_it)->end(); ++data_it)
			add_data((*list_it)->name, (*data_it)->type, (*data_it)->data, (*data_it)->size);
	}
	virtual ~MessageBuffer()
	{
		m_data_table.disable_rehash();

		DataTable::iterator table_end = m_data_table.end();
		for(DataTable::iterator it = m_data_table.begin(); it != table_end; ++it)
			delete *it;

		m_data_table.enable_rehash();
	}

	//
	// methods
	//

	// add_*
	status_t add_data(
		const String& name,
		Message::ValueType type,
		Message::Value data,
		size_t size,
		bool fixed_size = true,
		int num_items = 0)
	{
		DataTable::iterator it = m_data_table.find(name);
		MessageDataList* list;

		if(it != m_data_table.end())
			list = *it;
		else
		{
			list = new MessageDataList(name, num_items);
			m_data_table.insert(name, list);
		}

		MessageData* msg_data = new MessageData;
		msg_data->type = type;
		msg_data->data = data;
		msg_data->size = size;

		list->push_back(msg_data);

		return st::OK;
	}

	// add_*
	status_t replace_data(
		const String& name,
		Message::ValueType type,
		Message::Value data,
		size_t size,
		int index)
	{
		DataTable::iterator it = m_data_table.find(name);
		MessageDataList* list;

		if(it != m_data_table.end())
			list = *it;
		else
			return status_t(st::ERR, "MessageBuffer::replace_data(): " + name + "が見つかりません\n");

		if(index < 0 || index >= list->size())
			return status_t(st::ERR, "MessageBuffer::replace_data(): インデックスが範囲を超えています\n");

		MessageData* msg_data = new MessageData;
		msg_data->type = type;
		msg_data->data = data;
		msg_data->size = size;

		if((*list)[index])
			delete (*list)[index];

		(*list)[index] = msg_data;

		return st::OK;
	}

	bool has_data(const String& name, Message::ValueType type, int index)
	{
		DataTable::iterator it = m_data_table.find(name);
		if(it == m_data_table.end())
			return false;

		MessageDataList& list = **it;
		if(index < 0 || index >= list.size())
			return false;

		assert(list[index]);
		return list[index]->type == type;
	}

	// find_*
	status_t find_data(
		const String& name,
		Message::ValueType type,
		Message::Value* data,
		size_t* size,
		int index)
	{
		if(data == NULL)
			return status_t(st::ERR, "Message::find_data(): " + name + "が見つかりませんでした\n");

		DataTable::iterator it = m_data_table.find(name);

		try {
			if(it == m_data_table.end())
				throw status_t(st::ERR, "Message::find_data(): " + name + " が見つかりませんでした");
			else if((index < 0) || (abs((int)(*it)->size()) <= index))
				throw status_t(st::INDEX_ERROR);

			MessageData* msg_data = (*it)->at(index);

			if(msg_data == NULL)
				throw status_t(st::BAD_VALUE_ERROR, "MessageBuffer::find_data(): index が不正です\n");
			if(msg_data->type != type)
				throw status_t(st::BAD_VALUE_ERROR, "MessageBuffer::find_data(): type が適合していません\n");

			*data = msg_data->data;
			*size = msg_data->size;
		}
		catch(status_t& status)
		{
			data->as_int = 0;
			*size = sizeof(int);
			return status;
		}
		catch(...) {throw;}

		return st::OK;
	}

	status_t remove_name(const String& name)
	{
		DataTable::iterator it = m_data_table.find(name);

		if(it == m_data_table.end())
			return status_t(st::ERR, "Message::remove_name(): " + name + " が見つかりませんでした\n");

		delete *it;
		m_data_table.erase(it);

		return st::OK;
	}
	status_t remove_data(const String& name, int index)
	{
		DataTable::iterator it = m_data_table.find(name);
		MessageDataList& list = **it;

		if(it == m_data_table.end())
			return status_t(st::ERR, "Message::remove_data(): " + name + "が見つかりませんでした\n");
		else if(index < 0 || list.size() <= index)
			return st::INDEX_ERROR;

		list.erase(list.begin() + index);

		return st::OK;
	}

	void resize_list_directly(const String& name, DataTable::size_type size)
	{
		MessageDataList* list = NULL;

		DataTable::iterator it = m_data_table.find(name);
		if(it == m_data_table.end())
			m_data_table.insert(name, list = new MessageDataList(name, size));
		else
		{
			list = *it;
			list->resize(size, NULL);
		}
	}
	void insert_directly(const String& name, uint index, MessageData* data)
	{
		// 注意： 直前に resize_list_directly でリストのサイズを設定しておくこと！
		assert(data);

		DataTable::iterator it = m_data_table.find(name);
		assert(it != m_data_table.end());

		MessageDataList& list = **it;
		assert(index < list.size());

		if(list[index])// 上書か？
			delete list[index];

		list[index] = data;
	}

	//
	// accessors
	//
	iterator begin() {return m_data_table.begin();}
	iterator end()   {return m_data_table.end();}
	const_iterator begin() const {return m_data_table.begin();}
	const_iterator end() const   {return m_data_table.end();}

	size_t count_names() const {return m_data_table.size();}

private:
	//
	// data
	//
	DataTable m_data_table;

};// class MessageBuffer


//########################################################
// public methods

static const MSG g_empty_msg = {NULL, NTK_MESSAGE, 0, 0, 0, {0, 0}};

Message::Message()
:	RefCountable(NULL),
	what(0),
	m_msg(g_empty_msg),
	m_target(NULL),
	m_reply_handler(NULL)
{
}

Message::Message(uint command)
:	RefCountable(NULL),
	what(command),
	m_msg(g_empty_msg),
	m_target(NULL),
	m_reply_handler(NULL)
{
}

Message::Message(uint sys_what, WPARAM wparam, LPARAM lparam)
:	RefCountable(NULL),
	what(sys_what),
	m_target(NULL),
	m_reply_handler(NULL)
{
	m_msg.hwnd		= NULL;
	m_msg.message	= sys_what;
	m_msg.wParam	= wparam;
	m_msg.lParam	= lparam;
	m_msg.time = 0;
	m_msg.pt.x = 0;
	m_msg.pt.y = 0;
}

Message::Message(const MSG& msg)
:	RefCountable(NULL)
{
	operator=(msg);
}

Message::Message(const Message& rhs)
:	RefCountable(NULL)
{
	operator=(rhs);
}

Message&
Message::operator=(const Message& rhs)
{
	RefCountable::operator=(rhs);

	what            = rhs.what;
	m_msg           = rhs.m_msg;
	m_target        = rhs.m_target;
	m_reply_handler = rhs.m_reply_handler;

	return *this;
}

Message&
Message::operator=(const MSG& rhs)
{
	set_ref_count_buffer(NULL);

	what            = rhs.message;
	m_msg           = rhs;
	m_target        = NULL;
	m_reply_handler = NULL;

	return *this;
}

Message::~Message()
{
}

//--------------------------------------------------------
// add_*

status_t
Message::add_data(const String& name, Message::ValueType type, void* data, size_t size, bool fixed_size, int num_items)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, type, Value(data), size, fixed_size, num_items);
}

status_t
Message::add_bool(const String& name, bool data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, BOOL_TYPE, Value(data), sizeof(bool));
}

status_t
Message::add_int(const String& name, int data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, INT_TYPE, Value(data), sizeof(int));
}

status_t
Message::add_int8(const String& name, int8 data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, INT8_TYPE, Value(data), sizeof(int8));
}

status_t
Message::add_int16(const String& name, int16 data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, INT16_TYPE, Value(data), sizeof(int16));
}

status_t
Message::add_int32(const String& name, int32 data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, INT32_TYPE, Value(data), sizeof(int32));
}

status_t
Message::add_int64(const String& name, int64 data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, INT64_TYPE, Value((void*)new int64(data)), sizeof(int64));
}

status_t
Message::add_float(const String& name, float data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, FLOAT_TYPE, Value(data), sizeof(float));
}

status_t
Message::add_double(const String& name, double data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, DOUBLE_TYPE, Value((void*)new double(data)), sizeof(double));
}

status_t
Message::add_string(const String& name, const String& str)
{
	size_t length = str.length();
	char_t* data = new char_t[length +1];
	_tcscpy(data, str.c_str());

	return msg_buffer(WRITE_MODE)->add_data(
		name, STRING_TYPE, Value((void*)data), length);
}

status_t
Message::add_color(const String& name, const RGBColor& data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, COLOR_TYPE, Value((void*)new RGBColor(data)), sizeof(RGBColor));
}

status_t
Message::add_point(const String& name, const Point& data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, POINT_TYPE, Value((void*)new Point(data)), sizeof(Point));
}

status_t
Message::add_rect(const String& name, const Rect& data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, RECT_TYPE, Value((void*)new Rect(data)), sizeof(Rect));
}

status_t
Message::add_region(const String& name, const Region& data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, REGION_TYPE, Value((void*)new Region(data)), sizeof(Region));
}

status_t
Message::add_message(const String& name, const Message& data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, MESSAGE_TYPE, Value((void*)new Message(data)), data.flattened_size());
}

status_t
Message::add_pointer(const String& name, void* data)
{
	return msg_buffer(WRITE_MODE)->add_data(
		name, POINTER_TYPE, Value(data), sizeof(void*));
}

//--------------------------------------------------------
// replace_*

status_t
Message::replace_data(const String& name, Message::ValueType type, void* data, size_t size, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, type, Value(data), size, index);
}

status_t
Message::replace_bool(const String& name, bool data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, BOOL_TYPE, Value(data), sizeof(bool), index);
}

status_t
Message::replace_int(const String& name, int data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, INT_TYPE, Value(data), sizeof(int), index);
}

status_t
Message::replace_int8(const String& name, int8 data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, INT8_TYPE, Value(data), sizeof(int8), index);
}

status_t
Message::replace_int16(const String& name, int16 data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, INT16_TYPE, Value(data), sizeof(int16), index);
}

status_t
Message::replace_int32(const String& name, int32 data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, INT32_TYPE, Value(data), sizeof(int32), index);
}

status_t
Message::replace_int64(const String& name, int64 data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, INT64_TYPE, Value((void*)new int64(data)), sizeof(int64), index);
}

status_t
Message::replace_float(const String& name, float data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, FLOAT_TYPE, Value(data), sizeof(float), index);
}

status_t
Message::replace_double(const String& name, double data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, DOUBLE_TYPE, Value((void*)new double(data)), sizeof(double), index);
}

status_t
Message::replace_string(const String& name, const String& str, int index)
{
	size_t length = str.length();
	char_t* data = new char_t[length +1];
	_tcscpy(data, str.c_str());// コピーを取る

	return msg_buffer(WRITE_MODE)->replace_data(
		name, STRING_TYPE, Value((void*)data), length, index);
}

status_t
Message::replace_color(const String& name, const RGBColor& data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, COLOR_TYPE, Value((void*)new RGBColor(data)), sizeof(RGBColor), index);
}

status_t
Message::replace_point(const String& name, const Point& data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, POINT_TYPE, Value((void*)new Point(data)), sizeof(Point), index);
}

status_t
Message::replace_rect(const String& name, const Rect& data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, RECT_TYPE, Value((void*)new Rect(data)), sizeof(Rect), index);
}

status_t
Message::replace_region(const String& name, const Region& data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, REGION_TYPE, Value((void*)new Region(data)), sizeof(Region), index);
}

status_t
Message::replace_message(const String& name, const Message& data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, MESSAGE_TYPE, Value((void*)new Message(data)), data.flattened_size(), index);
}

status_t
Message::replace_pointer(const String& name, void* data, int index)
{
	return msg_buffer(WRITE_MODE)->replace_data(
		name, POINTER_TYPE, Value(data), sizeof(void*), index);
}

//--------------------------------------------------------
// has_*

bool
Message::has_data(const String& name, ValueType type, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return msg_buffer()->has_data(name, type, index);
}

bool
Message::has_bool(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, BOOL_TYPE, index);
}

bool
Message::has_int(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, INT_TYPE, index);
}

bool
Message::has_int8(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, INT8_TYPE, index);
}

bool
Message::has_int16(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, INT16_TYPE, index);
}

bool
Message::has_int32(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, INT32_TYPE, index);
}

bool
Message::has_int64(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, INT64_TYPE, index);
}

bool
Message::has_float(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, FLOAT_TYPE, index);
}

bool
Message::has_double(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, DOUBLE_TYPE, index);
}

bool
Message::has_string(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, STRING_TYPE, index);
}

bool
Message::has_color(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, COLOR_TYPE, index);
}

bool
Message::has_point(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, POINT_TYPE, index);
}

bool
Message::has_rect(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, RECT_TYPE, index);
}

bool
Message::has_region(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, REGION_TYPE, index);
}

bool
Message::has_message(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, MESSAGE_TYPE, index);
}

bool
Message::has_pointer(const String& name, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return false;

	return buf->has_data(name, POINTER_TYPE, index);
}

//--------------------------------------------------------
// find_*

status_t
Message::find_data(const String& name, Message::ValueType type, void** data, size_t* size, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	status_t sts = buf->find_data(name, type, &value, size, index);

	*data = value.pointer;

	return sts;
}

inline status_t
Message::find_bool(const String& name, bool* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, BOOL_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.as_bool;

	return sts;
}

bool
Message::find_bool(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	bool data;
	sts = find_bool(name, &data, index);

	return data;
}

inline status_t
Message::find_int(const String& name, int* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, INT_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.as_int;

	return sts;
}

int
Message::find_int(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	int data;
	sts = find_int(name, &data, index);

	return data;
}

inline status_t
Message::find_int8(const String& name, int8* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, INT8_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.as_int8;

	return sts;
}

int8
Message::find_int8(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	int8 data;
	sts = find_int8(name, &data, index);

	return data;
}

inline status_t
Message::find_int16(const String& name, int16* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, INT16_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.as_int16;

	return sts;
}

int16
Message::find_int16(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	int16 data;
	sts = find_int16(name, &data, index);

	return data;
}

inline status_t
Message::find_int32(const String& name, int32* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, INT32_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.as_int32;

	return sts;
}

int32
Message::find_int32(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	int32 data;
	sts = find_int32(name, &data, index);

	return data;
}

inline status_t
Message::find_int64(const String& name, int64* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, INT64_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(int64*)value.pointer;

	return sts;
}

int64
Message::find_int64(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	int64 data;
	sts = find_int64(name, &data, index);

	return data;
}

inline status_t
Message::find_float(const String& name, float* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, FLOAT_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.as_float;

	return sts;
}

float
Message::find_float(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	float data;
	sts = find_float(name, &data, index);

	return data;
}

inline status_t
Message::find_double(const String& name, double* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, DOUBLE_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(double*)value.pointer;

	return sts;
}

double
Message::find_double(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	double data;
	sts = find_double(name, &data, index);

	return data;
}

inline status_t
Message::find_string(const String& name, String* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, STRING_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = (char_t*)value.pointer;

	return sts;
}

String
Message::find_string(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	String data;
	sts = find_string(name, &data, index);

	return data;
}

inline status_t
Message::find_color(const String& name, RGBColor* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, COLOR_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(RGBColor*)value.pointer;

	return sts;
}

RGBColor
Message::find_color(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	RGBColor data;
	sts = find_color(name, &data, index);

	return data;
}

inline status_t
Message::find_point(const String& name, Point* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, POINT_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(Point*)value.pointer;

	return sts;
}

Point
Message::find_point(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	Point data;
	sts = find_point(name, &data, index);

	return data;
}

inline status_t
Message::find_rect(const String& name, Rect* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, RECT_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(Rect*)value.pointer;

	return sts;
}

Rect
Message::find_rect(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	Rect data;
	sts = find_rect(name, &data, index);

	return data;
}

inline status_t
Message::find_region(const String& name, Region* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, REGION_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(Region*)value.pointer;

	return sts;
}

Region
Message::find_region(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	Region data;
	sts = find_region(name, &data, index);

	return data;
}

inline status_t
Message::find_message(const String& name, Message* data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, MESSAGE_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = *(Message*)value.pointer;

	return sts;
}

Message
Message::find_message(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	Message data;
	sts = find_message(name, &data, index);

	return data;
}

inline status_t
Message::find_pointer(const String& name, void** data, int index) const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	Message::Value value;
	size_t size;
	status_t sts = buf->find_data(name, POINTER_TYPE, &value, &size, index);

	if(sts.is_valid_without_eval())
		*data = value.pointer;

	return sts;
}

void*
Message::find_pointer(const String& name, int index, status_t* status) const
{
	status_t sts_, &sts = status ? *status : sts_;
	void* data = NULL;
	sts = find_pointer(name, &data, index);

	return data;
}

//--------------------------------------------------------
// remove_*

status_t
Message::remove_name(const String& name)
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	return buf->remove_name(name);
}

status_t
Message::remove_data(const String& name, int index)
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return st::NOT_INITIALIZED_ERROR;

	return buf->remove_data(name, index);
}

//--------------------------------------------------------


static const char* MESSAGE_ARCHIVE_IDENTIFIER        = "NTKMSG";
static const int   MESSAGE_ARCHIVE_IDENTIFIER_LENGTH = 6;


Message::Size
Message::flattened_size() const
{
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return 0;

	Size total_size = 0;

	// file identifier
	total_size += MESSAGE_ARCHIVE_IDENTIFIER_LENGTH;

	// version check
	total_size += sizeof(uint32);

	// what
	total_size += sizeof(what);

	// count_items
	total_size += sizeof(Size);

	for(MessageBuffer::iterator list_it = buf->begin(); list_it != buf->end(); ++list_it)
	{
		MessageDataList* list = *list_it;

		// name
		total_size += sizeof(String::size_type) + list->name.length();

		// count_items
		total_size += sizeof(Size);

		for(MessageDataList::iterator it = list->begin(); it != list->end(); ++it)
		{
			// data
			total_size += (*it)->size;
		} 
	}

	return total_size;
}

status_t
Message::flatten(DataIO* io, Size* num_bytes) const
{
	if(io == NULL)
		return st::BAD_VALUE_ERROR;

	status_t sts = st::OK;
	DataIO::Size size_, &total_size = num_bytes ? *num_bytes : size_;

	// file identifier
	total_size += io->write(MESSAGE_ARCHIVE_IDENTIFIER, MESSAGE_ARCHIVE_IDENTIFIER_LENGTH);

	// version check
	static const uint32 NTK_VERSION_ = NTK_VERSION;
	total_size += io->write(&NTK_VERSION_, sizeof(NTK_VERSION_));

	// what
	total_size += io->write(&what, sizeof(what));

	// count items
	uint num_names = count_names();
	total_size += io->write(&num_names, sizeof(num_names));
	MessageBuffer* buf = msg_buffer();
	if(buf == NULL)
		return sts;

	for(MessageBuffer::iterator list_it = buf->begin(); list_it != buf->end(); ++list_it)
	{
		MessageDataList* list = *list_it;

		// name
		String::size_type length = list->name.length();
		total_size += io->write(&length, sizeof(length));
		total_size += io->write(list->name.c_str(), length);

		// count items
		MessageDataList::size_type num_items = 0;
		{
			MessageDataList::iterator it, end = list->end();
			for(it = list->begin(); it != end; ++it)
				if(*it != NULL)
					++num_items;
		}
		total_size += io->write(&num_items, sizeof(MessageBuffer::size_t));

		{
			MessageDataList::const_iterator it, end = list->end();
			for(it = list->begin(); it != end; ++it)
			{
				// data
				if(*it != NULL)
					total_size += (*it)->flatten(io, &sts);
			}
		}
	}

	return sts;
}

status_t
Message::flatten(void* buffer, Size* num_bytes) const
{
	if(buffer == NULL)
		return st::BAD_VALUE_ERROR;

	MemoryIO io(buffer, 0x7fffffff);// うわ、最悪･･･(^^;
	return flatten(&io, num_bytes);
}

status_t
Message::unflatten(DataIO* io)
{
	if(io == NULL)
		return st::BAD_VALUE_ERROR;

	// 初期化と再生成
	make_empty();

	status_t sts = st::OK;
	DataIO::Size read_size = 0;

	// file identifier
	char_t identifier[MESSAGE_ARCHIVE_IDENTIFIER_LENGTH + 1];
	read_size = io->read(identifier, MESSAGE_ARCHIVE_IDENTIFIER_LENGTH);
	if(read_size != MESSAGE_ARCHIVE_IDENTIFIER_LENGTH)
		return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

	identifier[MESSAGE_ARCHIVE_IDENTIFIER_LENGTH] = '\0';
	if(_tcscmp(identifier, MESSAGE_ARCHIVE_IDENTIFIER) != 0)
		return status_t(st::ERR, "Message::unflatten(): これは Message アーカイブではありません\n");

	// version check
	uint32 ntk_version;
	read_size = io->read(&ntk_version, sizeof(ntk_version));
	if(read_size != sizeof(ntk_version))
		return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

	if(ntk_version > NTK_VERSION)
		return status_t(st::ERR, "Message::unflatten(): このライブラリ (NTK[" + NTK_Version.as_string() + "]) はこのアーカイブのバージョンに対応していません\n");

	// what
	read_size = io->read(&what, sizeof(what));
	if(read_size != sizeof(what))
		return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

	// count items
	uint count_lists;
	read_size = io->read(&count_lists, sizeof(count_lists));
	if(read_size != sizeof(count_lists))
		return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

	if(count_lists == 0)
		return sts;

	MessageBuffer* buf = msg_buffer(WRITE_MODE);

	while(count_lists--)
	{
		// name
		String::size_type length;
		read_size = io->read(&length, sizeof(length));
		if(read_size != sizeof(length))
			return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

		char_t* name_buf = new char_t[length + 1];
		read_size = io->read(name_buf, length);

		if(read_size != length)
			return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

		name_buf[length] = '\0';
		String name = name_buf;
		delete [] name_buf;

		// count items
		MessageBuffer::size_t count_items;
		read_size = io->read(&count_items, sizeof(count_items));
		if(read_size != sizeof(count_items))
			return status_t(st::FAILED, "Message::unflatten(): 読み込みに失敗しました\n");

		buf->resize_list_directly(name, count_items);

		for(uint32 i=0; i<count_items; ++i)
		{
			// data
			MessageData* data = new MessageData;
			data->unflatten(io, &sts);
			if(sts.is_valid_without_eval() == false)
				return sts;

			buf->insert_directly(name, i, data);
		}
	}

	return sts;
}

status_t
Message::unflatten(const void* buffer)
{
	if(buffer == NULL)
		return st::BAD_VALUE_ERROR;

	MemoryIO io(buffer, 0x7fffffff);// うわ、最悪･･･(^^;
	return unflatten(&io);
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// public accessors and manipulators

void
Message::set_target(Handler* target_, Handler* reply_handler_)
{
	m_target = target_;
	m_reply_handler = reply_handler_;
}

String
Message::as_string() const
{
	String str("---");

	if(sys_what() == NTK_MESSAGE)
	{
		if(what > 0x40000000)
		{
			char message_code[5];

			message_code[0] = static_cast<char>((what >> 24) & 0xff);
			message_code[1] = static_cast<char>((what >> 16) & 0xff);
			message_code[2] = static_cast<char>((what >> 8) & 0xff);
			message_code[3] = static_cast<char>((what >> 0) & 0xff);
			message_code[4] = '\0';

			str = String("\'") + message_code + "\'";
		}else{
			str = format_string("%d", what);
		}
	}else{
		const char_t* tmp = get_win32_message_name(what);

		if(tmp)
			str = String("\"") + tmp + "\"";
		else
			str = "Unknown message";
	}

	return format_string(
		"[what:%-20s] [wp:%10s] [lp:%10s]",
		str.c_str(),
		format_string("0x%x", wparam()).c_str(),
		format_string("0x%x", lparam()).c_str());
}

uint
Message::count_names() const
{
	MessageBuffer* buf = msg_buffer();

	if(buf == NULL)
		return 0;
	else
		return buf->count_names();
}

bool
Message::is_empty() const
{
	return msg_buffer() == NULL;
}

status_t
Message::make_empty()
{
	set_ref_count_buffer(NULL);
	return st::OK;
}

//--------------------------------------------------------
// public operators

bool
Message::operator==(const Message& rhs) const
{
	return
		what == rhs.what &&
		RefCountable::operator==(rhs) &&
		m_target == rhs.m_target &&
		m_reply_handler == rhs.m_reply_handler;
}

bool
Message::operator!=(const Message& rhs) const
{
	return !operator==(rhs);
}

//********************************************************
// private methods : utilities

MessageBuffer*
Message::msg_buffer(uint mode) const
{
	MessageBuffer* buffer = (MessageBuffer*)ref_count_buffer();

	if(mode == WRITE_MODE)
	{
		if(buffer == NULL)
		{
			buffer = new MessageBuffer();
			const_cast<Message*>(this)->set_ref_count_buffer(buffer);
		}
		else if(buffer->ref_count() > 1)// バッファを共有している時は複製する
		{
			buffer = new MessageBuffer(*buffer);
			const_cast<Message*>(this)->set_ref_count_buffer(buffer);
		}
	}

	return buffer;
}


//########################################################


static const struct WindowMessage {
	//
	// data
	//
	const char_t* name;
	uint what;
} g_WINDOW_MESSAGES[] = {
"WM_NULL"                         ,0x0000,
"WM_CREATE"                       ,0x0001,
"WM_DESTROY"                      ,0x0002,
"WM_MOVE"                         ,0x0003,
"WM_SIZE"                         ,0x0005,
"WM_ACTIVATE"                     ,0x0006,
"WM_SETFOCUS"                     ,0x0007,
"WM_KILLFOCUS"                    ,0x0008,
"WM_ENABLE"                       ,0x000A,
"WM_SETREDRAW"                    ,0x000B,
"WM_SETTEXT"                      ,0x000C,
"WM_GETTEXT"                      ,0x000D,
"WM_GETTEXTLENGTH"                ,0x000E,
"WM_PAINT"                        ,0x000F,
"WM_CLOSE"                        ,0x0010,
"WM_QUERYENDSESSION"              ,0x0011,
"WM_QUIT"                         ,0x0012,
"WM_QUERYOPEN"                    ,0x0013,
"WM_ERASEBKGND"                   ,0x0014,
"WM_SYSCOLORCHANGE"               ,0x0015,
"WM_ENDSESSION"                   ,0x0016,
"WM_SHOWWINDOW"                   ,0x0018,
"WM_WININICHANGE/WM_SETTINGCHANGE",0x001A,
"WM_DEVMODECHANGE"                ,0x001B,
"WM_ACTIVATEAPP"                  ,0x001C,
"WM_FONTCHANGE"                   ,0x001D,
"WM_TIMECHANGE"                   ,0x001E,
"WM_CANCELMODE"                   ,0x001F,
"WM_SETCURSOR"                    ,0x0020,
"WM_MOUSEACTIVATE"                ,0x0021,
"WM_CHILDACTIVATE"                ,0x0022,
"WM_QUEUESYNC"                    ,0x0023,
"WM_GETMINMAXINFO"                ,0x0024,
"WM_PAINTICON"                    ,0x0026,
"WM_ICONERASEBKGND"               ,0x0027,
"WM_NEXTDLGCTL"                   ,0x0028,
"WM_SPOOLERSTATUS"                ,0x002A,
"WM_DRAWITEM"                     ,0x002B,
"WM_MEASUREITEM"                  ,0x002C,
"WM_DELETEITEM"                   ,0x002D,
"WM_VKEYTOITEM"                   ,0x002E,
"WM_CHARTOITEM"                   ,0x002F,
"WM_SETFONT"                      ,0x0030,
"WM_GETFONT"                      ,0x0031,
"WM_SETHOTKEY"                    ,0x0032,
"WM_GETHOTKEY"                    ,0x0033,
"WM_QUERYDRAGICON"                ,0x0037,
"WM_COMPAREITEM"                  ,0x0039,
"WM_GETOBJECT"                    ,0x003D,
"WM_COMPACTING"                   ,0x0041,
"WM_COMMNOTIFY"                   ,0x0044,
"WM_WINDOWPOSCHANGING"            ,0x0046,
"WM_WINDOWPOSCHANGED"             ,0x0047,
"WM_POWER"                        ,0x0048,
"WM_COPYDATA"                     ,0x004A,
"WM_CANCELJOURNAL"                ,0x004B,
"WM_NOTIFY"                       ,0x004E,
"WM_INPUTLANGCHANGEREQUEST"       ,0x0050,
"WM_INPUTLANGCHANGE"              ,0x0051,
"WM_TCARD"                        ,0x0052,
"WM_HELP"                         ,0x0053,
"WM_USERCHANGED"                  ,0x0054,
"WM_NOTIFYFORMAT"                 ,0x0055,
"WM_CONTEXTMENU"                  ,0x007B,
"WM_STYLECHANGING"                ,0x007C,
"WM_STYLECHANGED"                 ,0x007D,
"WM_DISPLAYCHANGE"                ,0x007E,
"WM_GETICON"                      ,0x007F,
"WM_SETICON"                      ,0x0080,
"WM_NCCREATE"                     ,0x0081,
"WM_NCDESTROY"                    ,0x0082,
"WM_NCCALCSIZE"                   ,0x0083,
"WM_NCHITTEST"                    ,0x0084,
"WM_NCPAINT"                      ,0x0085,
"WM_NCACTIVATE"                   ,0x0086,
"WM_GETDLGCODE"                   ,0x0087,
"WM_SYNCPAINT"                    ,0x0088,
"WM_NCMOUSEMOVE"                  ,0x00A0,
"WM_NCLBUTTONDOWN"                ,0x00A1,
"WM_NCLBUTTONUP"                  ,0x00A2,
"WM_NCLBUTTONDBLCLK"              ,0x00A3,
"WM_NCRBUTTONDOWN"                ,0x00A4,
"WM_NCRBUTTONUP"                  ,0x00A5,
"WM_NCRBUTTONDBLCLK"              ,0x00A6,
"WM_NCMBUTTONDOWN"                ,0x00A7,
"WM_NCMBUTTONUP"                  ,0x00A8,
"WM_NCMBUTTONDBLCLK"              ,0x00A9,
"WM_KEYFIRST"                     ,0x0100,
"WM_KEYDOWN"                      ,0x0100,
"WM_KEYUP"                        ,0x0101,
"WM_CHAR"                         ,0x0102,
"WM_DEADCHAR"                     ,0x0103,
"WM_SYSKEYDOWN"                   ,0x0104,
"WM_SYSKEYUP"                     ,0x0105,
"WM_SYSCHAR"                      ,0x0106,
"WM_SYSDEADCHAR"                  ,0x0107,
"WM_KEYLAST"                      ,0x0108,
"WM_IME_STARTCOMPOSITION"         ,0x010D,
"WM_IME_ENDCOMPOSITION"           ,0x010E,
"WM_IME_COMPOSITION"              ,0x010F,
"WM_IME_KEYLAST"                  ,0x010F,
"WM_INITDIALOG"                   ,0x0110,
"WM_COMMAND"                      ,0x0111,
"WM_SYSCOMMAND"                   ,0x0112,
"WM_TIMER"                        ,0x0113,
"WM_HSCROLL"                      ,0x0114,
"WM_VSCROLL"                      ,0x0115,
"WM_INITMENU"                     ,0x0116,
"WM_INITMENUPOPUP"                ,0x0117,
"WM_MENUSELECT"                   ,0x011F,
"WM_MENUCHAR"                     ,0x0120,
"WM_ENTERIDLE"                    ,0x0121,
"WM_MENURBUTTONUP"                ,0x0122,
"WM_MENUDRAG"                     ,0x0123,
"WM_MENUGETOBJECT"                ,0x0124,
"WM_UNINITMENUPOPUP"              ,0x0125,
"WM_MENUCOMMAND"                  ,0x0126,
"WM_CTLCOLORMSGBOX"               ,0x0132,
"WM_CTLCOLOREDIT"                 ,0x0133,
"WM_CTLCOLORLISTBOX"              ,0x0134,
"WM_CTLCOLORBTN"                  ,0x0135,
"WM_CTLCOLORDLG"                  ,0x0136,
"WM_CTLCOLORSCROLLBAR"            ,0x0137,
"WM_CTLCOLORSTATIC"               ,0x0138,
"WM_MOUSEFIRST"                   ,0x0200,
"WM_MOUSEMOVE"                    ,0x0200,
"WM_LBUTTONDOWN"                  ,0x0201,
"WM_LBUTTONUP"                    ,0x0202,
"WM_LBUTTONDBLCLK"                ,0x0203,
"WM_RBUTTONDOWN"                  ,0x0204,
"WM_RBUTTONUP"                    ,0x0205,
"WM_RBUTTONDBLCLK"                ,0x0206,
"WM_MBUTTONDOWN"                  ,0x0207,
"WM_MBUTTONUP"                    ,0x0208,
"WM_MBUTTONDBLCLK"                ,0x0209,
"WM_MOUSEWHEEL"                   ,0x020A,
"WM_MOUSELAST"                    ,0x020A,
"WM_MOUSELAST"                    ,0x0209,
"WM_PARENTNOTIFY"                 ,0x0210,
"WM_ENTERMENULOOP"                ,0x0211,
"WM_EXITMENULOOP"                 ,0x0212,
"WM_NEXTMENU"                     ,0x0213,
"WM_SIZING"                       ,0x0214,
"WM_CAPTURECHANGED"               ,0x0215,
"WM_MOVING"                       ,0x0216,
"WM_POWERBROADCAST"               ,0x0218,
"WM_DEVICECHANGE"                 ,0x0219,
"WM_MDICREATE"                    ,0x0220,
"WM_MDIDESTROY"                   ,0x0221,
"WM_MDIACTIVATE"                  ,0x0222,
"WM_MDIRESTORE"                   ,0x0223,
"WM_MDINEXT"                      ,0x0224,
"WM_MDIMAXIMIZE"                  ,0x0225,
"WM_MDITILE"                      ,0x0226,
"WM_MDICASCADE"                   ,0x0227,
"WM_MDIICONARRANGE"               ,0x0228,
"WM_MDIGETACTIVE"                 ,0x0229,
"WM_MDISETMENU"                   ,0x0230,
"WM_ENTERSIZEMOVE"                ,0x0231,
"WM_EXITSIZEMOVE"                 ,0x0232,
"WM_DROPFILES"                    ,0x0233,
"WM_MDIREFRESHMENU"               ,0x0234,
"WM_IME_SETCONTEXT"               ,0x0281,
"WM_IME_NOTIFY"                   ,0x0282,
"WM_IME_CONTROL"                  ,0x0283,
"WM_IME_COMPOSITIONFULL"          ,0x0284,
"WM_IME_SELECT"                   ,0x0285,
"WM_IME_CHAR"                     ,0x0286,
"WM_IME_REQUEST"                  ,0x0288,
"WM_IME_KEYDOWN"                  ,0x0290,
"WM_IME_KEYUP"                    ,0x0291,
"WM_MOUSEHOVER"                   ,0x02A1,
"WM_MOUSELEAVE"                   ,0x02A3,
"WM_CUT"                          ,0x0300,
"WM_COPY"                         ,0x0301,
"WM_PASTE"                        ,0x0302,
"WM_CLEAR"                        ,0x0303,
"WM_UNDO"                         ,0x0304,
"WM_RENDERFORMAT"                 ,0x0305,
"WM_RENDERALLFORMATS"             ,0x0306,
"WM_DESTROYCLIPBOARD"             ,0x0307,
"WM_DRAWCLIPBOARD"                ,0x0308,
"WM_PAINTCLIPBOARD"               ,0x0309,
"WM_VSCROLLCLIPBOARD"             ,0x030A,
"WM_SIZECLIPBOARD"                ,0x030B,
"WM_ASKCBFORMATNAME"              ,0x030C,
"WM_CHANGECBCHAIN"                ,0x030D,
"WM_HSCROLLCLIPBOARD"             ,0x030E,
"WM_QUERYNEWPALETTE"              ,0x030F,
"WM_PALETTEISCHANGING"            ,0x0310,
"WM_PALETTECHANGED"               ,0x0311,
"WM_HOTKEY"                       ,0x0312,
"WM_PRINT"                        ,0x0317,
"WM_PRINTCLIENT"                  ,0x0318,
"WM_HANDHELDFIRST"                ,0x0358,
"WM_HANDHELDLAST"                 ,0x035F,
"WM_AFXFIRST"                     ,0x0360,
"WM_AFXLAST"                      ,0x037F,
"WM_PENWINFIRST"                  ,0x0380,
"WM_PENWINLAST"                   ,0x038F,
"WM_APP"                          ,0x8000,
"WM_USER"                         ,0x0400,
NULL															,0
};

const char_t*
get_win32_message_name(uint what)
{
	for(int i=0; g_WINDOW_MESSAGES[i].name; ++i)
		if(what == g_WINDOW_MESSAGES[i].what)
			return g_WINDOW_MESSAGES[i].name;

	return NULL;
}


//########################################################
}// namespace ntk
