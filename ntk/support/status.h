/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: status.h,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_STATUS_H__
#define __NTK_SUPPORT_STATUS_H__


#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_STRING_H__
#include <ntk/support/string.h>
#endif

#ifndef __NTK_SUPPORT_REFCOUNT_H__
#include <ntk/support/refcount.h>
#endif

#ifndef __NTK_SUPPORT_DEBUG_H__
#include <ntk/support/debug.h>
#endif


namespace ntk {


struct st {
	//
	// constants
	//
	enum {
		OK        = 0,
		SUCCEEDED = 0,
		NO_ERR    = 0,

		// error codes: 0未満
		ERR          = -1,// 'ERROR' is #defined!! ;-(
		FAILED       = -2,
		SYSTEM_ERROR = -3,
		ANSI_ERROR   = -4,

		NO_MEMORY_ERROR       = -5,
		BAD_VALUE_ERROR       = -6,
		NOT_INITIALIZED_ERROR = -7,
		INDEX_ERROR           = -8,

		SUCCESS_LAST = OK +1,
		ERROR_LAST   = INDEX_ERROR -1
	};

	//
	// methods
	//
	NtkExport static String default_message(int what);
};


template<typename ST = st>
class Status : public ST {
public:
	//
	// methods
	//
	Status() {}
	Status(int what)
	{
		reset(what);
	}
	Status(int what, bool evaluated)
	{
		reset(what, evaluated);
	}
	Status(int what, const char_t* message, bool evaluated = false)
	{
		reset(what, message, evaluated);
	}
	Status(int what, const String& message, bool evaluated = false)
	{
		reset(what, message, evaluated);
	}
	Status(bool boolean)
	{
		reset(boolean ? OK : ERR);
	}
	virtual ~Status() {}

	void show_error() const
	{
		error(message());
	}

	//
	// accessors and manipulators
	//
	int what(bool evaluate_ = true) const
	{
		if(m_buffer)
		{
			if(evaluate_)
				const_cast<RefCounter<Buffer>&>(m_buffer)->evaluated = true;
			return m_buffer->what;
		}
		else return OK;// バッファが無効の時は OK を表す
	}
	String message(bool evaluate_ = true) const
	{
		if(m_buffer)
		{
			if(evaluate_)
				const_cast<RefCounter<Buffer>&>(m_buffer)->evaluated = true;
			return m_buffer->message;
		}
		else return "";// バッファが無効の時は OK を表す
	}
	void reset(int what_, bool evaluated_ = false)
	{
		if(what_ == OK)
			m_buffer.set_ref_count_buffer(NULL);
		else
			m_buffer.set_ref_count_buffer(
				new Buffer(what_, default_message(what_), evaluated_));
	}
	void reset(int what_, const char_t* message_, bool evaluated_ = false)
	{
		m_buffer.set_ref_count_buffer(new Buffer(what_, message_, evaluated_));
	}
	void reset(int what_, const String& message_, bool evaluated_ = false)
	{
		m_buffer.set_ref_count_buffer(new Buffer(what_, message_, evaluated_));
	}

	bool is_valid(bool show_error_message = false, bool evaluate_ = true) const
	{
		if(m_buffer.ref_count_buffer() == NULL || what(evaluate_) >= 0)
			return true;
		else
		{
			if(show_error_message)
				show_error();
			return false;
		}
	}
	bool is_valid_without_eval(bool show_error_message = false) const
	{
		return is_valid(show_error_message, false);
	}
	bool is_evaluated() const
	{
		return m_buffer ? m_buffer->evaluated : true;// バッファが無効の時は OK を表す
	}

	//
	// operators
	//
	operator bool() const {return is_valid();}
	operator Status<st>() const
	{
		if(! m_buffer)
			return st::OK;
		else
			return Status<st>(m_buffer->what, m_buffer->message, m_buffer->evaluated);
	}

private:
	//
	// types
	//
	struct Buffer : public RefCountBuffer {
		int what;
		String message;
		bool evaluated;

		Buffer(int what_, const String& error_message, bool evaluated_)
		:	what(what_), message(error_message), evaluated(what_ >= 0 || evaluated_)
		{}
		virtual ~Buffer()
		{
			if(! evaluated)
				error("未処理のエラーがあります: " + message);
		}
	};

	//
	// data
	//
	RefCounter<Buffer> m_buffer;

};// class Status


typedef Status<st> status_t;


} namespace Ntk = ntk;


#ifdef NTK_USING_BASIC_TYPES
	using ntk::status_t;
#endif

#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	#define NStatus ntk::Status
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		#define ntk_status ntk::Status

		#ifndef NTK_USING_BASIC_TYPES
		typedef ntk::status_t ntk_status_t;
		#endif
	#endif
#endif


//
// macro for success/error number definition
//
#define NTK_SUCCESS_CODE(SuperClass, num) ((SuperClass ::SUCCESS_LAST) + num)
#define NTK_ERROR_CODE(SuperClass, num) ((SuperClass ::ERROR_LAST) - num)


#endif//EOH
