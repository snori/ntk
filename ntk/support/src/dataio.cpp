/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: dataio.cpp,v 1.4 2003/11/11 12:07:10 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/support/dataio.h"


#include <tchar.h>
#include <ntk/windows/windows.h>
#include <ntk/support/debug.h>


namespace ntk {


//########################################################
// public methods

DataIO::DataIO()
{
}

DataIO::~DataIO()
{
}


//########################################################
// public methods

PositionIO::PositionIO()
{
}

PositionIO::~PositionIO()
{
}

PositionIO::Offset
PositionIO::read_at(Offset position, void* buffer, Size size)
{
	seek(position);
	return read(buffer, size);
}

PositionIO::Size
PositionIO::write_at(Size position, const void* buffer, Size size)
{
	seek(position);
	return write(buffer, size);
}

PositionIO::Size
PositionIO::size() const
{
	Size current = position();

	PositionIO* self = const_cast<PositionIO*>(this);
	Size size_ = self->seek(0, SEEK_END) - self->seek(0);

	self->seek(current);

	return size_;
}


//########################################################
// public methods

MemoryIO::MemoryIO(void* buffer, Size num_bytes)
:	m_buffer(buffer)
,	m_size(num_bytes)
,	m_position(0)
,	m_const(false)
{
}

MemoryIO::MemoryIO(const void* buffer, Size num_bytes)
:	m_buffer(const_cast<void*>(buffer))
,	m_size(num_bytes)
,	m_position(0)
,	m_const(true)
{
}

MemoryIO::~MemoryIO()
{
}

MemoryIO::Size
MemoryIO::read(void* data, Size num_bytes)
{
	Size pos = position();
	if(pos + num_bytes > m_size)
		num_bytes = m_size - pos;

	memcpy(data, m_buffer, num_bytes);

	return num_bytes;
}

MemoryIO::Size
MemoryIO::write(const void* data, Size num_bytes)
{
	if(m_const)
		return 0;

	Size pos = position();
	if(pos + num_bytes > m_size)
		num_bytes = m_size - pos;

	memcpy(m_buffer, data, num_bytes);

	return num_bytes;
}

//********************************************************
// public accessors

const void*
MemoryIO::buffer() const
{
	return m_buffer;
}

MemoryIO::Size
MemoryIO::position() const
{
	return m_position;
}

MemoryIO::Size
MemoryIO::seek(Offset position, uint flag)
{
	switch(flag)
	{
	case SEEK_SET:
		m_position = position;
		break;

	case SEEK_CUR:
		m_position += position;
		break;

	case SEEK_END:
		m_position = m_size + position;
		break;

#ifdef NTK_DEBUG
	default:
		error(_T("MemoryIO::seek: flag ÇÃílÇ™ïsê≥Ç≈Ç∑ÅB\n"));
#endif
	}

	if(m_position < 0)           m_position = 0;
	else if(m_position > m_size) m_position = m_size;

	return m_position;
}

MemoryIO::Size
MemoryIO::size() const
{
	return m_size;
}


//########################################################
}// namespace ntk
