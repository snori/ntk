/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: refcount.h,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_REFCOUNT_H__
#define __NTK_SUPPORT_REFCOUNT_H__


#include <assert.h>
#include <boost/utility.hpp>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


struct RefCountBuffer : public boost::noncopyable {
	//
	// methods
	//
	RefCountBuffer() : m_ref_count(0) {}
	virtual ~RefCountBuffer() {}

	int ref_count() const {return m_ref_count;}
	void inc_ref_count() {++m_ref_count;}
	void dec_ref_count()
	{
		assert(m_ref_count > 0);

		--m_ref_count;
		if(m_ref_count <= 0)
			delete this;
	}

private:
	//
	// data
	//
	int m_ref_count;

};// class RefCountBuffer


template<typename T>
class RefCounter {
public:
	//
	// types
	//
	typedef RefCounter<T> This;
	typedef T Buffer;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef This   this_t;
		typedef Buffer buffer_t;
	#endif

	//
	// methods
	//
	RefCounter()
	:	m_ref_count_buffer(NULL)
	{}
	RefCounter(Buffer* buffer)
	:	m_ref_count_buffer(NULL)
	{
		set_ref_count_buffer(buffer);
	}
	RefCounter(const RefCounter& val)
	:	m_ref_count_buffer(NULL)
	{
		set_ref_count_buffer(val.m_ref_count_buffer);
	}
	RefCounter& operator=(const RefCounter& rhs)
	{
		if(&rhs == this || m_ref_count_buffer == rhs.m_ref_count_buffer)
			return *this;

		set_ref_count_buffer(rhs.m_ref_count_buffer);

		return *this;
	}
	~RefCounter()
	{
		set_ref_count_buffer(NULL);
	}

	//
	// accessors and manipulators
	//
	Buffer* ref_count_buffer() {return m_ref_count_buffer;}
	const Buffer* ref_count_buffer() const {return m_ref_count_buffer;}
	void set_ref_count_buffer(Buffer* buffer)// NULL: delete buffer
	{
		if(m_ref_count_buffer)
			m_ref_count_buffer->dec_ref_count();

		m_ref_count_buffer = buffer;

		if(m_ref_count_buffer)
			m_ref_count_buffer->inc_ref_count();
	}

	//
	// operators
	//
	Buffer& operator*() {return *ref_count_buffer();}
	const Buffer& operator*() const {return *ref_count_buffer();}

	Buffer* operator->() {return ref_count_buffer();}
	const Buffer* operator->() const {return ref_count_buffer();}

	operator bool() const {return ref_count_buffer() != NULL;}

private:
	//
	// data
	//
	Buffer* m_ref_count_buffer;

};// class RefCounter


class RefCountable {
public:
	//
	// methods
	//
	RefCountable()
	:	m_ref_count_buffer(NULL)
	{}
	RefCountable(RefCountBuffer* buffer)
	:	m_ref_count_buffer(NULL)
	{
		set_ref_count_buffer(buffer);
	}
	RefCountable(const RefCountable& val)
	:	m_ref_count_buffer(NULL)
	{
		set_ref_count_buffer(val.m_ref_count_buffer);
	}
	RefCountable& operator=(const RefCountable& rhs)
	{
		if(&rhs == this || m_ref_count_buffer == rhs.m_ref_count_buffer)
			return *this;

		set_ref_count_buffer(rhs.m_ref_count_buffer);

		return *this;
	}
	virtual ~RefCountable()
	{
		set_ref_count_buffer(NULL);
	}

	void set_ref_count_buffer(RefCountBuffer* buffer)
	{
		if(m_ref_count_buffer)
			m_ref_count_buffer->dec_ref_count();

		m_ref_count_buffer = buffer;

		if(m_ref_count_buffer)
			m_ref_count_buffer->inc_ref_count();
	}

	//
	// accessors
	//
	RefCountBuffer* ref_count_buffer() {return m_ref_count_buffer;}
	const RefCountBuffer* ref_count_buffer() const {return m_ref_count_buffer;}

	//
	// operators
	//
	bool operator==(const RefCountable& rhs) const
	{
		return m_ref_count_buffer == rhs.m_ref_count_buffer;
	}
	bool operator!=(const RefCountable& rhs) const
	{
		return !operator==(rhs);
	}

private:
	//
	// data
	//
	RefCountBuffer* m_ref_count_buffer;

};// class RefCountable


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef RefCountBuffer ref_count_buffer_t;
	typedef RefCountable   ref_countable_t;
	#define ref_counter_t  RefCounter
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::RefCountBuffer NRefCountBuffer;
	typedef ntk::RefCountable   NRefCountable;
	#define NRefCounter         ntk::RefCounter
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::RefCountBuffer ntk_ref_count_buffer;
	typedef ntk::RefCountable   ntk_ref_countable;
	#define ntk_ref_counter     ntk::RefCounter
	#endif
#endif


#endif//EOH
