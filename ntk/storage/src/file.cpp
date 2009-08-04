/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: file.cpp,v 1.5 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#define NTK_BUILD
#include "ntk/storage/file.h"


#include <ntk/storage/directory.h>


namespace ntk {


//########################################################


String
FileStatus::default_message(int what)
{
	switch(what)
	{
	case NOT_INITIALIZED_ERROR:
		return "ファイルが初期化されていません\n";

	case ENTRY_NOT_FOUND:
		return "ファイルが見つかりません\n";

	case PERMISSION_DENIED:
		return "パーミッションエラーです\n";

	case ALREADY_INITIALIZED:
		return "すでにほかのファイルが開かれています\n";

	default:
		return st::default_message(what);
	}
}


//########################################################
// public methods

FILE::FILE()
:	m_fp(NULL)
{
}

FILE::FILE(const String& path, const String& access_mode)
:	m_fp(NULL)
{
	set_to(path, access_mode).what();
}

FILE::FILE(const Entry& entry, const String& access_mode)
:	m_fp(NULL)
{
	set_to(entry, access_mode).what();
}

FILE::FILE(const Directory& dir, const String& path, const String& access_mode)
:	m_fp(NULL)
{
	set_to(dir, path, access_mode).what();
}

FILE::~FILE()
{
	unset();
}

FILE::status_t
FILE::set_to(const String& path, const String& access_mode)
{
	// すでに開いていたら
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(path);
	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(access_mode);
}

FILE::status_t
FILE::set_to(const Entry& entry, const String& access_mode)
{
	// すでに開いていたら
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(entry);
	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(access_mode);
}

FILE::status_t
FILE::set_to(const Directory& dir, const String& path, const String& access_mode)
{
	// すでに開いていたら
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(dir, path);
	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(access_mode);
}

void
FILE::unset()
{
	if(is_valid() == false)
		return;

	Node::unset();

	fclose(m_fp);
	m_fp = NULL;
}

FILE::status_t
FILE::init_check() const
{
	status_t sts = Node::init_check();

	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return is_valid() ? st::OK : st::NOT_INITIALIZED_ERROR;
}

FILE::Size
FILE::read(void* buffer, Size num_bytes)
{
	if(! is_valid())
		return 0;

	return fread(buffer, 1, num_bytes, m_fp);
}

FILE::Size
FILE::write(const void* buffer, Size num_bytes)
{
	if(! is_valid())
		return 0;

	return fwrite(buffer, 1, num_bytes, m_fp);
}

//--------------------------------------------------------
// public utility methods

uint8
FILE::read_uint8()
{
	uint8 val;
	read(&val, sizeof(val));
	return val;
}

uint16
FILE::read_uint16()
{
	uint16 val;
	read(&val, sizeof(val));
	return val;
}

uint32
FILE::read_uint32()
{
	uint32 val;
	read(&val, sizeof(val));
	return val;
}

float
FILE::read_float()
{
	float val;
	read(&val, sizeof(val));
	return val;
}

double
FILE::read_double()
{
	double val;
	read(&val, sizeof(val));
	return val;
}

//--------------------------------------------------------
// public accessors

::FILE*
FILE::fp() const
{
	return m_fp;
}

const String&
FILE::access_mode() const
{
	return m_access_mode;
}

FILE::Offset
FILE::position() const
{
	return ftell(m_fp);
}

FILE::Offset
FILE::seek(Offset offset, uint flags)
{
	if(is_valid() == false)
		return status_t(st::ERR, "FILE(" + path().as_string() + ")::seek(): 失敗しました\n");

	fseek(m_fp, offset, flags);

	return position();
}

FILE::Size
FILE::size() const
{
	if(is_valid() == false)
		return 0xffffffff;

	Size current = position();

	fseek(m_fp, 0, SEEK_SET);
	Size begin = ftell(m_fp);

	fseek(m_fp, 0, SEEK_END);
	Size end = ftell(m_fp);

	fseek(m_fp, current, SEEK_SET);

	return end - begin;
}

//********************************************************
// functions

FILE::status_t
FILE::set_to_(const String& access_mode)
{
	m_access_mode = access_mode;
	m_fp = fopen(path().as_string().c_str(), m_access_mode.c_str());

	if(! is_valid())
		return status_t::ENTRY_NOT_FOUND;

	return st::OK;
}

//########################################################
// public methods

File::File()
:	m_handle(INVALID_HANDLE_VALUE)
,	m_position(0)
{
}

File::File(const String& path, uint mode, uint share_flags, uint create_flags, uint flags)
:	m_handle(INVALID_HANDLE_VALUE)
,	m_position(0)
{
	set_to(path, mode, share_flags, create_flags, flags).what();
}

File::File(const Entry& entry, uint mode, uint share_flags, uint create_flags, uint flags)
:	m_handle(INVALID_HANDLE_VALUE)
,	m_position(0)
{
	set_to(entry, mode, share_flags, create_flags, flags).what();
}

File::File(const Directory& dir, const String& path, uint mode, uint share_flags, uint create_flags, uint flags)
:	m_handle(INVALID_HANDLE_VALUE)
,	m_position(0)
{
	set_to(dir, path, mode, share_flags, create_flags, flags).what();
}

File::~File()
{
	unset();
}

File::status_t
File::set_to(const String& path, uint mode, uint share_flags, uint create_flags, uint flags)
{
	// すでに開いていたら
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(path);

	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(mode, share_flags, create_flags, flags);
}

File::status_t
File::set_to(const Entry& entry, uint mode, uint share_flags, uint create_flags, uint flags)
{
	// すでに開いていたら
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(entry);

	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(mode, share_flags, create_flags, flags);
}

File::status_t
File::set_to(const Directory& dir, const String& path, uint mode, uint share_flags, uint create_flags, uint flags)
{
	// すでに開いていたら
	if(is_valid())
		return status_t::ALREADY_INITIALIZED;

	status_t sts = Node::set_to(dir, path);

	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return set_to_(mode, share_flags, create_flags, flags);
}

void
File::unset()
{
	if(is_valid() == false)
		return;

	Node::unset();

	CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

File::status_t
File::init_check() const
{
	status_t sts = Node::init_check();

	if(sts.is_valid_without_eval() == false)
		return sts;
	else
		return is_valid() ? st::OK : st::NOT_INITIALIZED_ERROR;
}

File::Size
File::read(void* buffer, Size num_bytes)
{
	if(is_valid() == false || mode() == WRITE_ONLY)
		return 0;

	DWORD read_size;
	ReadFile(m_handle, buffer, num_bytes, &read_size, NULL);
	m_position += read_size;

	return read_size;
}

File::Size
File::write(const void* buffer, Size num_bytes)
{
	if(is_valid() == false || mode() == READ_ONLY)
		return 0;

	DWORD written_size;
	WriteFile(m_handle, buffer, num_bytes, &written_size, NULL);
	m_position += written_size;

	return written_size;
}

//--------------------------------------------------------
// public utility methods

uint8
File::read_uint8()
{
	uint8 val;
	read(&val, sizeof(val));
	return val;
}

uint16
File::read_uint16()
{
	uint16 val;
	read(&val, sizeof(val));
	return val;
}

uint32
File::read_uint32()
{
	uint32 val;
	read(&val, sizeof(val));
	return val;
}

float
File::read_float()
{
	float val;
	read(&val, sizeof(val));
	return val;
}

double
File::read_double()
{
	double val;
	read(&val, sizeof(val));
	return val;
}

//--------------------------------------------------------
// public accessors

HANDLE
File::handle() const
{
	return m_handle;
}

uint
File::mode() const
{
	return m_mode;
}

uint
File::share_flags() const
{
	return m_share_flags;
}

uint
File::create_flags() const
{
	return m_create_flags;
}

uint
File::flags() const
{
	return m_flags;
}

File::Offset
File::position() const
{
	return m_position;
}

File::Offset
File::seek(Size position, uint flags)
{
	if(is_valid() == false)
		return 0xffffffff;

	Size new_position = SetFilePointer(m_handle, position, NULL, flags);

	if(new_position != 0xffffffff)
		m_position = new_position;

	return new_position;
}

File::Size
File::size() const
{
	if(is_valid() == false)
		return 0xffffffff;

	return GetFileSize(m_handle, NULL);
}

//********************************************************
// functions

File::status_t
File::set_to_(uint mode, uint share_flags, uint create_flags, uint flags)
{
	m_handle = CreateFile(
		path().as_string().c_str(),
		mode,
		share_flags,
		NULL,
		create_flags,
		flags,
		NULL);

	if(is_valid() == false)
		return st::SYSTEM_ERROR;

	m_mode = mode;
	m_share_flags;
	m_create_flags = create_flags;
	m_flags = flags;

	// 現在位置を初期化
	m_position = 0;

	return st::OK;
}


//########################################################
}// namespace ntk
