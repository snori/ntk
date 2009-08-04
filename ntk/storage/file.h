/******************************************************************************

	The NTK Library
	Copyright (C) 1998-2003 Noritaka Suzuki

	$Id: file.h,v 1.6 2003/11/11 12:07:09 nsuzuki Exp $

******************************************************************************/
#pragma once
#ifndef __NTK_STORAGE_FILE_H__
#define __NTK_STORAGE_FILE_H__


#ifndef __NTK_WINDOWS_WINDOWS_H__
#include <ntk/windows/windows.h>
#endif

#ifndef __NTK_DEFS_H__
#include <ntk/defs.h>
#endif

#ifndef __NTK_SUPPORT_DATAIO_H__
#include <ntk/support/dataio.h>
#endif

#ifndef __NTK_STORAGE_NODE_H__
#include <ntk/storage/node.h>
#endif

#ifndef __NTK_SUPPORT_STATUS_H__
#include <ntk/support/status.h>
#endif


namespace ntk {


struct FileStatus : public st {
public:
	//
	// constants
	//
	enum {
		// errors
		ENTRY_NOT_FOUND     = NTK_ERROR_CODE(st, 1),
		FILE_EXISTS         = NTK_ERROR_CODE(st, 2),
		PERMISSION_DENIED   = NTK_ERROR_CODE(st, 3),
		ALREADY_INITIALIZED = NTK_ERROR_CODE(st, 4),
	};

	NtkExport static String default_message(int what);

};// struct FileStatus


class FILE : public Node, public PositionIO {
public:
	//
	// types
	//
	typedef Status<FileStatus> status_t;

	//
	// methods
	//
	NtkExport FILE();
	NtkExport FILE(const String& path, const String& access_mode);
	NtkExport FILE(const Entry& entry, const String& access_mode);
	NtkExport FILE(const Directory& dir, const String& path, const String& access_mode);
	NtkExport virtual ~FILE();

	NtkExport virtual status_t set_to(const String& path, const String& access_mode);
	NtkExport virtual status_t set_to(const Entry& entry, const String& access_mode);
	NtkExport virtual status_t set_to(const Directory& dir, const String& path, const String& access_path);
	NtkExport virtual void unset();

	NtkExport virtual status_t init_check() const;

	NtkExport virtual Size read(void* buffer, Size num_bytes);
	NtkExport virtual Size write(const void* buffer, Size num_bytes);

	// utility methods
	uint8 read_uint8();
	uint16 read_uint16();
	uint32 read_uint32();
	float read_float();
	double read_double();

	//
	// accessors
	//
	NtkExport virtual ::FILE* fp() const;
	NtkExport virtual const String& access_mode() const;

	NtkExport virtual Offset position() const;
	NtkExport virtual Offset seek(Offset offset, uint flag = SEEK_SET);

	NtkExport virtual Size size() const;

protected:
	//
	// methods
	//
	bool is_valid() const {return m_fp != NULL;}

private:
	//
	// data
	//
	::FILE* m_fp;
	String m_access_mode;

	//
	// methods
	//

	// hidden methods
	status_t set_to(const String& path);
	status_t set_to(const Entry& entry);
	status_t set_to(const Directory& dir, const String& path);

	//
	// functions
	//
	status_t set_to_(const String& access_mode);

};// class FILE


class File : public Node, public PositionIO {
public:
	//
	// types
	//
	typedef Status<FileStatus> status_t;

	//
	// constants
	//
	enum {
		READ_ONLY		= GENERIC_READ,
		WRITE_ONLY	= GENERIC_WRITE,
		READ_WRITE	= GENERIC_READ | GENERIC_WRITE,
	};//mode

	//
	// methods
	//
	NtkExport File();
	NtkExport File(
		const String& file_name,
		uint mode = READ_ONLY,
		uint share_flags = FILE_SHARE_READ,
		uint create_flags = OPEN_EXISTING,
		uint flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS);
	NtkExport File(
		const Entry& entry,
		uint mode = READ_ONLY,
		uint share_flags = FILE_SHARE_READ,
		uint create_flags = OPEN_EXISTING,
		uint flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS);
	NtkExport File(
		const Directory& dir,
		const String& path,
		uint mode = READ_ONLY,
		uint share_flags = FILE_SHARE_READ,
		uint create_flags = OPEN_EXISTING,
		uint flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS);
	NtkExport virtual ~File();

	NtkExport status_t set_to(
		const String& path,
		uint mode = READ_ONLY,
		uint share_flags = FILE_SHARE_READ,
		uint create_flags = OPEN_EXISTING,
		uint flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS);
	NtkExport status_t set_to(
		const Entry& entry,
		uint mode = READ_ONLY,
		uint share_flags = FILE_SHARE_READ,
		uint create_flags = OPEN_EXISTING,
		uint flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS);
	NtkExport status_t set_to(
		const Directory& dir,
		const String& path,
		uint mode = READ_ONLY,
		uint share_flags = FILE_SHARE_READ,
		uint create_flags = OPEN_EXISTING,
		uint flags = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS);
	NtkExport void unset();

	NtkExport virtual status_t init_check() const;

	NtkExport virtual Size read(void* buffer, Size num_bytes);
	NtkExport virtual Size write(const void* buffer, Size num_bytes);

	// utility methods
	uint8 read_uint8();
	uint16 read_uint16();
	uint32 read_uint32();
	float read_float();
	double read_double();

	//
	// accessors
	//
	NtkExport virtual HANDLE handle() const;
	NtkExport virtual uint mode() const;
	NtkExport virtual uint share_flags() const;
	NtkExport virtual uint create_flags() const;
	NtkExport virtual uint flags() const;

	NtkExport virtual Offset position() const;
	NtkExport virtual Offset seek(Offset position, uint flag = FILE_BEGIN);

	NtkExport virtual Size size() const;

protected:
	//
	// methods
	//
	bool is_valid() const {return m_handle != INVALID_HANDLE_VALUE;}

private:
	//
	// data
	//
	HANDLE m_handle;
	uint m_mode, m_share_flags, m_create_flags, m_flags;
	Size m_position;

	//
	// methods
	//

	// hidden methods
	status_t set_to(const String& path);
	status_t set_to(const Entry& entry);
	status_t set_to(const Directory& dir, const String& path);

	//
	// functions
	//
	status_t set_to_(uint mode, uint share_flags, uint create_flags, uint flags);

};// class File


#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef FILE       FILE_t;
	typedef File       file_t;
	typedef FileStatus file_statut_t;
#endif


} namespace Ntk = ntk;


#ifdef NTK_TYPEDEF_TYPES_AS_GLOBAL_TYPE
	#ifdef NTK_TYPEDEF_GLOBAL_NCLASS
	typedef ntk::FILE       NFILE;
	typedef ntk::File       NFile;
	typedef ntk::FileStatus NFileStatus;
	#endif

	#ifdef NTK_TYPEDEF_LOWERCASE_ONLY_TYPE_NAME
	typedef ntk::FILE       ntk_FILE;
	typedef ntk::File       ntk_file;
	typedef ntk::FileStatus ntk_file_status;
	#endif
#endif


#endif//EOH
