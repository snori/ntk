/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dataio.h,v 1.7 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_SUPPORT_DATAIO_H__
#define __NTK_SUPPORT_DATAIO_H__


#include <stdio.h>
#include <boost/utility.hpp>

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif


namespace ntk {


class DataIO : public boost::noncopyable {
public:
	//
	// types
	//
	typedef size_t Size;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Size size_t;
	#endif

	//
	// methods
	//
	NtkExport DataIO();
	NtkExport virtual ~DataIO();

	virtual Size read(void* data, Size num_bytes) =0;
	virtual Size write(const void* data, Size num_bytes) =0;

};// class DataIO


class PositionIO : public DataIO {
public:
	//
	// types
	//
	typedef size_t Offset;

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
		typedef Offset offset_t;
	#endif

	//
	// methods
	//
	NtkExport PositionIO();
	NtkExport virtual ~PositionIO();

	NtkExport virtual Size read_at(Offset position, void* buffer, Size num_bytes);
	NtkExport virtual Size write_at(Offset positoin, const void* buffer, Size num_bytes);

	//
	// accessors
	//
	virtual Offset position() const =0;
	virtual Offset seek(Offset position, uint flag = SEEK_SET) =0;
	NtkExport virtual Size size() const;

};// class PositionIO


class MemoryIO : public PositionIO {
public:
	//
	// methods
	//
	NtkExport MemoryIO(void* buffer, Size num_bytes);
	NtkExport MemoryIO(const void* buffer, Size num_bytes);
	NtkExport virtual ~MemoryIO();

	NtkExport virtual Size read(void* data, Size num_bytes);
	NtkExport virtual Size write(const void* data, Size num_bytes);

	//
	// accessors
	//
	NtkExport virtual const void* buffer() const;

	NtkExport virtual Size position() const;
	NtkExport virtual Size seek(Offset position, uint flag = SEEK_SET);
	NtkExport virtual Size size() const;

private:
	//
	// data
	//
	void* m_buffer;
	Size m_size, m_position;
	bool m_const;

};// class MemoryIO


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef DataIO     data_io_t;
	typedef PositionIO position_io_t;
	typedef MemoryIO   memory_io_t;
#endif


} namespace Ntk = ntk;


template<class T>
ntk::DataIO& operator>>(ntk::DataIO& lhs, T& rhs)
{
	lhs.read(&rhs, sizeof(rhs));
	return lhs;
}

template<class T>
ntk::DataIO& operator<<(ntk::DataIO& lhs, const T& rhs)
{
	lhs.write(&rhs, sizeof(rhs));
	return lhs;
}


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::DataIO     NDataIO;
	typedef ntk::PositionIO NPositionIO;
	typedef ntk::MemoryIO   NMemoryIO;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::DataIO     ntk_data_io;
	typedef ntk::PositionIO ntk_position_io;
	typedef ntk::MemoryIO   ntk_memory_io;
	#endif
#endif


#endif//EOH
